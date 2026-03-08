#ifndef CALIBRATIONWINDOW_H
#define CALIBRATIONWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QTimer>

namespace Ui {
class CalibrationWindow;
}

class CalibrationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CalibrationWindow(QWidget *parent = nullptr);
    ~CalibrationWindow();

private slots:
    void updateCamera();
    void on_btn_capture_clicked();
    void on_btn_continue_clicked();

private:
    Ui::CalibrationWindow *ui;

    cv::VideoCapture cap;
    QTimer *timer;

    cv::Mat currentFrame;

    bool calibrated = false;
    double pixel_per_cm = 0;

    cv::Mat homography;
    bool homography_ready = false;
};

#endif // CALIBRATIONWINDOW_H
