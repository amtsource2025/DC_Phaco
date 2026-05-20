/********************************************************************************
** Form generated from reading UI file 'display.ui'
**
** Created by: Qt User Interface Compiler version 5.15.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISPLAY_H
#define UI_DISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_display
{
public:
    QLabel *label;
    QPushButton *BackBut;
    QTextEdit *textEdit;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QDialog *display)
    {
        if (display->objectName().isEmpty())
            display->setObjectName(QString::fromUtf8("display"));
        display->resize(1280, 720);
        display->setStyleSheet(QString::fromUtf8("background-color:rgb(83, 104, 120);"));
        label = new QLabel(display);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(400, 180, 381, 31));
        label->setStyleSheet(QString::fromUtf8("\n"
"font: 15pt \"Ubuntu Condensed\";"));
        BackBut = new QPushButton(display);
        BackBut->setObjectName(QString::fromUtf8("BackBut"));
        BackBut->setGeometry(QRect(30, 30, 121, 91));
        BackBut->setStyleSheet(QString::fromUtf8("\n"
"image: url(:/images/backarrowimg.png);\n"
"border:none;"));
        textEdit = new QTextEdit(display);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(320, 270, 801, 281));
        textEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border:5px solid black;\n"
"border-radius:20px;"));
        label_2 = new QLabel(display);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(220, 170, 121, 61));
        label_2->setStyleSheet(QString::fromUtf8("image: url(:/images/touchscreen.png);\n"
"background-color:transparent;"));
        label_3 = new QLabel(display);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(120, 270, 51, 51));
        label_3->setStyleSheet(QString::fromUtf8("image: url(:/notifi.png);\n"
"background-color:transparent;"));

        retranslateUi(display);

        QMetaObject::connectSlotsByName(display);
    } // setupUi

    void retranslateUi(QDialog *display)
    {
        display->setWindowTitle(QCoreApplication::translate("display", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("display", "TouchScreen Calibratic", nullptr));
        BackBut->setText(QString());
        textEdit->setHtml(QCoreApplication::translate("display", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt; font-weight:600;\">You Will Be Redirected To The Calibration Window Within 15 Seconds</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:16pt; font-weight:600;\"><br /></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:16pt; font-weight:600;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; mar"
                        "gin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt; font-weight:600;\">1.Press the Crosshairs Displayed On The Window,Once It Opens</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:16pt; font-weight:600;\"><br /></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:16pt; font-weight:600;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt; font-weight:600;\">2.Once Completed Restart The System To Effect The New Changes To The Tounchscreen Display</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:16pt; font"
                        "-weight:600;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt; font-weight:600;\"> </span></p></body></html>", nullptr));
        label_2->setText(QString());
        label_3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class display: public Ui_display {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAY_H
