/********************************************************************************
** Form generated from reading UI file 'calibrationwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALIBRATIONWINDOW_H
#define UI_CALIBRATIONWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CalibrationWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_camera;
    QLabel *label_scale;
    QPushButton *btn_capture;
    QPushButton *btn_continue;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CalibrationWindow)
    {
        if (CalibrationWindow->objectName().isEmpty())
            CalibrationWindow->setObjectName("CalibrationWindow");
        CalibrationWindow->resize(622, 602);
        centralwidget = new QWidget(CalibrationWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        label_camera = new QLabel(centralwidget);
        label_camera->setObjectName("label_camera");
        label_camera->setMinimumSize(QSize(640, 400));

        verticalLayout->addWidget(label_camera);

        label_scale = new QLabel(centralwidget);
        label_scale->setObjectName("label_scale");
        label_scale->setMinimumSize(QSize(0, 0));

        verticalLayout->addWidget(label_scale);

        btn_capture = new QPushButton(centralwidget);
        btn_capture->setObjectName("btn_capture");
        btn_capture->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(btn_capture);

        btn_continue = new QPushButton(centralwidget);
        btn_continue->setObjectName("btn_continue");
        btn_continue->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(btn_continue);

        CalibrationWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CalibrationWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 622, 25));
        CalibrationWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(CalibrationWindow);
        statusbar->setObjectName("statusbar");
        CalibrationWindow->setStatusBar(statusbar);

        retranslateUi(CalibrationWindow);

        QMetaObject::connectSlotsByName(CalibrationWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CalibrationWindow)
    {
        CalibrationWindow->setWindowTitle(QCoreApplication::translate("CalibrationWindow", "MainWindow", nullptr));
        label_camera->setText(QString());
        label_scale->setText(QString());
        btn_capture->setText(QCoreApplication::translate("CalibrationWindow", "Calibrate", nullptr));
        btn_continue->setText(QCoreApplication::translate("CalibrationWindow", "Continue", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CalibrationWindow: public Ui_CalibrationWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBRATIONWINDOW_H
