/********************************************************************************
** Form generated from reading UI file 'calibration_select.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALIBRATION_SELECT_H
#define UI_CALIBRATION_SELECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Calibeation_Select
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_card;
    QPushButton *btn_aruco;

    void setupUi(QDialog *Calibeation_Select)
    {
        if (Calibeation_Select->objectName().isEmpty())
            Calibeation_Select->setObjectName("Calibeation_Select");
        Calibeation_Select->resize(626, 643);
        horizontalLayout = new QHBoxLayout(Calibeation_Select);
        horizontalLayout->setObjectName("horizontalLayout");
        btn_card = new QPushButton(Calibeation_Select);
        btn_card->setObjectName("btn_card");
        btn_card->setMinimumSize(QSize(0, 40));

        horizontalLayout->addWidget(btn_card);

        btn_aruco = new QPushButton(Calibeation_Select);
        btn_aruco->setObjectName("btn_aruco");
        btn_aruco->setMinimumSize(QSize(0, 40));

        horizontalLayout->addWidget(btn_aruco);


        retranslateUi(Calibeation_Select);

        QMetaObject::connectSlotsByName(Calibeation_Select);
    } // setupUi

    void retranslateUi(QDialog *Calibeation_Select)
    {
        Calibeation_Select->setWindowTitle(QCoreApplication::translate("Calibeation_Select", "Dialog", nullptr));
        btn_card->setText(QCoreApplication::translate("Calibeation_Select", "Calibrate with card", nullptr));
        btn_aruco->setText(QCoreApplication::translate("Calibeation_Select", "Calibrate with ArUco", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Calibeation_Select: public Ui_Calibeation_Select {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBRATION_SELECT_H
