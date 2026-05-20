/********************************************************************************
** Form generated from reading UI file 'keypad.ui'
**
** Created by: Qt User Interface Compiler version 5.15.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KEYPAD_H
#define UI_KEYPAD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_keypad
{
public:
    QPushButton *pushButton_5;
    QPushButton *pushButton_11;
    QPushButton *pushButton_14;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QPushButton *pushButton_4;
    QPushButton *pushButton_6;
    QPushButton *pushButton_13;
    QPushButton *pushButton_10;
    QPushButton *pushButton_8;
    QPushButton *pushButton_12;
    QPushButton *pushButton_15;
    QPushButton *pushButton_16;
    QLabel *label;

    void setupUi(QWidget *keypad)
    {
        if (keypad->objectName().isEmpty())
            keypad->setObjectName(QString::fromUtf8("keypad"));
        keypad->resize(402, 288);
        keypad->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        pushButton_5 = new QPushButton(keypad);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(205, 36, 51, 41));
        pushButton_5->setStyleSheet(QString::fromUtf8("font:25pt;border:none;\n"
"background-color:transparent;"));
        pushButton_11 = new QPushButton(keypad);
        pushButton_11->setObjectName(QString::fromUtf8("pushButton_11"));
        pushButton_11->setGeometry(QRect(290, 45, 81, 31));
        pushButton_11->setStyleSheet(QString::fromUtf8("font:20pt;border:none;\n"
"background-color:transparent;"));
        pushButton_14 = new QPushButton(keypad);
        pushButton_14->setObjectName(QString::fromUtf8("pushButton_14"));
        pushButton_14->setGeometry(QRect(37, 226, 51, 41));
        pushButton_14->setStyleSheet(QString::fromUtf8("font:25pt;border:none;\n"
"background-color:transparent;"));
        pushButton_3 = new QPushButton(keypad);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(205, 163, 51, 41));
        pushButton_3->setStyleSheet(QString::fromUtf8("font:25pt;border:none;\n"
"background-color:transparent;"));
        pushButton_2 = new QPushButton(keypad);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(205, 98, 51, 41));
        pushButton_2->setStyleSheet(QString::fromUtf8("font:25pt;border:none;\n"
"background-color:transparent;"));
        pushButton = new QPushButton(keypad);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(123, 163, 51, 41));
        pushButton->setStyleSheet(QString::fromUtf8("font:25pt;border:none;\n"
"background-color:transparent;"));
        pushButton_4 = new QPushButton(keypad);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(123, 98, 51, 41));
        pushButton_4->setStyleSheet(QString::fromUtf8("font:25pt;border:none;\n"
"background-color:transparent;"));
        pushButton_6 = new QPushButton(keypad);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(37, 98, 51, 41));
        pushButton_6->setStyleSheet(QString::fromUtf8("font:25pt;border:none;\n"
"background-color:transparent;"));
        pushButton_13 = new QPushButton(keypad);
        pushButton_13->setObjectName(QString::fromUtf8("pushButton_13"));
        pushButton_13->setGeometry(QRect(122, 226, 51, 41));
        pushButton_13->setStyleSheet(QString::fromUtf8("font:25pt;border:none;\n"
"background-color:transparent;"));
        pushButton_10 = new QPushButton(keypad);
        pushButton_10->setObjectName(QString::fromUtf8("pushButton_10"));
        pushButton_10->setGeometry(QRect(123, 36, 51, 41));
        pushButton_10->setStyleSheet(QString::fromUtf8("font:25pt;border:none;\n"
"background-color:transparent;"));
        pushButton_8 = new QPushButton(keypad);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(37, 36, 51, 41));
        pushButton_8->setStyleSheet(QString::fromUtf8("font:25pt;border:none;\n"
"background-color:transparent;"));
        pushButton_12 = new QPushButton(keypad);
        pushButton_12->setObjectName(QString::fromUtf8("pushButton_12"));
        pushButton_12->setGeometry(QRect(37, 162, 51, 41));
        pushButton_12->setStyleSheet(QString::fromUtf8("font:25pt;border:none;\n"
"background-color:transparent;"));
        pushButton_15 = new QPushButton(keypad);
        pushButton_15->setObjectName(QString::fromUtf8("pushButton_15"));
        pushButton_15->setGeometry(QRect(290, 112, 81, 31));
        pushButton_15->setStyleSheet(QString::fromUtf8("font:20pt;border:none;\n"
"background-color:transparent;"));
        pushButton_16 = new QPushButton(keypad);
        pushButton_16->setObjectName(QString::fromUtf8("pushButton_16"));
        pushButton_16->setGeometry(QRect(293, 190, 81, 21));
        pushButton_16->setStyleSheet(QString::fromUtf8("font:20pt;border:none;\n"
"background-color:transparent;"));
        label = new QLabel(keypad);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 401, 291));
        label->setStyleSheet(QString::fromUtf8("image: url(:/images/keypad9.png);\n"
"background-color: rgb(15, 186, 205);"));
        label->raise();
        pushButton_11->raise();
        pushButton_8->raise();
        pushButton_10->raise();
        pushButton_12->raise();
        pushButton_3->raise();
        pushButton_5->raise();
        pushButton_14->raise();
        pushButton_15->raise();
        pushButton_2->raise();
        pushButton_13->raise();
        pushButton_4->raise();
        pushButton->raise();
        pushButton_16->raise();
        pushButton_6->raise();

        retranslateUi(keypad);

        QMetaObject::connectSlotsByName(keypad);
    } // setupUi

    void retranslateUi(QWidget *keypad)
    {
        keypad->setWindowTitle(QCoreApplication::translate("keypad", "Form", nullptr));
        pushButton_5->setText(QCoreApplication::translate("keypad", "3", nullptr));
        pushButton_11->setText(QCoreApplication::translate("keypad", "ENTER", nullptr));
        pushButton_14->setText(QCoreApplication::translate("keypad", ".", nullptr));
        pushButton_3->setText(QCoreApplication::translate("keypad", "9", nullptr));
        pushButton_2->setText(QCoreApplication::translate("keypad", "6", nullptr));
        pushButton->setText(QCoreApplication::translate("keypad", "8", nullptr));
        pushButton_4->setText(QCoreApplication::translate("keypad", "5", nullptr));
        pushButton_6->setText(QCoreApplication::translate("keypad", "4", nullptr));
        pushButton_13->setText(QCoreApplication::translate("keypad", "0", nullptr));
        pushButton_10->setText(QCoreApplication::translate("keypad", "2", nullptr));
        pushButton_8->setText(QCoreApplication::translate("keypad", "1", nullptr));
        pushButton_12->setText(QCoreApplication::translate("keypad", "7", nullptr));
        pushButton_15->setText(QCoreApplication::translate("keypad", "CLEAR", nullptr));
        pushButton_16->setText(QCoreApplication::translate("keypad", "BACK", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class keypad: public Ui_keypad {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KEYPAD_H
