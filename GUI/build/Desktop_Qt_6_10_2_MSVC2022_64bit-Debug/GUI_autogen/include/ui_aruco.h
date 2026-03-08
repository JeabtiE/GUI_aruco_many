/********************************************************************************
** Form generated from reading UI file 'aruco.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARUCO_H
#define UI_ARUCO_H

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

class Ui_ArUco
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_camera;
    QLabel *label_text;
    QPushButton *btn_startcalibrate;
    QPushButton *btn_savecalibrate;
    QPushButton *btn_next;
    QPushButton *btn_back;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ArUco)
    {
        if (ArUco->objectName().isEmpty())
            ArUco->setObjectName("ArUco");
        ArUco->resize(670, 608);
        centralwidget = new QWidget(ArUco);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        label_camera = new QLabel(centralwidget);
        label_camera->setObjectName("label_camera");
        label_camera->setMinimumSize(QSize(640, 480));

        verticalLayout->addWidget(label_camera);

        label_text = new QLabel(centralwidget);
        label_text->setObjectName("label_text");

        verticalLayout->addWidget(label_text);

        btn_startcalibrate = new QPushButton(centralwidget);
        btn_startcalibrate->setObjectName("btn_startcalibrate");
        btn_startcalibrate->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(btn_startcalibrate);

        btn_savecalibrate = new QPushButton(centralwidget);
        btn_savecalibrate->setObjectName("btn_savecalibrate");
        btn_savecalibrate->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(btn_savecalibrate);

        btn_next = new QPushButton(centralwidget);
        btn_next->setObjectName("btn_next");
        btn_next->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(btn_next);

        btn_back = new QPushButton(centralwidget);
        btn_back->setObjectName("btn_back");
        btn_back->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(btn_back);

        ArUco->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ArUco);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 670, 25));
        ArUco->setMenuBar(menubar);
        statusbar = new QStatusBar(ArUco);
        statusbar->setObjectName("statusbar");
        ArUco->setStatusBar(statusbar);

        retranslateUi(ArUco);

        QMetaObject::connectSlotsByName(ArUco);
    } // setupUi

    void retranslateUi(QMainWindow *ArUco)
    {
        ArUco->setWindowTitle(QCoreApplication::translate("ArUco", "MainWindow", nullptr));
        label_camera->setText(QCoreApplication::translate("ArUco", "TextLabel", nullptr));
        label_text->setText(QCoreApplication::translate("ArUco", "TextLabel", nullptr));
        btn_startcalibrate->setText(QCoreApplication::translate("ArUco", "Start Calibration", nullptr));
        btn_savecalibrate->setText(QCoreApplication::translate("ArUco", "Save Calibration", nullptr));
        btn_next->setText(QCoreApplication::translate("ArUco", "Next", nullptr));
        btn_back->setText(QCoreApplication::translate("ArUco", "Back", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ArUco: public Ui_ArUco {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARUCO_H
