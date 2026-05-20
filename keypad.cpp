#include "keypad.h"
#include "ui_keypad.h"

#include <QDebug>

keypad::keypad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::keypad)
{
    ui->setupUi(this);

    connect(ui->pushButton,    &QPushButton::clicked, this, &keypad::entertext);   // 1
    connect(ui->pushButton_2,  &QPushButton::clicked, this, &keypad::entertext);   // 6
    connect(ui->pushButton_3,  &QPushButton::clicked, this, &keypad::entertext);   // 9
    connect(ui->pushButton_4,  &QPushButton::clicked, this, &keypad::entertext);   // 5
    connect(ui->pushButton_5,  &QPushButton::clicked, this, &keypad::entertext);   // 3
    connect(ui->pushButton_6,  &QPushButton::clicked, this, &keypad::entertext);   // 4
    connect(ui->pushButton_8,  &QPushButton::clicked, this, &keypad::entertext);   // 1
    connect(ui->pushButton_10, &QPushButton::clicked, this, &keypad::entertext);   // 2
    connect(ui->pushButton_11, &QPushButton::clicked, this, &keypad::enterenter);  // ENTER
    connect(ui->pushButton_12, &QPushButton::clicked, this, &keypad::entertext);   // 7
    connect(ui->pushButton_13, &QPushButton::clicked, this, &keypad::entertext);   // 0
    connect(ui->pushButton_14, &QPushButton::clicked, this, &keypad::entertext);   // .
    connect(ui->pushButton_15, &QPushButton::clicked, this, &keypad::enterclear);  // CLEAR
    connect(ui->pushButton_16, &QPushButton::clicked, this, &keypad::enterenter);  // BACK

    try {
            handler = new hwhandler;
        } catch (const std::runtime_error &e) {
            qDebug() << "hwhandler init error:" << e.what();
            handler = nullptr;
        }
    }
keypad::~keypad()
{
    delete ui;
}

void keypad::entertext()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        digit = button->text();
        emit textsignal(digit);
    }
    if (handler) handler->buzz();
}

void keypad::enterenter()
{
    emit entersignal();
    handler->buzz();
}

void keypad::enterclear()
{
    emit clearsignal();
    if (handler) handler->buzz();
}
