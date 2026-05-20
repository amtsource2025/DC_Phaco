#ifndef KEYPAD_H
#define KEYPAD_H

#include <QWidget>
#include <QMouseEvent>

#include "hwhandler.h"

namespace Ui {
class keypad;
}

class keypad : public QWidget
{
    Q_OBJECT

public:
    explicit keypad(QWidget *parent = nullptr);
    ~keypad();
    QString digit;
    QString text1;

signals:
    void textsignal(const QString& digit);
    void entersignal();
    void clearsignal();

private slots:
    void entertext();
    void enterenter();
    void enterclear();

private:
    Ui::keypad *ui;
    hwhandler *handler;

};

#endif // KEYPAD_H
