/********************************************************************************
** Form generated from reading UI file 'footswitch.ui'
**
** Created by: Qt User Interface Compiler version 5.15.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOOTSWITCH_H
#define UI_FOOTSWITCH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_footswitch
{
public:
    QLabel *label;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QPushButton *But_zero;
    QPushButton *But_zerodown;
    QPushButton *But_onedown;
    QPushButton *But_oneup;
    QPushButton *But_twodown;
    QPushButton *But_twoup;
    QPushButton *But_threedown;
    QPushButton *But_threeup;
    QLabel *label_2;
    QPushButton *But_save;
    QPushButton *But_topleft;
    QPushButton *But_topright;
    QPushButton *But_bottomleft;
    QPushButton *But_bottomright;
    QPushButton *But_footswitch;
    QComboBox *LeftFoot_com;
    QComboBox *Right_footcom;
    QComboBox *Bottom_leftcom;
    QComboBox *Bottom_rightcom;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QPushButton *pushButton;
    QPushButton *But_save_2;
    QLabel *label_3;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;

    void setupUi(QDialog *footswitch)
    {
        if (footswitch->objectName().isEmpty())
            footswitch->setObjectName(QString::fromUtf8("footswitch"));
        footswitch->resize(1280, 720);
        footswitch->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(18, 78, 102);\n"
"\n"
"\n"
""));
        label = new QLabel(footswitch);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(-80, 100, 631, 561));
        label->setStyleSheet(QString::fromUtf8("background-color:transparent;\n"
"border:none;\n"
"image: url(:/images/footimges.png);"));
        lineEdit = new QLineEdit(footswitch);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(960, 90, 211, 61));
        lineEdit->setStyleSheet(QString::fromUtf8("font:30pt \"Ubuntu\";\n"
"color: rgb(255, 255, 255);\n"
"border-radius:20px;border:2px solid black;\n"
"background-color: rgb(98, 160, 234);"));
        lineEdit->setAlignment(Qt::AlignCenter);
        lineEdit_2 = new QLineEdit(footswitch);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(960, 230, 211, 61));
        lineEdit_2->setStyleSheet(QString::fromUtf8("font:30pt \"Ubuntu\";\n"
"color: rgb(255, 255, 255);\n"
"background-color:#00B2FF;\n"
"border-radius:20px;border:2px solid black;"));
        lineEdit_2->setAlignment(Qt::AlignCenter);
        lineEdit_3 = new QLineEdit(footswitch);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(960, 360, 211, 61));
        lineEdit_3->setStyleSheet(QString::fromUtf8("font: 30pt \"Ubuntu\";\n"
"color: rgb(255, 255, 255);\n"
"background-color: #007BFF; /* Attractive Blue for second position */\n"
"border-radius: 20px;\n"
"border: 2px solid black;\n"
""));
        lineEdit_3->setAlignment(Qt::AlignCenter);
        lineEdit_4 = new QLineEdit(footswitch);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(960, 500, 211, 61));
        lineEdit_4->setStyleSheet(QString::fromUtf8("\n"
"font: 30pt \"Ubuntu\";\n"
"color: rgb(255, 255, 255);\n"
"background-color: #0056b3; /* Darker Blue */\n"
"border-radius: 20px;\n"
"border: 2px solid black;"));
        lineEdit_4->setAlignment(Qt::AlignCenter);
        But_zero = new QPushButton(footswitch);
        But_zero->setObjectName(QString::fromUtf8("But_zero"));
        But_zero->setGeometry(QRect(860, 80, 81, 81));
        But_zero->setStyleSheet(QString::fromUtf8("border:none;\n"
"background-color:transparent;\n"
"image: url(:/images/uarrow.png);"));
        But_zerodown = new QPushButton(footswitch);
        But_zerodown->setObjectName(QString::fromUtf8("But_zerodown"));
        But_zerodown->setGeometry(QRect(1190, 80, 81, 81));
        But_zerodown->setStyleSheet(QString::fromUtf8("border:none;\n"
"background-color:transparent;\n"
"image: url(:/images/darrow.png);"));
        But_onedown = new QPushButton(footswitch);
        But_onedown->setObjectName(QString::fromUtf8("But_onedown"));
        But_onedown->setGeometry(QRect(1190, 220, 81, 81));
        But_onedown->setStyleSheet(QString::fromUtf8("border:none;\n"
"background-color:transparent;\n"
"image: url(:/images/darrow.png);"));
        But_oneup = new QPushButton(footswitch);
        But_oneup->setObjectName(QString::fromUtf8("But_oneup"));
        But_oneup->setGeometry(QRect(860, 220, 81, 81));
        But_oneup->setStyleSheet(QString::fromUtf8("border:none;\n"
"background-color:transparent;\n"
"image: url(:/images/uarrow.png);"));
        But_twodown = new QPushButton(footswitch);
        But_twodown->setObjectName(QString::fromUtf8("But_twodown"));
        But_twodown->setGeometry(QRect(1190, 350, 81, 81));
        But_twodown->setStyleSheet(QString::fromUtf8("border:none;\n"
"background-color:transparent;\n"
"image: url(:/images/darrow.png);"));
        But_twoup = new QPushButton(footswitch);
        But_twoup->setObjectName(QString::fromUtf8("But_twoup"));
        But_twoup->setGeometry(QRect(860, 350, 81, 81));
        But_twoup->setStyleSheet(QString::fromUtf8("border:none;\n"
"background-color:transparent;\n"
"image: url(:/images/uarrow.png);"));
        But_threedown = new QPushButton(footswitch);
        But_threedown->setObjectName(QString::fromUtf8("But_threedown"));
        But_threedown->setGeometry(QRect(1190, 490, 81, 81));
        But_threedown->setStyleSheet(QString::fromUtf8("border:none;\n"
"background-color:transparent;\n"
"image: url(:/images/darrow.png);"));
        But_threeup = new QPushButton(footswitch);
        But_threeup->setObjectName(QString::fromUtf8("But_threeup"));
        But_threeup->setGeometry(QRect(860, 490, 81, 81));
        But_threeup->setStyleSheet(QString::fromUtf8("border:none;\n"
"background-color:transparent;\n"
"image: url(:/images/uarrow.png);"));
        label_2 = new QLabel(footswitch);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(780, 440, 71, 61));
        label_2->setStyleSheet(QString::fromUtf8("image: url(:/images/circle.png);\n"
"background-color:transparent;\n"
"border:none;"));
        But_save = new QPushButton(footswitch);
        But_save->setObjectName(QString::fromUtf8("But_save"));
        But_save->setGeometry(QRect(1010, 630, 231, 71));
        But_save->setStyleSheet(QString::fromUtf8("font:15pt;\n"
"color: rgb(255, 255, 255);\n"
"font:bold;\n"
"border:3px solid white;\n"
"background-color: rgb(12, 40, 82);"));
        But_topleft = new QPushButton(footswitch);
        But_topleft->setObjectName(QString::fromUtf8("But_topleft"));
        But_topleft->setGeometry(QRect(270, 250, 201, 61));
        But_topleft->setStyleSheet(QString::fromUtf8("QPushButton { text-align: center; font-size: 9pt; \n"
"background-color: rgb(0, 0, 0);\n"
"color: rgb(255, 255, 255);\n"
"border-radius:20px;\n"
"border:2px solid skyblue;\n"
"}"));
        But_topright = new QPushButton(footswitch);
        But_topright->setObjectName(QString::fromUtf8("But_topright"));
        But_topright->setGeometry(QRect(270, 460, 201, 61));
        But_topright->setStyleSheet(QString::fromUtf8("QPushButton { text-align:center; font-size: 9pt; \n"
"background-color: rgb(0, 0, 0);\n"
"color: rgb(255, 255, 255);\n"
"border-radius:20px;\n"
"border:2px solid skyblue;\n"
"}"));
        But_bottomleft = new QPushButton(footswitch);
        But_bottomleft->setObjectName(QString::fromUtf8("But_bottomleft"));
        But_bottomleft->setGeometry(QRect(60, 252, 201, 61));
        But_bottomleft->setStyleSheet(QString::fromUtf8("QPushButton { text-align:center; font-size: 9pt; \n"
"background-color: rgb(0, 0, 0);\n"
"color: rgb(255, 255, 255);\n"
"border-radius:20px;\n"
"border:2px solid skyblue;\n"
"}"));
        But_bottomright = new QPushButton(footswitch);
        But_bottomright->setObjectName(QString::fromUtf8("But_bottomright"));
        But_bottomright->setGeometry(QRect(60, 460, 201, 61));
        But_bottomright->setStyleSheet(QString::fromUtf8("QPushButton { text-align:center; font-size: 9pt; \n"
"background-color: rgb(0, 0, 0);\n"
"color: rgb(255, 255, 255);\n"
"border-radius:20px;\n"
"border:2px solid skyblue;\n"
"}"));
        But_footswitch = new QPushButton(footswitch);
        But_footswitch->setObjectName(QString::fromUtf8("But_footswitch"));
        But_footswitch->setGeometry(QRect(110, 296, 361, 181));
        But_footswitch->setStyleSheet(QString::fromUtf8("border:none;\n"
"background-color: transparent;\n"
"image: url(:/images/g2332.png);"));
        LeftFoot_com = new QComboBox(footswitch);
        LeftFoot_com->addItem(QString());
        LeftFoot_com->addItem(QString());
        LeftFoot_com->addItem(QString());
        LeftFoot_com->addItem(QString());
        LeftFoot_com->addItem(QString());
        LeftFoot_com->addItem(QString());
        LeftFoot_com->setObjectName(QString::fromUtf8("LeftFoot_com"));
        LeftFoot_com->setGeometry(QRect(320, 130, 261, 81));
        LeftFoot_com->setStyleSheet(QString::fromUtf8("border-radius:20px;\n"
"color: rgb(255, 255, 255);\n"
"font:20pt \"Ubuntu\";\n"
"background-color: rgb(0, 0, 0);"));
        Right_footcom = new QComboBox(footswitch);
        Right_footcom->addItem(QString());
        Right_footcom->addItem(QString());
        Right_footcom->addItem(QString());
        Right_footcom->addItem(QString());
        Right_footcom->addItem(QString());
        Right_footcom->addItem(QString());
        Right_footcom->setObjectName(QString::fromUtf8("Right_footcom"));
        Right_footcom->setGeometry(QRect(340, 540, 261, 81));
        Right_footcom->setStyleSheet(QString::fromUtf8("border-radius:20px;\n"
"color: rgb(255, 255, 255);\n"
"font:20pt \"Ubuntu\";\n"
"background-color: rgb(0, 0, 0);"));
        Bottom_leftcom = new QComboBox(footswitch);
        Bottom_leftcom->addItem(QString());
        Bottom_leftcom->addItem(QString());
        Bottom_leftcom->addItem(QString());
        Bottom_leftcom->addItem(QString());
        Bottom_leftcom->addItem(QString());
        Bottom_leftcom->addItem(QString());
        Bottom_leftcom->setObjectName(QString::fromUtf8("Bottom_leftcom"));
        Bottom_leftcom->setGeometry(QRect(20, 130, 261, 81));
        Bottom_leftcom->setStyleSheet(QString::fromUtf8("border-radius:20px;\n"
"color: rgb(255, 255, 255);\n"
"font:20pt \"Ubuntu\";\n"
"background-color: rgb(0, 0, 0);"));
        Bottom_rightcom = new QComboBox(footswitch);
        Bottom_rightcom->addItem(QString());
        Bottom_rightcom->addItem(QString());
        Bottom_rightcom->addItem(QString());
        Bottom_rightcom->addItem(QString());
        Bottom_rightcom->addItem(QString());
        Bottom_rightcom->addItem(QString());
        Bottom_rightcom->setObjectName(QString::fromUtf8("Bottom_rightcom"));
        Bottom_rightcom->setGeometry(QRect(30, 550, 271, 81));
        Bottom_rightcom->setStyleSheet(QString::fromUtf8("border-radius:20px;\n"
"color: rgb(255, 255, 255);\n"
"font:20pt \"Ubuntu\";\n"
"background-color: rgb(0, 0, 0);"));
        label_4 = new QLabel(footswitch);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(950, 40, 241, 41));
        label_4->setStyleSheet(QString::fromUtf8("font:30px;background-color:transparent;\n"
"color:white;\n"
"border-radius:10px;"));
        label_4->setAlignment(Qt::AlignCenter);
        label_5 = new QLabel(footswitch);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(950, 170, 241, 41));
        label_5->setStyleSheet(QString::fromUtf8("font:30px;background-color:transparent;\n"
"color:white;\n"
"border-radius:10px;"));
        label_5->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(footswitch);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(950, 310, 241, 41));
        label_6->setStyleSheet(QString::fromUtf8("font:30px;background-color:transparent;\n"
"color:white;\n"
"border-radius:10px;"));
        label_6->setAlignment(Qt::AlignCenter);
        label_7 = new QLabel(footswitch);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(950, 440, 241, 41));
        label_7->setStyleSheet(QString::fromUtf8("font:30px;background-color:transparent;\n"
"color:white;\n"
"border-radius:10px;"));
        label_7->setAlignment(Qt::AlignCenter);
        pushButton = new QPushButton(footswitch);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(0, 10, 141, 101));
        pushButton->setStyleSheet(QString::fromUtf8("border:none;\n"
"image: url(:/images/settings.png);\n"
"background-color:transparent;"));
        But_save_2 = new QPushButton(footswitch);
        But_save_2->setObjectName(QString::fromUtf8("But_save_2"));
        But_save_2->setGeometry(QRect(740, 630, 231, 71));
        But_save_2->setStyleSheet(QString::fromUtf8("font:15pt;\n"
"color: rgb(255, 255, 255);\n"
"font:bold;\n"
"border:3px solid white;\n"
"background-color: rgb(12, 40, 82);"));
        label_3 = new QLabel(footswitch);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(380, 301, 21, 41));
        label_3->setStyleSheet(QString::fromUtf8("image: url(:/images/uparrow.png);\n"
"border:none;\n"
"background-color:transparent;"));
        label_8 = new QLabel(footswitch);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(220, 302, 31, 51));
        label_8->setStyleSheet(QString::fromUtf8("image: url(:/images/uparrow.png);\n"
"border:none;\n"
"background-color:transparent;"));
        label_9 = new QLabel(footswitch);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(370, 430, 21, 41));
        label_9->setStyleSheet(QString::fromUtf8("border:none;\n"
"image: url(:/images/downarrow.png);\n"
"background-color:transparent;"));
        label_10 = new QLabel(footswitch);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(220, 420, 31, 51));
        label_10->setStyleSheet(QString::fromUtf8("border:none;\n"
"image: url(:/images/downarrow.png);\n"
"background-color:transparent;"));
        label_2->raise();
        label->raise();
        lineEdit->raise();
        lineEdit_2->raise();
        lineEdit_3->raise();
        lineEdit_4->raise();
        But_zero->raise();
        But_zerodown->raise();
        But_onedown->raise();
        But_oneup->raise();
        But_twodown->raise();
        But_twoup->raise();
        But_threedown->raise();
        But_threeup->raise();
        But_save->raise();
        But_topleft->raise();
        But_topright->raise();
        But_bottomleft->raise();
        But_bottomright->raise();
        But_footswitch->raise();
        LeftFoot_com->raise();
        Right_footcom->raise();
        Bottom_leftcom->raise();
        Bottom_rightcom->raise();
        label_4->raise();
        label_5->raise();
        label_6->raise();
        label_7->raise();
        pushButton->raise();
        But_save_2->raise();
        label_3->raise();
        label_8->raise();
        label_9->raise();
        label_10->raise();

        retranslateUi(footswitch);

        QMetaObject::connectSlotsByName(footswitch);
    } // setupUi

    void retranslateUi(QDialog *footswitch)
    {
        footswitch->setWindowTitle(QCoreApplication::translate("footswitch", "Dialog", nullptr));
        label->setText(QString());
        But_zero->setText(QString());
        But_zerodown->setText(QString());
        But_onedown->setText(QString());
        But_oneup->setText(QString());
        But_twodown->setText(QString());
        But_twoup->setText(QString());
        But_threedown->setText(QString());
        But_threeup->setText(QString());
        label_2->setText(QString());
        But_save->setText(QCoreApplication::translate("footswitch", "Save", nullptr));
        But_topleft->setText(QCoreApplication::translate("footswitch", "Top Left", nullptr));
        But_topright->setText(QCoreApplication::translate("footswitch", "Top Right", nullptr));
        But_bottomleft->setText(QCoreApplication::translate("footswitch", "Bottom left", nullptr));
        But_bottomright->setText(QCoreApplication::translate("footswitch", "Bottom Right", nullptr));
        But_footswitch->setText(QString());
        LeftFoot_com->setItemText(0, QCoreApplication::translate("footswitch", "Increment", nullptr));
        LeftFoot_com->setItemText(1, QCoreApplication::translate("footswitch", "Decrement", nullptr));
        LeftFoot_com->setItemText(2, QCoreApplication::translate("footswitch", "Reflux", nullptr));
        LeftFoot_com->setItemText(3, QCoreApplication::translate("footswitch", "Continuous Irrigation", nullptr));
        LeftFoot_com->setItemText(4, QCoreApplication::translate("footswitch", "Power ON/OFF", nullptr));
        LeftFoot_com->setItemText(5, QCoreApplication::translate("footswitch", "PDM", nullptr));

        Right_footcom->setItemText(0, QCoreApplication::translate("footswitch", "Increment", nullptr));
        Right_footcom->setItemText(1, QCoreApplication::translate("footswitch", "Decrement", nullptr));
        Right_footcom->setItemText(2, QCoreApplication::translate("footswitch", "Reflux", nullptr));
        Right_footcom->setItemText(3, QCoreApplication::translate("footswitch", "Continuous Irrigation", nullptr));
        Right_footcom->setItemText(4, QCoreApplication::translate("footswitch", "Power ON/OFF", nullptr));
        Right_footcom->setItemText(5, QCoreApplication::translate("footswitch", "PDM", nullptr));

        Bottom_leftcom->setItemText(0, QCoreApplication::translate("footswitch", "Increment", nullptr));
        Bottom_leftcom->setItemText(1, QCoreApplication::translate("footswitch", "Decrement", nullptr));
        Bottom_leftcom->setItemText(2, QCoreApplication::translate("footswitch", "Reflux", nullptr));
        Bottom_leftcom->setItemText(3, QCoreApplication::translate("footswitch", "Continuous Irrigation", nullptr));
        Bottom_leftcom->setItemText(4, QCoreApplication::translate("footswitch", "Power ON/OFF", nullptr));
        Bottom_leftcom->setItemText(5, QCoreApplication::translate("footswitch", "PDM", nullptr));

        Bottom_rightcom->setItemText(0, QCoreApplication::translate("footswitch", "Increment", nullptr));
        Bottom_rightcom->setItemText(1, QCoreApplication::translate("footswitch", "Decrement", nullptr));
        Bottom_rightcom->setItemText(2, QCoreApplication::translate("footswitch", "Reflux", nullptr));
        Bottom_rightcom->setItemText(3, QCoreApplication::translate("footswitch", "Continuous Irrigation", nullptr));
        Bottom_rightcom->setItemText(4, QCoreApplication::translate("footswitch", "Power ON/OFF", nullptr));
        Bottom_rightcom->setItemText(5, QCoreApplication::translate("footswitch", "PDM", nullptr));

        label_4->setText(QCoreApplication::translate("footswitch", "0 Position", nullptr));
        label_5->setText(QCoreApplication::translate("footswitch", "1 Position", nullptr));
        label_6->setText(QCoreApplication::translate("footswitch", "2 Position", nullptr));
        label_7->setText(QCoreApplication::translate("footswitch", "3 Position", nullptr));
        pushButton->setText(QString());
        But_save_2->setText(QCoreApplication::translate("footswitch", "Load Default", nullptr));
        label_3->setText(QString());
        label_8->setText(QString());
        label_9->setText(QString());
        label_10->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class footswitch: public Ui_footswitch {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOOTSWITCH_H
