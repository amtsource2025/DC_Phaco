/********************************************************************************
** Form generated from reading UI file 'prime.ui'
**
** Created by: Qt User Interface Compiler version 5.15.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRIME_H
#define UI_PRIME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_prime
{
public:
    QPushButton *clean_but;
    QPushButton *Tune_but;
    QPushButton *prime1_but;
    QTabWidget *tabWidget;
    QWidget *tab_6;
    QLabel *process_lab_2;
    QPushButton *start_prime_but_2;
    QCheckBox *start_check_2;
    QCheckBox *wait_Check_2;
    QCheckBox *motor_Check_2;
    QCheckBox *done_Check_2;
    QProgressBar *progressBar_2;
    QWidget *tab_8;
    QLabel *warning_lab_2;
    QTextEdit *Instruction_lab_2;
    QPushButton *begin_clean_but_2;
    QLabel *warning_img_2;
    QLabel *label_10;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *lab_time;
    QLabel *lab_date;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QComboBox *comboBox_4;
    QLabel *lbloverall;
    QLabel *label_7;
    QFrame *line;

    void setupUi(QWidget *prime)
    {
        if (prime->objectName().isEmpty())
            prime->setObjectName(QString::fromUtf8("prime"));
        prime->resize(1280, 720);
        prime->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(18, 78, 102);\n"
"\n"
"\n"
""));
        clean_but = new QPushButton(prime);
        clean_but->setObjectName(QString::fromUtf8("clean_but"));
        clean_but->setGeometry(QRect(0, 500, 351, 141));
        clean_but->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font: 30pt \"Ubuntu\";\n"
"    background-color: transparent;\n"
"    border-radius: 20px;\n"
"    border: none;\n"
"	image: url(:/images/tuneddd.png);\n"
"    font-weight: bold;\n"
"color:white;\n"
"}\n"
"\n"
"QPushButton:focus {\n"
"    outline: none;\n"
"    border: none;\n"
"}\n"
""));
        Tune_but = new QPushButton(prime);
        Tune_but->setObjectName(QString::fromUtf8("Tune_but"));
        Tune_but->setGeometry(QRect(0, 280, 351, 141));
        Tune_but->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font: 30pt \"Ubuntu\";\n"
"    background-color: transparent;\n"
"    border-radius: 20px;\n"
"    border: none;\n"
"	image: url(:/images/tuneddd.png);\n"
"    font-weight: bold;\n"
"color:white;\n"
"}\n"
"\n"
"QPushButton:focus {\n"
"    outline: none;\n"
"    border: none;\n"
"}\n"
""));
        prime1_but = new QPushButton(prime);
        prime1_but->setObjectName(QString::fromUtf8("prime1_but"));
        prime1_but->setGeometry(QRect(0, 90, 351, 141));
        prime1_but->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font: 30pt \"Ubuntu\";\n"
"    background-color: transparent;\n"
"    border-radius: 20px;\n"
"    border: none;\n"
"	image: url(:/images/tuneddd.png);\n"
"    font-weight: bold;\n"
"color:white;\n"
"}\n"
"\n"
"QPushButton:focus {\n"
"    outline: none;\n"
"    border: none;\n"
"}\n"
""));
        tabWidget = new QTabWidget(prime);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(410, -30, 871, 791));
        tabWidget->setStyleSheet(QString::fromUtf8(""));
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        tab_6->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(18, 78, 102);\n"
"\n"
"\n"
""));
        process_lab_2 = new QLabel(tab_6);
        process_lab_2->setObjectName(QString::fromUtf8("process_lab_2"));
        process_lab_2->setGeometry(QRect(140, 110, 351, 51));
        process_lab_2->setStyleSheet(QString::fromUtf8("font:25pt \"Ubuntu\";\n"
"background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);\n"
"border-radius:20px;\n"
"border:none;font:bold;"));
        start_prime_but_2 = new QPushButton(tab_6);
        start_prime_but_2->setObjectName(QString::fromUtf8("start_prime_but_2"));
        start_prime_but_2->setGeometry(QRect(250, 620, 321, 121));
        start_prime_but_2->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"font:20pt \"Ubuntu\";\n"
"image: url(:/images/g4210.png);\n"
"background-color:transparent;\n"
"color: rgb(255, 255, 255);\n"
"border-radius:20px;\n"
"border:none;font:bold;\n"
"}\n"
"QPushButton:focus {\n"
"    outline: none;\n"
"    border: none;\n"
"}\n"
""));
        start_check_2 = new QCheckBox(tab_6);
        start_check_2->setObjectName(QString::fromUtf8("start_check_2"));
        start_check_2->setGeometry(QRect(110, 260, 721, 51));
        start_check_2->setStyleSheet(QString::fromUtf8("QCheckBox::indicator {\n"
"	color: rgb(255, 255, 255);\n"
"    width: 30px;\n"
"    height: 30px;\n"
"}\n"
"QCheckBox {\n"
"    font-size: 30px;\n"
"	color: rgb(255, 255, 255);\n"
"\n"
"}\n"
""));
        wait_Check_2 = new QCheckBox(tab_6);
        wait_Check_2->setObjectName(QString::fromUtf8("wait_Check_2"));
        wait_Check_2->setGeometry(QRect(110, 360, 751, 71));
        wait_Check_2->setStyleSheet(QString::fromUtf8("QCheckBox::indicator {\n"
"	color: rgb(255, 255, 255);\n"
"    width: 30px;\n"
"    height: 30px;\n"
"}\n"
"QCheckBox {\n"
"    font-size: 30px;\n"
"	color: rgb(255, 255, 255);\n"
"\n"
"}\n"
""));
        motor_Check_2 = new QCheckBox(tab_6);
        motor_Check_2->setObjectName(QString::fromUtf8("motor_Check_2"));
        motor_Check_2->setGeometry(QRect(110, 170, 381, 61));
        motor_Check_2->setStyleSheet(QString::fromUtf8("QCheckBox::indicator {\n"
"	color: rgb(255, 255, 255);\n"
"    width: 30px;\n"
"    height: 30px;\n"
"}\n"
"QCheckBox {\n"
"    font-size: 30px;\n"
"	color: rgb(255, 255, 255);\n"
"\n"
"}\n"
""));
        done_Check_2 = new QCheckBox(tab_6);
        done_Check_2->setObjectName(QString::fromUtf8("done_Check_2"));
        done_Check_2->setGeometry(QRect(110, 470, 321, 41));
        done_Check_2->setStyleSheet(QString::fromUtf8("QCheckBox::indicator {\n"
"	color: rgb(255, 255, 255);\n"
"    width: 30px;\n"
"    height: 30px;\n"
"}\n"
"QCheckBox {\n"
"    font-size: 30px;\n"
"	color: rgb(255, 255, 255);\n"
"\n"
"}\n"
""));
        progressBar_2 = new QProgressBar(tab_6);
        progressBar_2->setObjectName(QString::fromUtf8("progressBar_2"));
        progressBar_2->setGeometry(QRect(40, 560, 761, 51));
        progressBar_2->setValue(0);
        tabWidget->addTab(tab_6, QString());
        progressBar_2->raise();
        process_lab_2->raise();
        start_prime_but_2->raise();
        start_check_2->raise();
        wait_Check_2->raise();
        motor_Check_2->raise();
        done_Check_2->raise();
        tab_8 = new QWidget();
        tab_8->setObjectName(QString::fromUtf8("tab_8"));
        tab_8->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(18, 78, 102);\n"
"\n"
"\n"
""));
        warning_lab_2 = new QLabel(tab_8);
        warning_lab_2->setObjectName(QString::fromUtf8("warning_lab_2"));
        warning_lab_2->setGeometry(QRect(70, 110, 431, 41));
        warning_lab_2->setStyleSheet(QString::fromUtf8("font:25pt \"Ubuntu\";\n"
"background-color: rgb(36, 31, 49);\n"
"color: rgb(255, 255, 255);\n"
"border-radius:20px;\n"
"border:none;\n"
"font:bold;"));
        Instruction_lab_2 = new QTextEdit(tab_8);
        Instruction_lab_2->setObjectName(QString::fromUtf8("Instruction_lab_2"));
        Instruction_lab_2->setGeometry(QRect(130, 260, 741, 281));
        Instruction_lab_2->setStyleSheet(QString::fromUtf8("font:20px;border:none;\n"
"background-color: transparent;\n"
""));
        begin_clean_but_2 = new QPushButton(tab_8);
        begin_clean_but_2->setObjectName(QString::fromUtf8("begin_clean_but_2"));
        begin_clean_but_2->setGeometry(QRect(320, 570, 331, 141));
        begin_clean_but_2->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"font:20pt \"Ubuntu\";\n"
"background-color: transparent;\n"
"image: url(:/images/g4210.png);\n"
"color: rgb(255, 255, 255);\n"
"font:bold;\n"
"border-radius:20px;\n"
"border:none;\n"
"}\n"
"QPushButton:focus {\n"
"    outline: none;\n"
"    border: none;\n"
"}\n"
""));
        warning_img_2 = new QLabel(tab_8);
        warning_img_2->setObjectName(QString::fromUtf8("warning_img_2"));
        warning_img_2->setGeometry(QRect(90, 110, 31, 41));
        warning_img_2->setStyleSheet(QString::fromUtf8("image: url(:/images/warning1.png);\n"
"BACKGROUND-COLOR:TRANSPARENT;"));
        label_10 = new QLabel(tab_8);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(80, 180, 651, 51));
        label_10->setStyleSheet(QString::fromUtf8("font: 75 20pt \"Ubuntu\";\n"
"color: rgb(255, 255, 255);\n"
"font:bold;\n"
"background-color:transparent;"));
        tabWidget->addTab(tab_8, QString());
        label = new QLabel(prime);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(270, 160, 61, 71));
        label->setStyleSheet(QString::fromUtf8("image: url(:/images/prime.png);\n"
"background-color:transparent;\n"
"border:none;"));
        label_2 = new QLabel(prime);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(270, 360, 61, 61));
        label_2->setStyleSheet(QString::fromUtf8("image: url(:/images/wifi.png);\n"
"background-color:transparent;\n"
"border:none;"));
        label_3 = new QLabel(prime);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(270, 570, 71, 61));
        label_3->setStyleSheet(QString::fromUtf8("image: url(:/images/clean.png);\n"
"background-color:transparent;\n"
"border:none;"));
        lab_time = new QLabel(prime);
        lab_time->setObjectName(QString::fromUtf8("lab_time"));
        lab_time->setGeometry(QRect(1010, 20, 251, 51));
        lab_time->setStyleSheet(QString::fromUtf8("\n"
"background-color: transparent;\n"
"border:1px solid white;\n"
"border-radius:10px;\n"
"color: rgb(255, 255, 255);\n"
"font:20pt \"Ubuntu\";\n"
"font-weight: bold;\n"
""));
        lab_time->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lab_date = new QLabel(prime);
        lab_date->setObjectName(QString::fromUtf8("lab_date"));
        lab_date->setGeometry(QRect(744, 20, 261, 51));
        lab_date->setStyleSheet(QString::fromUtf8("\n"
"background-color: transparent;\n"
"border:1px solid white;\n"
"border-radius:10px;\n"
"color: rgb(255, 255, 255);\n"
"font:20pt \"Ubuntu\";\n"
"font-weight: bold;"));
        lab_date->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_4 = new QLabel(prime);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(1010, 20, 61, 51));
        label_4->setStyleSheet(QString::fromUtf8("border-radius:12px;\n"
"image: url(:/images/clock.png);\n"
"background-color: transparent;"));
        label_5 = new QLabel(prime);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(720, 0, 121, 91));
        label_5->setStyleSheet(QString::fromUtf8("border-radius:12px;\n"
"image: url(:/images/calendor.png);\n"
"background-color: transparent;"));
        label_6 = new QLabel(prime);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 19, 341, 51));
        label_6->setStyleSheet(QString::fromUtf8("\n"
"background-color: transparent;\n"
"border:1px solid white;\n"
"border-radius:10px;\n"
"color: rgb(255, 255, 255);\n"
"font:20pt \"Ubuntu\";\n"
"font-weight: bold;"));
        label_6->setAlignment(Qt::AlignCenter);
        comboBox_4 = new QComboBox(prime);
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->setObjectName(QString::fromUtf8("comboBox_4"));
        comboBox_4->setGeometry(QRect(350, 20, 391, 51));
        comboBox_4->setStyleSheet(QString::fromUtf8("\n"
"background-color: transparent;\n"
"border:1px solid white;\n"
"border-radius:10px;\n"
"color: rgb(255, 255, 255);\n"
"font:20pt \"Ubuntu\";\n"
"font-weight: bold;"));
        lbloverall = new QLabel(prime);
        lbloverall->setObjectName(QString::fromUtf8("lbloverall"));
        lbloverall->setGeometry(QRect(0, 0, 1281, 81));
        lbloverall->setStyleSheet(QString::fromUtf8(""));
        label_7 = new QLabel(prime);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(410, 80, 16, 651));
        label_7->setStyleSheet(QString::fromUtf8(""));
        line = new QFrame(prime);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(0, 70, 1280, 3));
        line->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        lab_date->raise();
        lab_time->raise();
        label_5->raise();
        label_4->raise();
        prime1_but->raise();
        tabWidget->raise();
        label_2->raise();
        label_3->raise();
        label->raise();
        Tune_but->raise();
        clean_but->raise();
        lbloverall->raise();
        label_6->raise();
        comboBox_4->raise();
        label_7->raise();
        line->raise();

        retranslateUi(prime);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(prime);
    } // setupUi

    void retranslateUi(QWidget *prime)
    {
        prime->setWindowTitle(QCoreApplication::translate("prime", "Form", nullptr));
        clean_but->setText(QCoreApplication::translate("prime", "CLEAN", nullptr));
        Tune_but->setText(QCoreApplication::translate("prime", "TUNE", nullptr));
        prime1_but->setText(QCoreApplication::translate("prime", "PRIME", nullptr));
        process_lab_2->setText(QCoreApplication::translate("prime", "     PROCESS ", nullptr));
        start_prime_but_2->setText(QCoreApplication::translate("prime", "START PRIME", nullptr));
        start_check_2->setText(QCoreApplication::translate("prime", "Start Irrigation Valve", nullptr));
        wait_Check_2->setText(QCoreApplication::translate("prime", "Wait For Chamber To Be Filled", nullptr));
        motor_Check_2->setText(QCoreApplication::translate("prime", "Start Motor", nullptr));
        done_Check_2->setText(QCoreApplication::translate("prime", "Done", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QCoreApplication::translate("prime", "Tab 1", nullptr));
        warning_lab_2->setText(QCoreApplication::translate("prime", "   BEFORE CLEANING", nullptr));
        Instruction_lab_2->setHtml(QCoreApplication::translate("prime", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:20px; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-weight:600; color:#ffffff;\">1.Disconnect the Irrigation/Aspiration connectors from handpiece</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt; font-weight:600; color:#ffffff;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-weight:600; color:#ffffff;\">2.Connect the irrigation,aspir"
                        "ation connectors together</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt; font-weight:600; color:#ffffff;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-weight:600; color:#ffffff;\">3.Disconnect the saline connection from irrigation</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt; font-weight:600; color:#ffffff;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-weight:600; color:#ffffff;\">4.Disconnect the tubing from irriagtion valve</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:"
                        "0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt; font-weight:600; color:#ffffff;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-weight:600; color:#ffffff;\">5.Dip the irrigation connector in distilled water bowl.</span></p></body></html>", nullptr));
        begin_clean_but_2->setText(QCoreApplication::translate("prime", "Begin Clean", nullptr));
        warning_img_2->setText(QString());
        label_10->setText(QCoreApplication::translate("prime", " Instructions To Be Followed:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_8), QCoreApplication::translate("prime", "Page", nullptr));
        label->setText(QString());
        label_2->setText(QString());
        label_3->setText(QString());
        lab_time->setText(QString());
        lab_date->setText(QString());
        label_4->setText(QString());
        label_5->setText(QString());
        label_6->setText(QString());
        comboBox_4->setItemText(0, QCoreApplication::translate("prime", "Surgeon 1", nullptr));
        comboBox_4->setItemText(1, QCoreApplication::translate("prime", "Surgeon 2", nullptr));
        comboBox_4->setItemText(2, QCoreApplication::translate("prime", "Surgeon 3", nullptr));
        comboBox_4->setItemText(3, QCoreApplication::translate("prime", "Surgeon 4", nullptr));
        comboBox_4->setItemText(4, QCoreApplication::translate("prime", "Surgeon 5", nullptr));

        lbloverall->setText(QString());
        label_7->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class prime: public Ui_prime {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIME_H
