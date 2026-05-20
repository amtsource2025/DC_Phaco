#ifndef FOOTSWITCH_H
#define FOOTSWITCH_H

#include <QDialog>
#define PATH "/home/phacohigh.db"
#include<QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include<QDebug>
namespace Ui {
class footswitch;
}

class footswitch : public QDialog
{
    Q_OBJECT

public:
    explicit footswitch(QWidget *parent = nullptr);
    ~footswitch();
    void setValues();
    void SurgeonFoot(const QString &surgeon);
    void receivelineEditval(int &val1,int &val2,int &val3,int &val4);
    void rxfootpedalmodes(const QString &topleft,const QString &topright,const QString &bottomleft,const QString &bottomright);
protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    void on_But_save_clicked();

    void on_But_zero_clicked();

    void on_But_zerodown_clicked();

    void on_But_oneup_clicked();

    void on_But_onedown_clicked();

    void on_But_twoup_clicked();

    void on_But_twodown_clicked();

    void on_But_threeup_clicked();

    void on_But_threedown_clicked();

    void on_pushButton_clicked();
    void on_But_footswitch_clicked();

    void on_But_topleft_clicked();

    void on_But_topright_clicked();

    void on_But_bottomleft_clicked();

    void on_But_bottomright_clicked();

    void on_But_save_2_clicked();

signals:
    void sendvaltomain(int &val0,int &val1,int &val2,int &val3);
    void topleft(const QString &left);
    void topright(const QString &right);
    void bottomleft(const QString &bleft);
    void bottomright(const QString &bright);
private:
    Ui::footswitch *ui;
    int part1Value = 0;
       int part2Value = 0;
       int part3Value = 0;
       int part4Value = 0;
       QSqlDatabase db;
       QString surgeonName;
       bool showingpaint=false;

};

#endif // FOOTSWITCH_H
