/********************************************************************************
** Form generated from reading UI file 'another.ui'
**
** Created by: Qt User Interface Compiler version 5.15.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANOTHER_H
#define UI_ANOTHER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_another
{
public:

    void setupUi(QDialog *another)
    {
        if (another->objectName().isEmpty())
            another->setObjectName(QString::fromUtf8("another"));
        another->resize(640, 480);

        retranslateUi(another);

        QMetaObject::connectSlotsByName(another);
    } // setupUi

    void retranslateUi(QDialog *another)
    {
        another->setWindowTitle(QCoreApplication::translate("another", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class another: public Ui_another {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANOTHER_H
