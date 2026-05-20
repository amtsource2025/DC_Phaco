/********************************************************************************
** Form generated from reading UI file 'general.ui'
**
** Created by: Qt User Interface Compiler version 5.15.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GENERAL_H
#define UI_GENERAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_general
{
public:
    QPushButton *networkingbut;
    QPushButton *about_but;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *seria_lab;
    QLabel *software_lab;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QWidget *tab_2;
    QTextEdit *textEdit;
    QPushButton *clearallbut;
    QProgressBar *progressBar;
    QLabel *label;
    QWidget *tab_3;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *updatedate_but;
    QLabel *label_8;
    QLabel *label_9;
    QWidget *tab_4;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QCheckBox *checkBox;
    QPushButton *updatenetbut;
    QCheckBox *checkBox_2;
    QPushButton *clockbut;
    QLabel *label_7;
    QPushButton *storagebut;
    QPushButton *backbut;

    void setupUi(QDialog *general)
    {
        if (general->objectName().isEmpty())
            general->setObjectName(QString::fromUtf8("general"));
        general->resize(1280, 720);
        general->setStyleSheet(QString::fromUtf8("background-color: rgb(162, 128, 137);"));
        networkingbut = new QPushButton(general);
        networkingbut->setObjectName(QString::fromUtf8("networkingbut"));
        networkingbut->setGeometry(QRect(130, 550, 171, 91));
        networkingbut->setStyleSheet(QString::fromUtf8("font:15pt \"Ubuntu\";\n"
"background-color:rgb(255,255,255);\n"
"border-radius:20px;\n"
"border:5px solid black;"));
        about_but = new QPushButton(general);
        about_but->setObjectName(QString::fromUtf8("about_but"));
        about_but->setGeometry(QRect(130, 130, 181, 91));
        about_but->setStyleSheet(QString::fromUtf8("font:15pt \"Ubuntu\";\n"
"background-color:rgb(255,255,255);\n"
"border-radius:20px;\n"
"border:none;\n"
"border:5px solid black;"));
        tabWidget = new QTabWidget(general);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(430, 10, 801, 711));
        tabWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(162, 128, 137);"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        seria_lab = new QLabel(tab);
        seria_lab->setObjectName(QString::fromUtf8("seria_lab"));
        seria_lab->setGeometry(QRect(80, 160, 161, 71));
        seria_lab->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 15pt \"Ubuntu Condensed\";\n"
"font:bold;\n"
"border:2px solid black;\n"
"border-radius:20px;"));
        software_lab = new QLabel(tab);
        software_lab->setObjectName(QString::fromUtf8("software_lab"));
        software_lab->setGeometry(QRect(80, 350, 171, 61));
        software_lab->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 15pt \"Ubuntu Condensed\";\n"
"font:bold;\n"
"border:2px solid black;\n"
"border-radius:20px;"));
        lineEdit = new QLineEdit(tab);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(320, 160, 431, 71));
        lineEdit->setStyleSheet(QString::fromUtf8("font:20pt \"Ubuntu\";\n"
"background-color: rgb(255,255,255);\n"
"border-radius:20px;"));
        lineEdit_2 = new QLineEdit(tab);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(330, 340, 431, 71));
        lineEdit_2->setStyleSheet(QString::fromUtf8("font:20pt \"Ubuntu\";\n"
"background-color: rgb(255,255,255);\n"
"border-radius:20px;"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        textEdit = new QTextEdit(tab_2);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(200, 100, 521, 251));
        textEdit->setStyleSheet(QString::fromUtf8("border:none;\n"
"background-color:transparent;"));
        clearallbut = new QPushButton(tab_2);
        clearallbut->setObjectName(QString::fromUtf8("clearallbut"));
        clearallbut->setGeometry(QRect(260, 370, 341, 41));
        clearallbut->setStyleSheet(QString::fromUtf8("font:15pt \"Ubuntu\";\n"
"background-color:rgb(255,255,255);\n"
"border-radius:20px;\n"
"border:2px solid black;"));
        progressBar = new QProgressBar(tab_2);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(120, 470, 621, 41));
        progressBar->setStyleSheet(QString::fromUtf8("\n"
"QProgressBar{\n"
"	background-color: rgb(61, 56, 70);\n"
"border-radius:20px;\n"
"}\n"
"QProgressBar::chunk{\n"
"background-color: rgb(38, 162, 105);\n"
"\n"
"\n"
"border-radius:20px;\n"
"}"));
        progressBar->setValue(0);
        progressBar->setTextVisible(false);
        label = new QLabel(tab_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(120, 100, 511, 41));
        label->setStyleSheet(QString::fromUtf8("font:40px;\n"
"border:3px solid  red;\n"
"border-radius:20px;\n"
"background-color:transparent;"));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        label_3 = new QLabel(tab_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(280, 150, 471, 61));
        label_3->setStyleSheet(QString::fromUtf8("font:20pt \"Ubuntu\";\n"
"background-color:rgb(255,255,255);\n"
"border-radius:20px;\n"
"border:2px solid black;"));
        label_4 = new QLabel(tab_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(290, 310, 471, 61));
        label_4->setStyleSheet(QString::fromUtf8("font:20pt \"Ubuntu\";\n"
"background-color:rgb(255,255,255);\n"
"border-radius:20px;\n"
"border:2px solid black;"));
        updatedate_but = new QPushButton(tab_3);
        updatedate_but->setObjectName(QString::fromUtf8("updatedate_but"));
        updatedate_but->setGeometry(QRect(260, 440, 331, 61));
        updatedate_but->setStyleSheet(QString::fromUtf8("font:15pt \"Ubuntu\";\n"
"background-color: rgb(246, 245, 244);\n"
"border:3px solid black;\n"
"border-radius:20px;"));
        label_8 = new QLabel(tab_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(160, 320, 71, 41));
        label_8->setStyleSheet(QString::fromUtf8("font: 15pt \"Ubuntu Condensed\";\n"
"background-color: rgb(255, 255, 255);\n"
"border-radius:20px;\n"
"font:bold;\n"
"border:1px solid black\n"
"\n"
""));
        label_9 = new QLabel(tab_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(150, 160, 71, 41));
        label_9->setStyleSheet(QString::fromUtf8("font: 15pt \"Ubuntu Condensed\";\n"
"background-color: rgb(255, 255, 255);\n"
"border-radius:20px;\n"
"font:bold;\n"
"border:1px solid black\n"
"\n"
""));
        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        label_5 = new QLabel(tab_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(130, 200, 71, 41));
        label_5->setStyleSheet(QString::fromUtf8("font: 15pt \"Ubuntu Condensed\";\n"
"background-color: rgb(255, 255, 255);\n"
"border-radius:20px;\n"
"font:bold;\n"
"border:1px solid black\n"
"\n"
""));
        label_6 = new QLabel(tab_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(130, 320, 71, 41));
        label_6->setStyleSheet(QString::fromUtf8("font: 15pt \"Ubuntu Condensed\";\n"
"background-color: rgb(255, 255, 255);\n"
"border-radius:20px;\n"
"font:bold;\n"
"border:1px solid black\n"
"\n"
""));
        lineEdit_3 = new QLineEdit(tab_4);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(300, 180, 331, 71));
        lineEdit_3->setStyleSheet(QString::fromUtf8("font:15pt \"Ubuntu\";\n"
"background-color: rgb(255,255,255);\n"
"border-radius:20px;\n"
"border:2px solid black;"));
        lineEdit_4 = new QLineEdit(tab_4);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(300, 310, 331, 61));
        lineEdit_4->setStyleSheet(QString::fromUtf8("font:15pt \"Ubuntu\";\n"
"background-color: rgb(255,255,255);\n"
"border-radius:20px;\n"
"border:2px solid black;"));
        checkBox = new QCheckBox(tab_4);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(290, 440, 181, 23));
        updatenetbut = new QPushButton(tab_4);
        updatenetbut->setObjectName(QString::fromUtf8("updatenetbut"));
        updatenetbut->setGeometry(QRect(300, 510, 211, 51));
        updatenetbut->setStyleSheet(QString::fromUtf8("font:15pt \"Ubuntu\";\n"
"background-color:rgb(255,255,255);\n"
"border-radius:20px;\n"
"border:3px solid black;"));
        checkBox_2 = new QCheckBox(tab_4);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setGeometry(QRect(670, 330, 92, 23));
        tabWidget->addTab(tab_4, QString());
        clockbut = new QPushButton(general);
        clockbut->setObjectName(QString::fromUtf8("clockbut"));
        clockbut->setGeometry(QRect(130, 410, 171, 91));
        clockbut->setStyleSheet(QString::fromUtf8("font:15pt \"Ubuntu\";\n"
"background-color:rgb(255,255,255);\n"
"border-radius:20px;\n"
"border:5px solid black;"));
        label_7 = new QLabel(general);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(430, 10, 801, 31));
        label_7->setStyleSheet(QString::fromUtf8("background-color: rgb(162, 128, 137);"));
        storagebut = new QPushButton(general);
        storagebut->setObjectName(QString::fromUtf8("storagebut"));
        storagebut->setGeometry(QRect(130, 280, 181, 91));
        storagebut->setStyleSheet(QString::fromUtf8("font:15pt \"Ubuntu\";\n"
"background-color:rgb(255,255,255);\n"
"border-radius:20px;\n"
"border:5px solid black;"));
        backbut = new QPushButton(general);
        backbut->setObjectName(QString::fromUtf8("backbut"));
        backbut->setGeometry(QRect(40, 20, 111, 91));
        backbut->setStyleSheet(QString::fromUtf8("image: url(:/images/backarrowimg.png);\n"
"background-color:transparent;\n"
"border:none;"));

        retranslateUi(general);

        QMetaObject::connectSlotsByName(general);
    } // setupUi

    void retranslateUi(QDialog *general)
    {
        general->setWindowTitle(QCoreApplication::translate("general", "Dialog", nullptr));
        networkingbut->setText(QCoreApplication::translate("general", "Networking", nullptr));
        about_but->setText(QCoreApplication::translate("general", "About", nullptr));
        seria_lab->setText(QCoreApplication::translate("general", "Serial Number", nullptr));
        software_lab->setText(QCoreApplication::translate("general", "Software Version", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("general", "Tab 1", nullptr));
        textEdit->setHtml(QCoreApplication::translate("general", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:18pt; font-weight:600;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; font-weight:600;\">Total Disk Space : 3599.00 MB</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-ind"
                        "ent:0; text-indent:0px; font-size:18pt; font-weight:600;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; font-weight:600;\">Available Disk Space: 2997.00MB</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:18pt; font-weight:600;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; font-weight:600;\">Log Space Utilization: 0.00MB</span></p></body></html>", nullptr));
        clearallbut->setText(QCoreApplication::translate("general", "Clear All Logs", nullptr));
        label->setText(QCoreApplication::translate("general", "Storage Information", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("general", "Tab 2", nullptr));
        label_3->setText(QString());
        label_4->setText(QString());
        updatedate_but->setText(QCoreApplication::translate("general", "Update Date And Time", nullptr));
        label_8->setText(QCoreApplication::translate("general", "TIME", nullptr));
        label_9->setText(QCoreApplication::translate("general", "DATE", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("general", "Page", nullptr));
        label_5->setText(QCoreApplication::translate("general", "SSID", nullptr));
        label_6->setText(QCoreApplication::translate("general", "PSK", nullptr));
        checkBox->setText(QCoreApplication::translate("general", "Clear Existing Entries", nullptr));
        updatenetbut->setText(QCoreApplication::translate("general", "Update NetWorking ", nullptr));
        checkBox_2->setText(QCoreApplication::translate("general", "Show", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("general", "Page", nullptr));
        clockbut->setText(QCoreApplication::translate("general", "Clock", nullptr));
        label_7->setText(QString());
        storagebut->setText(QCoreApplication::translate("general", "Storage", nullptr));
        backbut->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class general: public Ui_general {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GENERAL_H
