/********************************************************************************
** Form generated from reading UI file 'tuning.ui'
**
** Created by: Qt User Interface Compiler version 5.15.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TUNING_H
#define UI_TUNING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_tuning
{
public:
    QPushButton *But_Tune;
    QPushButton *But_Next;
    QPushButton *But_Handpiece;
    QPushButton *But_value;
    QPushButton *ButRTune;
    QLabel *lblRTune;
    QLabel *lblTuned;
    QPushButton *butTuned;
    QLabel *lblinfo;

    void setupUi(QDialog *tuning)
    {
        if (tuning->objectName().isEmpty())
            tuning->setObjectName(QString::fromUtf8("tuning"));
        tuning->resize(861, 711);
        But_Tune = new QPushButton(tuning);
        But_Tune->setObjectName(QString::fromUtf8("But_Tune"));
        But_Tune->setGeometry(QRect(170, 310, 561, 141));
        But_Tune->setStyleSheet(QString::fromUtf8("QPushButton{background-color:transparent;\n"
"image: url(:/images/handpiece1.png);\n"
"border:none;}\n"
"QPushButton:focus {\n"
"    outline: none;\n"
"    border: none;\n"
"}"));
        But_Next = new QPushButton(tuning);
        But_Next->setObjectName(QString::fromUtf8("But_Next"));
        But_Next->setGeometry(QRect(690, 570, 151, 121));
        But_Next->setStyleSheet(QString::fromUtf8("background-color:transparent;\n"
"image: url(:/images/nextbutton.png);\n"
"border:none;"));
        But_Handpiece = new QPushButton(tuning);
        But_Handpiece->setObjectName(QString::fromUtf8("But_Handpiece"));
        But_Handpiece->setGeometry(QRect(150, 370, 141, 131));
        But_Handpiece->setStyleSheet(QString::fromUtf8("background-color:transparent;\n"
"border:none;"));
        But_value = new QPushButton(tuning);
        But_value->setObjectName(QString::fromUtf8("But_value"));
        But_value->setGeometry(QRect(370, 300, 131, 141));
        But_value->setStyleSheet(QString::fromUtf8(""));
        ButRTune = new QPushButton(tuning);
        ButRTune->setObjectName(QString::fromUtf8("ButRTune"));
        ButRTune->setGeometry(QRect(160, 620, 521, 41));
        ButRTune->setStyleSheet(QString::fromUtf8("background-color: rgb(12, 40, 82);\n"
"color:rgb(255,255,255);\n"
"font:20pt;\n"
"font:bold;\n"
""));
        lblRTune = new QLabel(tuning);
        lblRTune->setObjectName(QString::fromUtf8("lblRTune"));
        lblRTune->setGeometry(QRect(150, 620, 71, 41));
        lblRTune->setStyleSheet(QString::fromUtf8("image: url(:/images/singletick.png);\n"
"background-color:transparent;\n"
"border:none;"));
        lblTuned = new QLabel(tuning);
        lblTuned->setObjectName(QString::fromUtf8("lblTuned"));
        lblTuned->setGeometry(QRect(150, 670, 71, 41));
        lblTuned->setStyleSheet(QString::fromUtf8("background-color:transparent;\n"
"image: url(:/images/doubled.png);\n"
"border:none;"));
        butTuned = new QPushButton(tuning);
        butTuned->setObjectName(QString::fromUtf8("butTuned"));
        butTuned->setGeometry(QRect(160, 670, 521, 41));
        butTuned->setStyleSheet(QString::fromUtf8("background-color: rgb(12, 40, 82);\n"
"color:rgb(255,255,255);\n"
"font:20pt;\n"
"font:bold;\n"
""));
        lblinfo = new QLabel(tuning);
        lblinfo->setObjectName(QString::fromUtf8("lblinfo"));
        lblinfo->setGeometry(QRect(380, 110, 471, 31));
        lblinfo->setStyleSheet(QString::fromUtf8("background-color: rgb(12, 40, 82);\n"
"color:rgb(255,255,255);\n"
"font:20pt;\n"
"font:bold;\n"
""));
        lblinfo->setAlignment(Qt::AlignCenter);
        lblinfo->raise();
        But_Tune->raise();
        But_Next->raise();
        But_value->raise();
        But_Handpiece->raise();
        ButRTune->raise();
        lblRTune->raise();
        butTuned->raise();
        lblTuned->raise();

        retranslateUi(tuning);

        QMetaObject::connectSlotsByName(tuning);
    } // setupUi

    void retranslateUi(QDialog *tuning)
    {
        tuning->setWindowTitle(QCoreApplication::translate("tuning", "Dialog", nullptr));
        But_Tune->setText(QString());
        But_Next->setText(QString());
        But_Handpiece->setText(QString());
        But_value->setText(QCoreApplication::translate("tuning", "PushButton", nullptr));
        ButRTune->setText(QCoreApplication::translate("tuning", "Ready For Tune", nullptr));
        lblRTune->setText(QString());
        lblTuned->setText(QString());
        butTuned->setText(QCoreApplication::translate("tuning", " Tuned", nullptr));
        lblinfo->setText(QCoreApplication::translate("tuning", "Tune Status", nullptr));
    } // retranslateUi

};

namespace Ui {
    class tuning: public Ui_tuning {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TUNING_H
