/********************************************************************************
** Form generated from reading UI file 'settings.ui'
**
** Created by: Qt User Interface Compiler version 5.15.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_settings
{
public:
    QPushButton *display_but;
    QLabel *general_lab;
    QPushButton *surgeon_but;
    QPushButton *general_but;
    QLabel *footpedal_lab;
    QLabel *display_lab;
    QLabel *learner_lab;
    QPushButton *back_but;
    QLabel *surgeon_lab;
    QPushButton *footpedal_but;
    QPushButton *learner_but;

    void setupUi(QDialog *settings)
    {
        if (settings->objectName().isEmpty())
            settings->setObjectName(QString::fromUtf8("settings"));
        settings->resize(1280, 720);
        settings->setStyleSheet(QString::fromUtf8("background-color: rgb(162, 128, 137);"));
        display_but = new QPushButton(settings);
        display_but->setObjectName(QString::fromUtf8("display_but"));
        display_but->setGeometry(QRect(890, 140, 131, 101));
        display_but->setStyleSheet(QString::fromUtf8("image: url(:/images/display.png);\n"
"border:none;"));
        general_lab = new QLabel(settings);
        general_lab->setObjectName(QString::fromUtf8("general_lab"));
        general_lab->setGeometry(QRect(160, 230, 161, 51));
        general_lab->setStyleSheet(QString::fromUtf8("background-color:transparent;font: 30pt \"Ubuntu Condensed\";\n"
"font:bold;\n"
"border:5px solid black;\n"
"border-radius:20px;"));
        general_lab->setAlignment(Qt::AlignCenter);
        surgeon_but = new QPushButton(settings);
        surgeon_but->setObjectName(QString::fromUtf8("surgeon_but"));
        surgeon_but->setGeometry(QRect(120, 450, 181, 171));
        surgeon_but->setStyleSheet(QString::fromUtf8("image: url(:/images/doctor.png);\n"
"border-radius:20px;\n"
"background-color:transparent;"));
        general_but = new QPushButton(settings);
        general_but->setObjectName(QString::fromUtf8("general_but"));
        general_but->setGeometry(QRect(170, 110, 140, 131));
        general_but->setStyleSheet(QString::fromUtf8("image: url(:/images/genral1.png);\n"
"border:none;\n"
"background-color:transparent;"));
        footpedal_lab = new QLabel(settings);
        footpedal_lab->setObjectName(QString::fromUtf8("footpedal_lab"));
        footpedal_lab->setGeometry(QRect(810, 610, 261, 61));
        footpedal_lab->setStyleSheet(QString::fromUtf8("background-color:transparent;font: 30pt \"Ubuntu Condensed\";\n"
"font:bold;\n"
"border:5px solid black;\n"
"border-radius:20px;"));
        footpedal_lab->setAlignment(Qt::AlignCenter);
        display_lab = new QLabel(settings);
        display_lab->setObjectName(QString::fromUtf8("display_lab"));
        display_lab->setGeometry(QRect(870, 230, 181, 51));
        display_lab->setStyleSheet(QString::fromUtf8("background-color:transparent;font: 30pt \"Ubuntu Condensed\";\n"
"font:bold;\n"
"border:5px solid black;\n"
"border-radius:20px;"));
        display_lab->setAlignment(Qt::AlignCenter);
        learner_lab = new QLabel(settings);
        learner_lab->setObjectName(QString::fromUtf8("learner_lab"));
        learner_lab->setGeometry(QRect(460, 410, 191, 51));
        learner_lab->setStyleSheet(QString::fromUtf8("background-color:transparent;font: 30pt \"Ubuntu Condensed\";\n"
"font:bold;\n"
"border:5px solid black;\n"
"border-radius:20px;"));
        learner_lab->setAlignment(Qt::AlignCenter);
        back_but = new QPushButton(settings);
        back_but->setObjectName(QString::fromUtf8("back_but"));
        back_but->setGeometry(QRect(30, 10, 121, 121));
        back_but->setStyleSheet(QString::fromUtf8("image: url(:/images/backarrowimg.png);\n"
"background-color:transparent;\n"
"border:none;"));
        surgeon_lab = new QLabel(settings);
        surgeon_lab->setObjectName(QString::fromUtf8("surgeon_lab"));
        surgeon_lab->setGeometry(QRect(110, 610, 201, 51));
        surgeon_lab->setStyleSheet(QString::fromUtf8("background-color:transparent;font: 30pt \"Ubuntu Condensed\";\n"
"font:bold;\n"
"border:5px solid black;\n"
"border-radius:20px;"));
        surgeon_lab->setAlignment(Qt::AlignCenter);
        footpedal_but = new QPushButton(settings);
        footpedal_but->setObjectName(QString::fromUtf8("footpedal_but"));
        footpedal_but->setGeometry(QRect(820, 430, 211, 211));
        footpedal_but->setStyleSheet(QString::fromUtf8("border-radius:20px;\n"
"background-color:transparent;\n"
"image: url(:/images/footimg.png);"));
        learner_but = new QPushButton(settings);
        learner_but->setObjectName(QString::fromUtf8("learner_but"));
        learner_but->setGeometry(QRect(480, 300, 140, 131));
        learner_but->setStyleSheet(QString::fromUtf8("image: url(:/images/learner.png);\n"
"background-color:transparent;\n"
"border:none;"));

        retranslateUi(settings);

        QMetaObject::connectSlotsByName(settings);
    } // setupUi

    void retranslateUi(QDialog *settings)
    {
        settings->setWindowTitle(QCoreApplication::translate("settings", "Dialog", nullptr));
        display_but->setText(QString());
        general_lab->setText(QCoreApplication::translate("settings", "General", nullptr));
        surgeon_but->setText(QString());
        general_but->setText(QCoreApplication::translate("settings", "general", nullptr));
        footpedal_lab->setText(QCoreApplication::translate("settings", "FOOT PEDAL", nullptr));
        display_lab->setText(QCoreApplication::translate("settings", "Display", nullptr));
        learner_lab->setText(QCoreApplication::translate("settings", "Learner", nullptr));
        back_but->setText(QString());
        surgeon_lab->setText(QCoreApplication::translate("settings", "SURGEON", nullptr));
        footpedal_but->setText(QString());
        learner_but->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class settings: public Ui_settings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
