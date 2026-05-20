#ifndef PRIME_H
#define PRIME_H

#include <QWidget>
#include <QTimer>
#include "doctor.h"
#include "hwhandler.h"
#include "tuning.h"
#include <QSqlDatabase>
#include <QSqlQuery>

#define PATH "/home/phacohigh.db"

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
    // CHANGE 1: add MainWindow* parameter
    explicit prime(MainWindow *mainWin, QWidget *parent = nullptr);
    ~prime();
    void click();
    void onUpdateStatusTimeout();
    void start_irrigation();
    void champer_Filled();
    void done();
    void motoron();
    void motoroff();
    void serialnumber();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
    void timer();
    void primetimer();
    void current(int tab);
    void Start_Tune();
    void onComboBoxIndexChanged(int index);
    void setLastSelectedValue();
    void Tune();
    void comboboxselected();

private slots:
    void Prime();
    void Clean();
    void on_start_prime_but_2_clicked();
    void on_begin_clean_but_2_clicked();
    void on_Tune_but_clicked();

signals:
    void sendsignal(const QString &text);
    void sendsurgeon_tune(const int &text);

private:
    Ui::prime *ui;
    // CHANGE 2: add m_mainWin member
    MainWindow *m_mainWin;
    tuning *tune;
    QTimer *timer1;
    QTimer *pretimer;
    QTimer *cleantimer;
    int value = 0;
    int val1 = 0;
    doctor *sur;
    settings *set;
    QTimer *statusUpdateTimer;
    hwhandler *hand;
    QSqlDatabase db;
    int nFsCount;
};

#endif // PRIME_H
