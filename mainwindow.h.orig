#ifndef MAINWINDOW_H
#define MAINWINDOW_H


//#include "settings.h"

#include "keypad.h"
#include <QMainWindow>
#include<QSlider>
#include<QPushButton>
#include<QLabel>
#include<QTableWidget>
#include<QMessageBox>
#include <QTimer>
#include<QIntValidator>
#include "footlib.h"
#include "vaccum.h"
#include<QElapsedTimer>
#include"hwhandler.h"
#include"doctor.h"
#include <QDir>
#include <QFileInfo>
#include<QSqlQuery>
#include<QSqlRecord>
#include<QMap>
#include<QComboBox>

#define SQLPATH "/home/amt-04/phacohigh.db"
#define PATH "/home/phacohigh.db"

class settings;
class prime;
 class footpedal;
 class doctor;
 class demo;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;

             }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void footpedalbeep();
    void beepsound();
void push(const QString &surgeonName);
void changesvaluesql();
void changebuttonstyle();
void speedofthelabe(QLabel *label);
    bool eventFilter(QObject* object, QEvent* event);
    void setRange(QLineEdit* lineEdit, int prevValue, int value, int maxValue);
    void exportGPIO(int pin);
    void setGPIODirection(const QString &direction,int pin);
    int readGPIOValue(int pin);
    void updateStatus();
    void printAllRows();
    void handlePDM();
    int range=0;
    int rec;
    int butname=0;
    int readGPIO(int gpio,int gpio1,int gpio2,int gpio3);
    void moveTab(int usIndex);
    void moveTab1(int usIndex);
    void moveTab2(int usIndex);
    void moveTab3(int usIndex);
    struct SystemState {
           bool powerOn;
           bool powerOn1;
           bool powerOn2;
           bool powerOn3;
           bool powerOn4;
           QString buttonState;
           QString button;
           //us2
           QString buttonstate1;
           QString button1;
           //us3
           QString buttonstate2;
           QString button2;
           //us4
           QString buttonstate3;
           QString button3;
           //vit
           QString buttonstate4;
           QString button4;

       }power;
    struct SurgeonData {
        QString surgeonName;
        int lineEdit1Value;
        int lineEdit2Value;
         int lineEdit3Value;
         QString powerDelivery;
         QString pdmMode;
         QString vacmode;
    };
    struct SurgicalData {
        QString comboBoxValue;
        QString combo;
        int dia;
        int us1pow, us1vac, us1asp;
        int us2pow, us2vac, us2asp;
        int us3pow, us3vac, us3asp;
        int us4pow, us4vac, us4asp;
        int ia1vac, ia1asp;
        int ia2vac, ia2asp;
        int vitcut, vitvac, vitasp;
        QString powmode, vacmode, powmethod;
        QString us2powmode, us2vacmode, us2powermethod;
        QString us3powmode, us3vacmode, us3powermethod;
        QString us4powmode, us4vacmode, us4powermethod;
        QString ia1mode, ia2mode;
        QString vitmode, vitvacmode;
    }surgicalData;
    int Flow_LUT[42]={95,95,  //0
                    110,110,  //2
                    112,112,  //4
                    115,115,  //6
                    122,122,  //8
                    130,130, //10
                    145,145, //12
                    155,150, //14
                    165,155, //16
                    175,160, //18
                    185,170, //20
                    195,195, //22
                    205,205, //24
                    215,215, //26
                    225,225, //28
                    245,245, //30
                    255,255, //32
                    265,265, //34
                    275,275, //36
                    290,290, //38
                    299,299  //40
                    };
void setTuneMode();
void enableButtons(bool powerOn);
   void setLastSelectedValue();
   void rx_viberationvalue(const QString &text);
public slots:
   int readsensorvalue();
   void footpedalvalues(int &value1,int &value2,int &value3,int &value4);
   void rx_defaultvalues(int &value1,int &value2,int &value3,int &value4);
   void disablefunction();
 void onComboBoxIndexChanged(int index);
 void onPdmModeSelected(int gpio);
 void onPdmModeSelected1(int gpio);
 void onPdmModeSelected2(int gpio);
 void onPdmModeSelected3(int gpio);
 void updateLineedit(QLineEdit *lineEdit, int prevValue, int value, int maxValue);
 void on_clicked(const QString& digit);
 void on_clickedenter();
 void current(int tab);
 void tabupdate(int index);
 void footpedalcheck();
 void updateElapsedTime();
 void updateProgressBar();
 void motoron(QLineEdit *lineEdit);
 void motoroff();
 void motorccwon();
 int increasebutton(int input);
 int decreasebutton(int  input);
 void DIATHERMYBUT();
 void moved(int gpio);
 void movePushButtonBottomToTop(int gpio);
 void footreflux(int gpio);
 void continousirrigation(int value);
 void poweronoff(int gpio);

 void onCutMode_vitComChanged(int index);
 void onCutMode_vitComChanged1(int index);
 void onCutMode_vitComChanged2(int index);
 void onCutMode_vitComChanged3(int index);
 void receivecombo(const QString &text);
 void performpump(const QString &text);
 void receiveValues(
        const QString &comboBoxValue,
        const QString &combo,
        int dia,
        int us1pow, int us1vac, int us1asp,
        int us2pow, int us2vac, int us2asp,
        int us3pow, int us3vac, int us3asp,
        int us4pow, int us4vac, int us4asp,
        int ia1vac, int ia1asp,
        int ia2vac, int ia2asp,
        int vitcut, int vitvac, int vitasp,
        const QString &powmode, const QString &vacmode, const QString &powmethod,
        const QString &us2powmode, const QString &us2vacmode, const QString &us2powermethod,
        const QString &us3powmode, const QString &us3vacmode, const QString &us3powermethod,
        const QString &us4powmode, const QString &us4vacmode, const QString &us4powermethod,
        const QString &ia1mode, const QString &ia2mode,
        const QString &vitmode, const QString &vitvacmode
    );

 void on_pushButton_clicked();
 void ULTRASONICBUT1();
private slots:

 void sensor2();
 void ULTRASONICBUT2();

 void ULTRASONICBUT4();

 void IRRIGATIONBUT1();

 void IRRIGATIONBUT2();

 void VITRECTOMYBUT();

 void diapowup();

 void diapowdown();

 void vitcutup();

 void vitcutdown();

 void vitvacup();

 void vitvacdown();

 void vitflowup();

 void vitflowdown();

 void ia2vacup();

 void ia2vacdown();

 void ia2flowup();

 void ia2flowdown();

 void ia1vacup();

 void ia1vacdown();

 void ia1flowup();

 void ia1flowdown();

 void US1POWUP();

 void US1POWDOWN();

 void US1VACDOWN();

 void US1VACUP();

 void US1FLOWDOWN();

 void US1FLOWUP();

 void us2powup();

 void us2powdown();

 void us2vacup();

 void us2vacdown();

 void us2flowup();

 void us2flowdown();

 void ULTRASONICBUT3();

 void us3powup();

 void us3powdown();

 void us3vacup();

 void us3vacdown();

 void us3flowup();

 void us3flowdown();

void us4powup();

 void us4powdown();

 void us4vacup();

 void us4vacdown();

 void us4flowup();

 void us4flowdown();

 void BACKBUT();


void us1_linear_nonlinear();

void us2_linear_nonlinear();


void us3_linear_nonlinear();

void us4_linear_nonlinear();


void ia1_linear_nonlinear();

void ia2_linear_nonlinear();

void vit_linear_nonlinear();

void pulseup_mode();

void pulsedown_mode();

void ocuburstup_mode();

void ocuburstdown_mode();

void singleburstup_mode();

void singleburstdown_mode();

void multiburstup_mode();

void multiburstdown_mode();

void ocupulseup_mode();

void ocupulsedown_mode();

void coldphacoup_mode();

void coldphacodown_mode();

void coldphaco1up_mode();

void coldphaco1down_mode();

void updatehandpieceStatus();

void linearvaccum();
void onUpdateStatusTimeout();

void updateTimers();

void on_us1onoff_clicked();

void on_us2onoff_clicked();

void on_us3onoff_clicked();

void on_us4onoff_clicked();

void on_vitonoff_clicked();

void doctorwindow_show(QString Sugroenname);

void footpedalwindow_show();

void on_us1vacmode_clicked();

void on_us2vacmode_clicked();

void on_us3vacmode_clicked();

void on_vitvacmode_clicked();

void on_us4vacmode_clicked();

void on_CI4_2_clicked();

void on_SETTINGS_BUT_2_clicked();

void on_pushButton_42_clicked();




void on_ocuburstdown_but_clicked();

void on_ocuburstup_but_clicked();

void on_ocupulsedown_but_clicked();

void on_ocupulseup_but_clicked();

signals:
     void sensorValueChanged(int value);
     void surgeonSelected(const QString &surgeonName);
     void left_foot(const QString &value);
     void right_foot(const QString &value);
     void bottom_left(const QString &value);
     void bottom_right(const QString &value);
     void con_irrigation(bool on); // Signal to handle Continuous Irrigation state
     void sendsurgeon(const QString &value);
private:
    Ui::MainWindow *ui;
    //instance
    settings *s;
    keypad *key;
    footpedal *foot;
    doctor *in;
    footlib *lfoot;
    hwhandler *handler;
    Vaccum *vac;



int getvalue(int input);
int currentButtonIndex;
int maxValue;
int currentButton;
int counter=0;
int currentbut;
int pulse;
int ocupulse;
int ocuburst;
int singleburst;
int multiburst;
int coldphaco;
int coldphaco1;
int gpioNumber = 960;
int nfpzero;
int nfpone;
int nfptwo;
int nfpthree;
int diacount=0;
int us1count=0;
int us2count=0;
int us3count=0;
int us4count=0;
int ia1count=0;
int ia2count=0;
int vitcount=0;

QMessageBox *message;
QMessageBox *messageline;

QTimer  *statusUpdateTimer;
QElapsedTimer timer5;
QTimer *sensortimer;
QTimer *footime;
QTimer *protimer;
QTimer *footsensor;
QTimer *timermsg;
QTime currentTimer;
QTimer *updateTimer;
QTimer *Tacutalsensor;
  QTimer *timerfoot;

bool flag1=false;
bool on;
bool cius1;
bool cius2;
bool cius3;
bool cius4;
bool civit;
bool ci_ia1;
bool ci_ia2;
bool cidia;
bool overallci;
bool con=1;
bool us1PdmMode = false;
bool us2PdmMode = false;
bool us3PdmMode = false;
bool us4PdmMode = false;
bool powerOn1;
bool isTuneEnabled;
bool us1linear=false;
bool ia1vaclinear=false;
bool ia2vaclinear=false;
bool ventonus1=false;
bool ventonus2=false;
bool ventonus3=false;
bool ventonus4=false;
bool ventonia1=false;
bool ventonia2=false;
bool ventonvit=false;
bool ventondia=false;
bool overallpower=false;
bool us1poweron;
bool us2poweron;
bool us3poweron;
bool us4poweron;
bool vitonoff=false;
bool motoria1 = false;
bool motoria2=false;
bool motorus1=false;
bool motorus2=false;
bool motorus3=false;
bool motorus4=false;
bool motorvit = false;



QString us1;
QString us2;
QString us3;
QString us4;
QString ia1;
QString ia2;
QString vit;
QString vus1;
QString vus2;
QString vus3;
QString vus4;
QString vvit;
QString vibon;


qint64 elap=0;
qint64 elapsedTimeUS1;
qint64 elapsedTimeUS2;
qint64 elapsedTimeUS3;
qint64 elapsedTimeUS4;



QMap<QString, QMap<int, QStringList>> surgeonData;
QMap<QLineEdit*, int> lastValidValues;
QMap<QLineEdit*, int> lastValidValues1;

QSqlDatabase db;

void updateLabel();
void updateTabsBasedOnComboBox(const QString &text);

};
#endif // MAINWINDOW_H
