/********************************************************************************
** Form generated from reading UI file 'footpedal.ui'
**
** Created by: Qt User Interface Compiler version 5.15.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOOTPEDAL_H
#define UI_FOOTPEDAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_footpedal
{
public:
    QComboBox *right_footcom;
    QComboBox *left_footcom;
    QLabel *foot_img;
    QComboBox *bleft_footcom;
    QComboBox *bright_footcom;
    QPushButton *savebut;
    QPushButton *backbut;
    QLabel *label_7;
    QLabel *right_lab;
    QLabel *left_lab;
    QLabel *bleft_lab;
    QLabel *bright_lab;

    void setupUi(QDialog *footpedal)
    {
        if (footpedal->objectName().isEmpty())
            footpedal->setObjectName(QString::fromUtf8("footpedal"));
        footpedal->resize(1280, 720);
        footpedal->setStyleSheet(QString::fromUtf8("background-color: rgb(4, 118, 151);"));
        right_footcom = new QComboBox(footpedal);
        right_footcom->addItem(QString());
        right_footcom->addItem(QString());
        right_footcom->addItem(QString());
        right_footcom->addItem(QString());
        right_footcom->addItem(QString());
        right_footcom->addItem(QString());
        right_footcom->setObjectName(QString::fromUtf8("right_footcom"));
        right_footcom->setGeometry(QRect(430, 210, 281, 101));
        right_footcom->setStyleSheet(QString::fromUtf8("font:20pt \"Ubuntu\";\n"
"color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);\n"
"\n"
"border-radius:20px;\n"
""));
        left_footcom = new QComboBox(footpedal);
        left_footcom->addItem(QString());
        left_footcom->addItem(QString());
        left_footcom->addItem(QString());
        left_footcom->addItem(QString());
        left_footcom->addItem(QString());
        left_footcom->addItem(QString());
        left_footcom->setObjectName(QString::fromUtf8("left_footcom"));
        left_footcom->setGeometry(QRect(430, 100, 281, 101));
        left_footcom->setStyleSheet(QString::fromUtf8("font:20pt \"Ubuntu\";\n"
"color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);\n"
"\n"
"border-radius:20px;\n"
""));
        foot_img = new QLabel(footpedal);
        foot_img->setObjectName(QString::fromUtf8("foot_img"));
        foot_img->setGeometry(QRect(860, 140, 281, 431));
        foot_img->setMaximumSize(QSize(291, 16777215));
        foot_img->setStyleSheet(QString::fromUtf8("image: url(:/images/footimg.png);\n"
"background-color:transparent;"));
        bleft_footcom = new QComboBox(footpedal);
        bleft_footcom->addItem(QString());
        bleft_footcom->addItem(QString());
        bleft_footcom->addItem(QString());
        bleft_footcom->addItem(QString());
        bleft_footcom->addItem(QString());
        bleft_footcom->addItem(QString());
        bleft_footcom->setObjectName(QString::fromUtf8("bleft_footcom"));
        bleft_footcom->setGeometry(QRect(430, 320, 281, 111));
        bleft_footcom->setStyleSheet(QString::fromUtf8("font:20pt \"Ubuntu\";\n"
"color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);\n"
"\n"
"border-radius:20px;\n"
""));
        bright_footcom = new QComboBox(footpedal);
        bright_footcom->addItem(QString());
        bright_footcom->addItem(QString());
        bright_footcom->addItem(QString());
        bright_footcom->addItem(QString());
        bright_footcom->addItem(QString());
        bright_footcom->addItem(QString());
        bright_footcom->setObjectName(QString::fromUtf8("bright_footcom"));
        bright_footcom->setGeometry(QRect(430, 440, 281, 111));
        bright_footcom->setStyleSheet(QString::fromUtf8("font:20pt \"Ubuntu\";\n"
"color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);\n"
"\n"
"border-radius:20px;\n"
""));
        savebut = new QPushButton(footpedal);
        savebut->setObjectName(QString::fromUtf8("savebut"));
        savebut->setGeometry(QRect(250, 590, 541, 61));
        savebut->setStyleSheet(QString::fromUtf8("font: 30pt \"Ubuntu Condensed\";\n"
"background-color: rgb(255, 255, 255);\n"
"border-radius:20px;\n"
"border:3px solid black;"));
        backbut = new QPushButton(footpedal);
        backbut->setObjectName(QString::fromUtf8("backbut"));
        backbut->setGeometry(QRect(40, 20, 111, 91));
        backbut->setStyleSheet(QString::fromUtf8("background-color:transparent;\n"
"image: url(:/images/backbuttonarrow.png);\n"
"border:none;"));
        label_7 = new QLabel(footpedal);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(940, 70, 271, 51));
        label_7->setStyleSheet(QString::fromUtf8("font: 20pt \"Ubuntu Condensed\";\n"
"font:bold;\n"
"background-color:transparent;"));
        right_lab = new QLabel(footpedal);
        right_lab->setObjectName(QString::fromUtf8("right_lab"));
        right_lab->setGeometry(QRect(160, 260, 171, 41));
        right_lab->setStyleSheet(QString::fromUtf8("font: 20pt \"Ubuntu Condensed\";\n"
"font:bold;"));
        left_lab = new QLabel(footpedal);
        left_lab->setObjectName(QString::fromUtf8("left_lab"));
        left_lab->setGeometry(QRect(160, 160, 171, 41));
        left_lab->setStyleSheet(QString::fromUtf8("font: 20pt \"Ubuntu Condensed\";\n"
"font:bold;"));
        bleft_lab = new QLabel(footpedal);
        bleft_lab->setObjectName(QString::fromUtf8("bleft_lab"));
        bleft_lab->setGeometry(QRect(160, 350, 201, 51));
        bleft_lab->setStyleSheet(QString::fromUtf8("font: 20pt \"Ubuntu Condensed\";\n"
"font:bold;"));
        bright_lab = new QLabel(footpedal);
        bright_lab->setObjectName(QString::fromUtf8("bright_lab"));
        bright_lab->setGeometry(QRect(160, 470, 241, 41));
        bright_lab->setStyleSheet(QString::fromUtf8("font: 20pt \"Ubuntu Condensed\";\n"
"font:bold;"));

        retranslateUi(footpedal);

        QMetaObject::connectSlotsByName(footpedal);
    } // setupUi

    void retranslateUi(QDialog *footpedal)
    {
        footpedal->setWindowTitle(QCoreApplication::translate("footpedal", "Dialog", nullptr));
        right_footcom->setItemText(0, QCoreApplication::translate("footpedal", "Reflux", nullptr));
        right_footcom->setItemText(1, QCoreApplication::translate("footpedal", "Decrement", nullptr));
        right_footcom->setItemText(2, QCoreApplication::translate("footpedal", "Increment", nullptr));
        right_footcom->setItemText(3, QCoreApplication::translate("footpedal", "Continuous Irrigation", nullptr));
        right_footcom->setItemText(4, QCoreApplication::translate("footpedal", "Power ON/OFF", nullptr));
        right_footcom->setItemText(5, QCoreApplication::translate("footpedal", "PDM", nullptr));

        left_footcom->setItemText(0, QCoreApplication::translate("footpedal", "Reflux", nullptr));
        left_footcom->setItemText(1, QCoreApplication::translate("footpedal", "Increment", nullptr));
        left_footcom->setItemText(2, QCoreApplication::translate("footpedal", "Decrement", nullptr));
        left_footcom->setItemText(3, QCoreApplication::translate("footpedal", "Continuous Irrigation", nullptr));
        left_footcom->setItemText(4, QCoreApplication::translate("footpedal", "Power ON/OFF", nullptr));
        left_footcom->setItemText(5, QCoreApplication::translate("footpedal", "PDM", nullptr));

        foot_img->setText(QString());
        bleft_footcom->setItemText(0, QCoreApplication::translate("footpedal", "Reflux", nullptr));
        bleft_footcom->setItemText(1, QCoreApplication::translate("footpedal", "Increment", nullptr));
        bleft_footcom->setItemText(2, QCoreApplication::translate("footpedal", "Decrement", nullptr));
        bleft_footcom->setItemText(3, QCoreApplication::translate("footpedal", "Continuous Irrigation", nullptr));
        bleft_footcom->setItemText(4, QCoreApplication::translate("footpedal", "Power ON/OFF", nullptr));
        bleft_footcom->setItemText(5, QCoreApplication::translate("footpedal", "PDM", nullptr));

        bright_footcom->setItemText(0, QCoreApplication::translate("footpedal", "Reflux", nullptr));
        bright_footcom->setItemText(1, QCoreApplication::translate("footpedal", "Increment", nullptr));
        bright_footcom->setItemText(2, QCoreApplication::translate("footpedal", "Decrement", nullptr));
        bright_footcom->setItemText(3, QCoreApplication::translate("footpedal", "Continuous Irrigation", nullptr));
        bright_footcom->setItemText(4, QCoreApplication::translate("footpedal", "Power ON/OFF", nullptr));
        bright_footcom->setItemText(5, QCoreApplication::translate("footpedal", "PDM", nullptr));

        savebut->setText(QCoreApplication::translate("footpedal", "SAVE FOR ALL SURGEONS", nullptr));
        backbut->setText(QString());
        label_7->setText(QCoreApplication::translate("footpedal", "Foot Pedal", nullptr));
        right_lab->setText(QCoreApplication::translate("footpedal", "TOP RIGHT", nullptr));
        left_lab->setText(QCoreApplication::translate("footpedal", "TOP LEFT", nullptr));
        bleft_lab->setText(QCoreApplication::translate("footpedal", "BOTTOM LEFT", nullptr));
        bright_lab->setText(QCoreApplication::translate("footpedal", "BOTTOM RIGHT", nullptr));
    } // retranslateUi

};

namespace Ui {
    class footpedal: public Ui_footpedal {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOOTPEDAL_H
