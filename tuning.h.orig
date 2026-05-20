#ifndef TUNING_H
#define TUNING_H

#include <QDialog>
#include<QPainter>
#include<QTimer>
#include<QtMath>
#include"mainwindow.h"
#include"hwhandler.h"
#include"vaccum.h"
namespace Ui {
class tuning;
}

class tuning : public QDialog
{
    Q_OBJECT

public:
    explicit tuning(QWidget *parent = nullptr);
    ~tuning();
    // Sets the progress value (0-100)
    void setValue(int value);
    void updateCircle();
    void resizeEvent(QResizeEvent *event);
    void exportGPIO(int pin);
    void setGPIODirection(const QString &direction,int pin);
    int readGPIOValue(int pin);
    void updatehandpieceStatus();
    void updateProgress();
void resetState();
int Tune_Phaco();
int ADC_value();
public slots:
void on_pushButton_clicked();

protected:
    // Override paintEvent to draw the circular progress bar
    void paintEvent(QPaintEvent *event) override;
    void paintEvent1(QPaintEvent *event);

private slots:

    void on_But_Next_clicked();

    void on_But_Handpiece_clicked();

    void on_But_value_clicked();

signals:
    void sendvisibleisgnale(bool status);
    void activatemain();
private:
    Ui::tuning *ui;
    int m_value=0; // Holds the current value of the progress (0 to 100)
    int m_value1=0; // Holds the current value of the progress (0 to 100)
    int currentCircle;
    QTimer *timer;
     bool isRunning=false;
     QTimer *timer1;
      QColor circleColor; // New member variable for color
      MainWindow *main;
      hwhandler *hand;
      QTimer *handpiece;
      Vaccum *vacSensor;
      bool powerState;
        bool tuneCompleted = false;
};

#endif // TUNING_H
