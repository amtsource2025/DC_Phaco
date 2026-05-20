#ifndef PRIME_H
#define PRIME_H

#include <QWidget>
#include "mainwindow.h"
#include<QTimer>
#include "doctor.h"
//#include "settings.h"
#include"hwhandler.h"
#include"tuning.h"
class MainWindow;
class doctor;
class settings;
namespace Ui {
class prime;

}

class prime : public QWidget
{
    Q_OBJECT

public:
    explicit prime(QWidget *parent = nullptr);
    ~prime();
    void click();

    void onUpdateStatusTimeout();
    void start_irrigation();

    void champer_Filled();
    void done();
    void motoron();
    void motoroff();
protected:
    bool eventFilter(QObject *watched, QEvent *event) override; // Declaration of eventFilter


public slots:
    void timer();
    void primetimer();
    void current(int tab);
    void Start_Tune();

    void Tune();
private slots:
    void Prime();


    void Clean();








    void on_start_prime_but_2_clicked();

    void on_begin_clean_but_2_clicked();

    void on_pushButton_5_clicked();


    void on_pushButton_8_clicked();

    void on_Tune_but_clicked();



private:
    Ui::prime *ui;
//    int hp=0;
    MainWindow *m;
    tuning *tune;
    QTimer *timer1;
    QTimer *pretimer;
    int value=0;
    int val1=0;
    doctor *sur;
    settings *set;
     QTimer *statusUpdateTimer;
     hwhandler *hand;
};

#endif // PRIME_H
