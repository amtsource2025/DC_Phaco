#ifndef TUNING_H
#define TUNING_H

#include <QDialog>
#include<QPainter>
#include<QTimer>
#include<QtMath>
#include"mainwindow.h"
#include"hwhandler.h"
#include"vaccum.h"
#include<QSqlDatabase>
#define PATH "/home/phacohigh.db"

#define TUNE_LOWERFREQ_COUNT 2564
#define TUNE_HIGHFREQ_COUNT 2272

#define ADS7841_FS_CH0              0x97
#define ADS7841_SENSOR_CH1          0xD7
#define ADS7841_CURRENTSENSOR_CH2   0xA7
#define ADS7841_VOLTAGESENSOR_CH3   0xE7

#define XPAR_AXI_ADITHYA_0_BASEADDR 0x43C20000
#define XPAR_AXI_SPEAK_0_BASEADDR   0x43C30000

namespace Ui {
class tuning;
}

class tuning : public QDialog
{
    Q_OBJECT

public:
    explicit tuning(MainWindow *mainWin = nullptr, QWidget *parent = nullptr);  // Changed this line
    ~tuning();
    void setValue(int value);
    void updateCircle();
    void resizeEvent(QResizeEvent *event);
    void exportGPIO(int pin);
    void setGPIODirection(const QString &direction, int pin);
    int readGPIOValue(int pin);
    void updatehandpieceStatus();
    void updateProgress();
    void resetState();
    int Tune_Phaco();
    int ADC_value();
    void loose_tip();

public slots:
    void on_pushButton_clicked();
    void rx_surgeonName(const int &text);
    void setHandpieceNo(const QString &hpNo);

protected:
    void paintEvent(QPaintEvent *event) override;
    void paintEvent1(QPaintEvent *event);

private slots:
    void on_But_Next_clicked();
    void on_But_Handpiece_clicked();
    void on_But_value_clicked();
    void on_ButRTune_clicked();
    void on_butTuned_clicked();
    void alreadyTune();

signals:
    void sendvisibleisgnale(bool status);
    void sendfreq(int count);
    void activatemain();

private:
    Ui::tuning *ui;
    int m_value = 0;
    int m_value1 = 0;
    int currentCircle;
    QSqlDatabase db;
    QTimer *timer;
    bool isRunning = false;
    QTimer *timer1;
    QColor circleColor;
    MainWindow *main;
    hwhandler *hand;
    QTimer *handpiece;
    Vaccum *vacSensor;
    bool powerState;
    bool tuneCompleted = false;
    int nADCValue;
    int statushp;
    int nFsCount;
    int surgeon;
    int previousGpioValue;
    bool isTuned = false;
    static QString m_handpieceNo;
};

#endif // TUNING_H
