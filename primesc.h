#ifndef PRIMESC_H
#define PRIMESC_H

#include <QWidget>
#include "mainwindow.h"
#include<QTimer>
#include "doctor.h"
#include "settings.h"
class MainWindow;
class doctor;
class settings;
namespace Ui {
class primesc;

}

class primesc : public QWidget
{
    Q_OBJECT

public:
    explicit primesc(QWidget *parent = nullptr);
    ~primesc();
    void click();

public slots:
    void timer();
    void current(int tab);
    void update();
   void hp(int connected);
private slots:
    void prime();

    void Tune();

    void Clean();

    void Start_Tune();




    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


    void on_pushButton_4_clicked();

private:
    Ui::primesc *ui;
//    int hp=0;
    MainWindow *m;
    QTimer *timer1;
    int value=0;
    int val1=0;
    doctor *sur;
    settings *set;
};

#endif // PRIMESC_H
