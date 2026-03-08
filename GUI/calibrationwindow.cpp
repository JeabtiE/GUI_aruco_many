#include "calibrationwindow.h"
#include "ui_calibrationwindow.h"
#include "config.h"
#include "mainwindow.h"

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <opencv2/objdetect/aruco_dictionary.hpp>
#include <opencv2/objdetect/aruco_board.hpp>
#include <iostream>

#include <QMessageBox>

CalibrationWindow::CalibrationWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CalibrationWindow)
{
    ui->setupUi(this);

    ui->label_camera->setScaledContents(true);

    ui->btn_continue->setEnabled(false);

    cap.open(0, cv::CAP_DSHOW);

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout,
            this, &CalibrationWindow::updateCamera);

    timer->start(30);
}

CalibrationWindow::~CalibrationWindow()
{
    if(timer)
        timer->stop();

    if(cap.isOpened())
        cap.release();

    delete ui;
}

void CalibrationWindow::updateCamera()
{
    cv::Mat frame;
    cap >> frame;

    if(frame.empty()) return;

    currentFrame = frame.clone();

    ///////////////////////////////////////////////////////////
    // ARUCO DETECTION
    ///////////////////////////////////////////////////////////

    static cv::aruco::Dictionary dictionary =
        cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);

    static cv::aruco::DetectorParameters params;
    static cv::aruco::ArucoDetector detector(dictionary, params);

    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f>> corners;

    detector.detectMarkers(frame, corners, ids);

    if(ids.size() > 0)
    {
        cv::aruco::drawDetectedMarkers(frame, corners, ids);

        std::vector<cv::Point2f> centers(4);
        bool found[4] = {false,false,false,false};

        for(int i=0;i<ids.size();i++)
        {
            if(ids[i] >=0 && ids[i] <=3)
            {
                cv::Point2f c;

                c.x = (corners[i][0].x + corners[i][2].x)/2;
                c.y = (corners[i][0].y + corners[i][2].y)/2;

                centers[ids[i]] = c;
                found[ids[i]] = true;

                cv::circle(frame,c,6,cv::Scalar(0,255,0),-1);
            }
        }

        ///////////////////////////////////////////////////////
        // ถ้าเห็น marker ครบ 4 ตัว
        ///////////////////////////////////////////////////////

        if(found[0] && found[1] && found[2] && found[3])
        {
            std::vector<cv::Point2f> imgPts(4);
            std::vector<cv::Point2f> worldPts(4);

            // mapping ตาม layout marker
            imgPts[0] = centers[0]; // top-left
            imgPts[1] = centers[1]; // top-right
            imgPts[2] = centers[2]; // bottom-right
            imgPts[3] = centers[3]; // bottom-left

            // ระยะจริง (mm) marker ห่างกัน 30cm
            worldPts[0] = cv::Point2f(0,0);
            worldPts[1] = cv::Point2f(300,0);
            worldPts[2] = cv::Point2f(300,300);
            worldPts[3] = cv::Point2f(0,300);

            if(!calibrated)
            {
                homography = cv::findHomography(imgPts, worldPts);

                if(!homography.empty())
                    homography_ready = true;
            }

            ui->label_scale->setText("Homography Ready");

            ////////////////////////////////////////////////////
            // ทดสอบ world coordinate
            ////////////////////////////////////////////////////

            if(homography_ready)
            {
                std::vector<cv::Point2f> src(1);
                std::vector<cv::Point2f> dst;

                src[0] = cv::Point2f(frame.cols/2, frame.rows/2);

                cv::perspectiveTransform(src, dst, homography);

                int X = (int)dst[0].x;
                int Y = (int)dst[0].y;

                std::string text =
                    "X: " + std::to_string(X) +
                    " mm  Y: " + std::to_string(Y) + " mm";

                cv::putText(frame,
                            text,
                            cv::Point(30,90),
                            cv::FONT_HERSHEY_SIMPLEX,
                            0.7,
                            cv::Scalar(0,255,0),
                            2);
            }
        }
    }

    ///////////////////////////////////////////////////////////

    if(!homography_ready)
    {
        cv::putText(frame,
                    "Place 4 Aruco markers",
                    cv::Point(30,50),
                    cv::FONT_HERSHEY_SIMPLEX,
                    0.7,
                    cv::Scalar(0,0,255),
                    2);
    }
    else
    {
        cv::putText(frame,
                    "Calibration Ready",
                    cv::Point(30,50),
                    cv::FONT_HERSHEY_SIMPLEX,
                    0.7,
                    cv::Scalar(0,255,0),
                    2);
    }

    ///////////////////////////////////////////////////////////

    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

    QImage img(frame.data,
               frame.cols,
               frame.rows,
               frame.step,
               QImage::Format_RGB888);

    ui->label_camera->setPixmap(QPixmap::fromImage(img));
}

void CalibrationWindow::on_btn_capture_clicked()
{
    if(!homography_ready)
    {
        QMessageBox::warning(this,
                             "Calibration",
                             "Aruco markers not detected!");
        return;
    }

    calibrated = true;

    qDebug() << "Homography locked";

    ui->btn_continue->setEnabled(true);
}

void CalibrationWindow::on_btn_continue_clicked()
{
    if(!homography_ready)
    {
        QMessageBox::warning(this,
                             "Calibration Required",
                             "Please detect 4 Aruco markers first!");
        return;
    }

    // หยุด timer
    if(timer)
    {
        timer->stop();
        disconnect(timer, nullptr, this, nullptr);
    }

    // ปล่อยกล้อง
    cap.release();

    MainWindow *w = new MainWindow(homography);
    w->show();

    this->close();
}
