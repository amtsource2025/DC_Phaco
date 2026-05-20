#ifndef FOOTPEDAL_H
#define FOOTPEDAL_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include "doctor.h"
#include"mainwindow.h"
#define PATH "phacohigh.db"


class doctor;
class MainWindow;


namespace Ui {
class footpedal;
}

class footpedal : public QDialog
{
    Q_OBJECT

public:
    explicit footpedal(QWidget *parent = nullptr);
    ~footpedal();
    // Example state and flag initialization (if they are not already initialized)
        int state1=0 ;  // State for GPIO 961
       bool flag1 = 0;   // Flag for GPIO 961
       int state2=0 ;  // State for GPIO 962
      bool flag2= 0;   // Flag for GPIO 962
      int state3=0 ;  // State for GPIO 963
     bool flag3= 0;   // Flag for GPIO 963
     int state4=0 ;  // State for GPIO 964
    bool flag4 = 0;   // Flag for GPIO 964

    void createAndSetupComboBoxes();
        void setupConnections();

          void exportGPIO(int gpioNumber);
          void setGPIODirection(int gpioNumber, const QString &direction);
          void writeGPIOValue(int gpioNumber, int value);
          int readGPIOValue(int gpioNumber);
          void readInitialGPIOValues();
          void initializeGPIO();
          void receivedval(const int &text,const int &text1,const int &text2,const int &text3);

public slots:
            void combobox1(const QString &text);
            void combobox2(const QString &text);
            void combobox3(const QString &text);
            void combobox4(const QString &text);
            void storeComboBoxSelection(int index);

            void updateFootpedalComboBoxes(const QString &surgeonName);
            void updateFootpedalComboBoxes1(const QString &surgeonName);

signals:
    void moveTopToBottom(int value);    // Signal for moving pushbutton from top to bottom
    void moveBottomToTop(int value);    // Signal for moving pushbutton from bottom to top
    void performReflux(int value);      // Signal for performing reflux action
    void togglePower(bool on);        // Signal for toggling power on/off

    void continous_irrigation(int value);
    void powerdm(int value1);
    void powerdm1(int value1);
    void powerdm2(int value1);
    void powerdm3(int value1);


    void sendleftfootdoc(const QString &text);
    void sendrightfootdoc(const QString &text);
    void sendbottomleftddoc(const QString &text);
    void sendbottomrightdoc(const QString &text);

    void activatemain();



private slots:
    void Back();
    void on_pushButton_clicked();

private:
    Ui::footpedal *ui;
QString currentSurgeonName;
QString left;
QString right;
QString bleft;
QString bright;
doctor *d1;
int pinTopLeft;
int pinTopRight;
int pinBottomLeft;
int pinBottomRight;
int index;
int gpioNumber;
MainWindow *m;
QString leftFootcomAction;
QString rightFootcomAction;
QString bleftFootcomAction;
QString brightFootcomAction;
QSqlDatabase db;
bool flagcon;
int f1;
int f2;
int f3;
int f4;



};

#endif // FOOTPEDAL_H
