#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QHeaderView>

using namespace cv;
using namespace std;

QString getGrade(double weight)
{
    if(weight > 3.0) return "A";
    else if(weight >= 2.0) return "B";
    else return "C";
}

MainWindow::MainWindow(cv::Mat H, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    homography = H.clone();

    if(!cap.open(0))
    {
        QMessageBox::critical(this,"Error","Cannot open camera");
        return;
    }

    cameraTimer = new QTimer(this);
    connect(cameraTimer,&QTimer::timeout,this,&MainWindow::updateCamera);
    cameraTimer->start(30);

    countdownTimer = new QTimer(this);

    connect(countdownTimer,&QTimer::timeout,this,[=](){

        countdown--;

        if(countdown<=0)
        {
            saveWatermelon();
            countdown = 15;
        }

    });

    countdownTimer->start(1000);

    ui->tableWidget->setColumnCount(5);

    ui->tableWidget->setHorizontalHeaderLabels({
        "No",
        "Width (cm)",
        "Height (cm)",
        "Weight (kg)",
        "Grade"
    });

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    cameraTimer->stop();
    countdownTimer->stop();

    if(cap.isOpened())
        cap.release();

    delete ui;
}

void MainWindow::updateCamera()
{
    if(!cap.isOpened())
        return;

    Mat frame;
    cap >> frame;

    if(frame.empty())
        return;

    if(homography.empty())
        return;

    GaussianBlur(frame,frame,Size(11,11),0.1);

    Mat hsv;
    cvtColor(frame,hsv,COLOR_BGR2HSV);

    Mat mask;

    Scalar lower(20,20,20);
    Scalar upper(110,255,255);

    inRange(hsv,lower,upper,mask);

    Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(9,9));

    morphologyEx(mask,mask,MORPH_CLOSE,kernel);
    morphologyEx(mask,mask,MORPH_OPEN,kernel);

    vector<vector<Point>> contours;
    findContours(mask,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

    melons.clear();

    for(size_t i=0;i<contours.size();i++)
    {
        double area = contourArea(contours[i]);

        if(area < 8000)
            continue;

        double perimeter = arcLength(contours[i],true);

        if(perimeter == 0)
            continue;

        double circularity = 4 * CV_PI * area /(perimeter*perimeter);

        if(circularity < 0.45)
            continue;

        RotatedRect rect = minAreaRect(contours[i]);

        Point2f box[4];
        rect.points(box);

        for(int j=0;j<4;j++)
            line(frame,box[j],box[(j+1)%4],Scalar(0,255,0),2);

        float w = rect.size.width;
        float h = rect.size.height;

        Point2f center = rect.center;

        if(w<h) swap(w,h);

        float angle = rect.angle * CV_PI /180;

        Point2f dx(cos(angle),sin(angle));
        Point2f dy(-sin(angle),cos(angle));

        Point2f x1 = center - dx*(w/2);
        Point2f x2 = center + dx*(w/2);

        Point2f y1 = center - dy*(h/2);
        Point2f y2 = center + dy*(h/2);

        vector<Point2f> src = {x1,x2,y1,y2};
        vector<Point2f> dst;

        perspectiveTransform(src,dst,homography);

        if(dst.size()!=4)
            continue;

        double width_mm = norm(dst[0]-dst[1]);
        double height_mm = norm(dst[2]-dst[3]);

        double width_cm = width_mm/10.0;
        double height_cm = height_mm/10.0;

        // สูตรน้ำหนัก (ลบ /10 ออกแล้ว)
        double weight =
            (0.0018 * ((2 * width_cm * height_cm * height_cm * 3.14) / 3)) + 0.2228;

        weight = weight / 10.0;

        Melon m;

        m.width = width_cm;
        m.height = height_cm;
        m.weight = weight;
        m.center = center;

        melons.push_back(m);
    }

    // ---------- Overlay Text ----------
    int y = 30;

    for(size_t i=0;i<melons.size();i++)
    {
        char line[100];

        sprintf(line,"ID %d", (int)i+1);
        putText(frame,line,Point(10,y),FONT_HERSHEY_SIMPLEX,0.6,Scalar(0,255,255),2);
        y += 25;

        sprintf(line,"W: %.2f cm",melons[i].width);
        putText(frame,line,Point(10,y),FONT_HERSHEY_SIMPLEX,0.6,Scalar(255,200,0),2);
        y += 25;

        sprintf(line,"H: %.2f cm",melons[i].height);
        putText(frame,line,Point(10,y),FONT_HERSHEY_SIMPLEX,0.6,Scalar(255,200,0),2);
        y += 25;

        sprintf(line,"Weight: %.2f kg",melons[i].weight);
        putText(frame,line,Point(10,y),FONT_HERSHEY_SIMPLEX,0.6,Scalar(0,255,0),2);
        y += 35;
    }

    char obj[50];
    sprintf(obj,"Objects: %d",(int)melons.size());
    putText(frame,obj,Point(10,y),FONT_HERSHEY_SIMPLEX,0.7,Scalar(0,0,255),2);

    y += 30;

    char save[50];
    sprintf(save,"Save in: %d",countdown);
    putText(frame,save,Point(10,y),FONT_HERSHEY_SIMPLEX,0.7,Scalar(0,200,255),2);

    // ---------- Display ----------
    cvtColor(frame,frame,COLOR_BGR2RGB);

    QImage img(frame.data,
               frame.cols,
               frame.rows,
               frame.step,
               QImage::Format_RGB888);

    ui->label_camera->setPixmap(QPixmap::fromImage(img.copy()));
}

void MainWindow::saveWatermelon()
{
    if(melons.size()==0) return;

    for(size_t i=0;i<melons.size();i++)
    {

        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        double width = melons[i].width;
        double height = melons[i].height;
        double weight = melons[i].weight;

        ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString::number(watermelon_no)));
        ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString::number(width,'f',2)));
        ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString::number(height,'f',2)));
        ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString::number(weight,'f',2)));

        QString grade = getGrade(weight);

        ui->tableWidget->setItem(row,4,new QTableWidgetItem(grade));

        weights.push_back(weight);
        grades.push_back(grade);

        if(grade=="A") countA++;
        if(grade=="B") countB++;
        if(grade=="C") countC++;

        watermelon_no++;
    }

    saved = true;
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->tableWidget->setRowCount(0);
}

void MainWindow::on_pushButton_export_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,"Save Report","","CSV Files (*.csv)");

    if(fileName.isEmpty()) return;

    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    int rowCount = ui->tableWidget->rowCount();
    int colCount = ui->tableWidget->columnCount();

    for(int col=0;col<colCount;col++)
    {
        out << ui->tableWidget->horizontalHeaderItem(col)->text();

        if(col < colCount-1)
            out << ",";
    }

    out << "\n";

    for(int row=0;row<rowCount;row++)
    {
        for(int col=0;col<colCount;col++)
        {
            QTableWidgetItem *item = ui->tableWidget->item(row,col);

            if(item)
                out << item->text();

            if(col < colCount-1)
                out << ",";
        }

        out << "\n";
    }

    file.close();

    QMessageBox::information(this,"Success","Export Complete!");
}

void MainWindow::on_pushButton_finish_clicked()
{
    cameraTimer->stop();
    countdownTimer->stop();

    int total = weights.size();

    if(total==0) return;

    double sum = 0;

    for(double w:weights)
        sum += w;

    double avgWeight = sum / total;

    QString text;

    text += "Total : " + QString::number(total) + "\n";
    text += "Average Weight : " + QString::number(avgWeight,'f',2)+" kg\n";
    text += "Grade A : " + QString::number(countA) + "\n";
    text += "Grade B : " + QString::number(countB) + "\n";
    text += "Grade C : " + QString::number(countC) + "\n";

    QMessageBox::information(this,"Dashboard",text);
}

void MainWindow::on_pushButton_continue_clicked()
{
    cameraTimer->start(30);
    countdownTimer->start(1000);
}
