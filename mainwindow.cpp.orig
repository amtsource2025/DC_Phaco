 #include "mainwindow.h"
#include "ui_mainwindow.h"
#include"footpedal.h"

#include <QTimer>
#include<cmath>
#include<QStyleFactory>
#include<QSqlError>
#include<QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , elapsedTimeUS1(0)
    , elapsedTimeUS2(0)
    , elapsedTimeUS3(0)
    , elapsedTimeUS4(0)
    , butname(-1)
    ,isTuneEnabled(false)
    ,powerOn1(false)
    ,overallci(false)
    ,us1poweron(false)
    ,us2poweron(false)
    ,us3poweron(false)
    ,us4poweron(false)
    ,vitonoff(false)


{
    ui->setupUi(this);//class initialisation
    vac=new Vaccum;
    handler=new hwhandler;
    foot=new footpedal;
    in=new doctor;
    //s=new settings;
    key=new keypad;
    footsensor=new QTimer;
    lfoot=new footlib;
    ui->label_32->hide();
    ui->CutMode_vit->hide();
    ui->CutMode_vitCom->hide();
    ui->tabWidget_2->tabBar()->hide();

    ui->tabWidget_2->hide();
  //dial update
   ui->dial_2->setStyleSheet("");
   ui->dial_2->setStyle(QStyleFactory::create("Fusion"));
   ui->dial_2->setStyleSheet("background-color: rgb(26, 95, 180);");
   ui->dial_2->setRange(0, 4096);
//last value updated
   setLastSelectedValue();

                        //timers
//elapsed timer
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateTimers);
    updateTimer->start(1000); // Update every 100 milliseconds

//footpedalcheck
    protimer=new QTimer;
    connect(protimer,&QTimer::timeout,this,&MainWindow::footpedalcheck);
    protimer->start(1);

//update sensor for linear
       sensortimer = new QTimer(this);
       connect(sensortimer, SIGNAL(timeout()), this, SLOT(sensor2()));
       sensortimer->start(100); // Update labels every 1000 milliseconds (1 second)
 Tacutalsensor=new QTimer;
 connect(Tacutalsensor,&QTimer::timeout,this,&MainWindow::readsensorvalue);
 Tacutalsensor->start(1);
//update handpiece status when handpiece is connected or not
   statusUpdateTimer = new QTimer(this);
   connect(statusUpdateTimer, &QTimer::timeout, this, &MainWindow::updatehandpieceStatus);
   statusUpdateTimer->start(500); // Update every second

//update sensor for nonlinear
 timerfoot= new QTimer;
  // connect(timerfoot, &QTimer::timeout, this, &MainWindow::linearvaccum);
 //  timerfoot->start(100); // 100 milliseconds interval

//continuous irrigation
  connect(ui->CI5_5,&QPushButton::clicked,this,&MainWindow::on_CI4_2_clicked);
//tabwidget setting
  ui->tabWidget->setCurrentIndex(7);

//connection for send values from the doctor window and receive values from the mainwindow
       connect(in,&doctor::sendValues,this,&MainWindow::receiveValues);
       connect(in,&doctor::transmitval,this,&MainWindow::footpedalvalues);
      // connect(in,&doctor::transmitval,this,&MainWindow::rx_defaultvalues);

       connect(in,&doctor::activatemainwindow,this,&MainWindow::disablefunction);
       QString styleSheet5 = "QLabel {"
                  "image: url(:/images/not_Tuned.png);"
                   "background-color:transparent;"
                                        "}";
//update footpedal value from doctor window to mainwindow and mainwindow to footpedal screen
      connect(this,&MainWindow::left_foot,foot,&footpedal::combobox1);
      connect(this,&MainWindow::right_foot,foot,&footpedal::combobox2);
      connect(this,&MainWindow::bottom_left,foot,&footpedal::combobox3);
      connect(this,&MainWindow::bottom_right,foot,&footpedal::combobox4);
//update surgeon name for footpedal
      connect(this,&MainWindow::sendsurgeon,foot,&footpedal::updateFootpedalComboBoxes1);
//update gpio pins and in and out
    exportGPIO(960);
    setGPIODirection("in",960);
//handpiece status
    updatehandpieceStatus();
//updatebutton status if  power on means then only increase and decrease button working
    ui->us1powup_but->setEnabled(false);
    ui->us1powdown_but->setEnabled(false);
    ui->us2powup_but->setEnabled(false);
    ui->us2powdown_but->setEnabled(false);
    ui->us3powup_but->setEnabled(false);
    ui->us3powdown_but->setEnabled(false);
    ui->us4powup_but->setEnabled(false);
    ui->us4powdown_but->setEnabled(false);
    ui->vitpowup_but->setEnabled(false);
    ui->vitpowdown_but->setEnabled(false);

//settext value in line edit like preset
//us1
   ui->lineEdit_57->setText("100");
   ui->lineEdit_55->setText("500");
   ui->lineEdit_56->setText("40");
   //us2
   ui->lineEdit_58->setText("100");
   ui->lineEdit_60->setText("500");
   ui->lineEdit_59->setText("40");
   //us3
   ui->lineEdit_61->setText("100");
   ui->lineEdit_63->setText("500");
   ui->lineEdit_62->setText("40");
   //us4
   ui->lineEdit_64->setText("100");
   ui->lineEdit_66->setText("500");
   ui->lineEdit_65->setText("40");
   //ia1
  // ui->lineEdit_58->setText("100");
   ui->lineEdit_70->setText("500");
   ui->lineEdit_69->setText("40");
   //ia2
   //ui->lineEdit_58->setText("100");
   ui->lineEdit_68->setText("500");
   ui->lineEdit_67->setText("40");
   //vit
   ui->lineEdit_71->setText("960");
   ui->lineEdit_73->setText("500");
   ui->lineEdit_72->setText("40");
   //dia
  ui->lineEdit_74->setText("100");

  // keypad connetion
    connect(key,&keypad::textsignal,this,&MainWindow::on_clicked);
    connect(key,&keypad::entersignal,this,&MainWindow::on_clickedenter);
 //footpedal connection receive value from the footpedal and updateing
    connect(foot, &footpedal::moveTopToBottom, this, &MainWindow::moved);
    connect(foot, &footpedal::moveBottomToTop, this, &MainWindow::movePushButtonBottomToTop);
   connect(foot,&footpedal::performReflux,this,&MainWindow::footreflux);
   connect(foot,&footpedal::togglePower,this,&MainWindow::poweronoff);
   connect(foot,&footpedal::continous_irrigation,this,&MainWindow::continousirrigation);
  connect(foot,&footpedal::powerdm,this,&MainWindow::onPdmModeSelected);
  connect(foot,&footpedal::powerdm1,this,&MainWindow::onPdmModeSelected1);
  connect(foot,&footpedal::powerdm2,this,&MainWindow::onPdmModeSelected2);
  connect(foot,&footpedal::powerdm3,this,&MainWindow::onPdmModeSelected3);
  connect(in,&doctor::pumpsignal,this,&MainWindow::performpump);
  connect(in,&doctor::tx_viberation,this,&MainWindow::rx_viberationvalue);
  connect(ui->comboBox,&QComboBox::currentTextChanged,this,&MainWindow::performpump);

//tabwidget current index selected
    ui->tabWidget->setTabText(6, "VITRECTOMY");
    QString tabStyle = "QTabBar::tab:selected { background-color: black; color: #ffffff; }";
//tab name
     ui-> tabWidget->setStyleSheet(tabStyle);
    ui->tabWidget->setTabText(0, "ULTRASONIC 1");
    ui->tabWidget->setTabText(1, "ULTRASONIC 2");
    ui->tabWidget->setTabText(2, "ULTRASONIC 3");
    ui->tabWidget->setTabText(3, "ULTRASONIC 4");
    ui->tabWidget->setTabText(4, "IRRIGATION ASPIRATION 1");
    ui->tabWidget->setTabText(5, "IRRIGATION ASPIRATION 2");
    ui->tabWidget->setTabText(7, "DIATHERMY");

//stylesheet
           QFont font("Ubuntu", 20, QFont::Bold);

// Set the font for the QPushButton
     ui->ULTRASONICBUT1->setFont(font);
     ui->ULTRASONICBUT2->setFont(font);
     ui->ULTRASONICBUT3->setFont(font);
     ui->ULTRASONICBUT4->setFont(font);
     ui->IA1BUT->setFont(font);
     ui->IA2BUT->setFont(font);
     ui->VITRECTOMYBUT->setFont(font);
     ui->DIABUT->setFont(font);

    //US1
    ui->lineEdit_57->installEventFilter(this);
    ui->lineEdit_55->installEventFilter(this);
    ui->lineEdit_56->installEventFilter(this);
    //US 2
     ui->lineEdit_58->installEventFilter(this);
     ui->lineEdit_60->installEventFilter(this);
     ui->lineEdit_59->installEventFilter(this);
     //US 3
     ui->lineEdit_61->installEventFilter(this);
     ui->lineEdit_63->installEventFilter(this);
     ui->lineEdit_62->installEventFilter(this);
     //US 4
     ui->lineEdit_64->installEventFilter(this);
     ui->lineEdit_66->installEventFilter(this);
     ui->lineEdit_65->installEventFilter(this);
     //IA1
     ui->lineEdit_70->installEventFilter(this);
     ui->lineEdit_69->installEventFilter(this);
     //IA 2
     ui->lineEdit_68->installEventFilter(this);
     ui->lineEdit_67->installEventFilter(this);
     //VIT
     ui->lineEdit_71->installEventFilter(this);
     ui->lineEdit_73->installEventFilter(this);
     ui->lineEdit_72->installEventFilter(this);
     //DIA
     ui->lineEdit_74->installEventFilter(this);
     ui->lineEdit_57->setMaxLength(3);

   //select surgeon which is updated
    connect(ui->comboBox_4, &QComboBox::currentTextChanged, this, &MainWindow::push);

    //pump update
     connect(ui->comboBox,&QComboBox::currentTextChanged,this,&MainWindow::performpump);

   //method combo box updated
     connect(ui->CutMode_vitCom, QOverload<const QString &>::of(&QComboBox::currentTextChanged), this, &MainWindow::updateTabsBasedOnComboBox);
      connect(ui->CutMode_vitCom_2, QOverload<const QString &>::of(&QComboBox::currentTextChanged), this, &MainWindow::updateTabsBasedOnComboBox);
      connect(ui->CutMode_vitCom_3, QOverload<const QString &>::of(&QComboBox::currentTextChanged), this, &MainWindow::updateTabsBasedOnComboBox);
    connect(ui->CutMode_vitCom_4, QOverload<const QString &>::of(&QComboBox::currentTextChanged), this, &MainWindow::updateTabsBasedOnComboBox);


    //stylesheet tab
   int currenttab=7;
     connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::current);
     current(currenttab);


     //name change connection
       connect(ui->ULTRASONICBUT1,&QPushButton::clicked,this,&MainWindow::ULTRASONICBUT1);
       connect(ui->ULTRASONICBUT2,&QPushButton::clicked,this,&MainWindow::ULTRASONICBUT2);
       connect(ui->ULTRASONICBUT3,&QPushButton::clicked,this,&MainWindow::ULTRASONICBUT3);
       connect(ui->ULTRASONICBUT4,&QPushButton::clicked,this,&MainWindow::ULTRASONICBUT4);
       connect(ui->IA1BUT,&QPushButton::clicked,this,&MainWindow::IRRIGATIONBUT1);
       connect(ui->IA2BUT,&QPushButton::clicked,this,&MainWindow::IRRIGATIONBUT2);
       connect(ui->VITRECTOMYBUT,&QPushButton::clicked,this,&MainWindow::VITRECTOMYBUT);
       connect(ui->DIABUT,&QPushButton::clicked,this,&MainWindow::DIATHERMYBUT);
       connect(ui->us1powup_but,&QPushButton::clicked,this,&MainWindow::US1POWUP);
       connect(ui->us1powdown_but,&QPushButton::clicked,this,&MainWindow::US1POWDOWN);
       connect(ui->us1vacdow_but,&QPushButton::clicked,this,&MainWindow::US1VACDOWN);
       connect(ui->us1vacup_but,&QPushButton::clicked,this,&MainWindow::US1VACUP);
       connect(ui->us1flowdown_but,&QPushButton::clicked,this,&MainWindow::US1FLOWDOWN);
       connect(ui->us1flowup_but,&QPushButton::clicked,this,&MainWindow::US1FLOWUP);
       connect(ui->BACKBUT,&QPushButton::clicked,this,&MainWindow::BACKBUT);

       //us2
       connect(ui->us2powup_but,&QPushButton::clicked,this,&MainWindow::us2powup);
       connect(ui->us2powdown_but,&QPushButton::clicked,this,&MainWindow::us2powdown);
       connect(ui->us2vacdown_but,&QPushButton::clicked,this,&MainWindow::us2vacdown);
       connect(ui->us2vacup_but,&QPushButton::clicked,this,&MainWindow::us2vacup);
       connect(ui->us2flowdown_but,&QPushButton::clicked,this,&MainWindow::us2flowdown);
       connect(ui->us2flowup_but,&QPushButton::clicked,this,&MainWindow::us2flowup);
       //us3
       connect(ui->us3powup_but,&QPushButton::clicked,this,&MainWindow::us3powup);
       connect(ui->us3powdown_but,&QPushButton::clicked,this,&MainWindow::us3powdown);
       connect(ui->us3vacdown_but,&QPushButton::clicked,this,&MainWindow::us3vacdown);
       connect(ui->us3vacup_but,&QPushButton::clicked,this,&MainWindow::us3vacup);
       connect(ui->us3flowdown_but,&QPushButton::clicked,this,&MainWindow::us3flowdown);
       connect(ui->us3flowup_but,&QPushButton::clicked,this,&MainWindow::us3flowup);
       //us4
       connect(ui->us4powup_but,&QPushButton::clicked,this,&MainWindow::us4powup);
       connect(ui->us4powdown_but,&QPushButton::clicked,this,&MainWindow::us4powdown);
       connect(ui->us4vacup_but,&QPushButton::clicked,this,&MainWindow::us4vacup);
       connect(ui->us4vacdown_but,&QPushButton::clicked,this,&MainWindow::us4vacdown);
       connect(ui->us4flowup_but,&QPushButton::clicked,this,&MainWindow::us4flowup);
       connect(ui->us4flowdown_but,&QPushButton::clicked,this,&MainWindow::us4flowdown);
       //ia1
       connect(ui->ia2vacup_but,&QPushButton::clicked,this,&MainWindow::ia1vacup);
       connect(ui->ia2vacdown_but,&QPushButton::clicked,this,&MainWindow::ia1vacdown);
       connect(ui->ia2flowup_but,&QPushButton::clicked,this,&MainWindow::ia1flowup);
       connect(ui->ia2flowdown_but,&QPushButton::clicked,this,&MainWindow::ia1flowdown);
       ui->ia1flowup_but->isEnabled();
       ui->ia1flowdown_but->isEnabled();
       ui->ia1vacup_but->isEnabled();
       ui->ia1vacdown_but->isEnabled();
       //ia2
       connect(ui->ia1vacup_but,&QPushButton::clicked,this,&MainWindow::ia2vacup);
       connect(ui->ia1vacdown_but,&QPushButton::clicked,this,&MainWindow::ia2vacdown);
       connect(ui->ia1flowup_but,&QPushButton::clicked,this,&MainWindow::ia2flowup);
       connect(ui->ia1flowdown_but,&QPushButton::clicked,this,&MainWindow::ia2flowdown);
       //vit
       connect(ui->vitpowup_but,&QPushButton::clicked,this,&MainWindow::vitcutup);
       connect(ui->vitpowdown_but,&QPushButton::clicked,this,&MainWindow::vitcutdown);
       connect(ui->vitvacup_but,&QPushButton::clicked,this,&MainWindow::vitvacup);
       connect(ui->vitvacdown_but,&QPushButton::clicked,this,&MainWindow::vitvacdown);
       connect(ui->vitflowup_but,&QPushButton::clicked,this,&MainWindow::vitflowup);
       connect(ui->vitflowdown_but,&QPushButton::clicked,this,&MainWindow::vitflowdown);
      //dia
       connect(ui->diapowup_but,&QPushButton::clicked,this,&MainWindow::diapowup);
       connect(ui->diapowdown_but,&QPushButton::clicked,this,&MainWindow::diapowdown);


      //mode
      connect(ui->us1mode,&QPushButton::clicked,this,&MainWindow::us1_linear_nonlinear);
      connect(ui->us2mode,&QPushButton::clicked,this,&MainWindow::us2_linear_nonlinear);
      connect(ui->us3mode,&QPushButton::clicked,this,&MainWindow::us3_linear_nonlinear);
      connect(ui->us4mode,&QPushButton::clicked,this,&MainWindow::us4_linear_nonlinear);
      connect(ui->ia1mode,&QPushButton::clicked,this,&MainWindow::ia1_linear_nonlinear);
      connect(ui->ia2mode,&QPushButton::clicked,this,&MainWindow::ia2_linear_nonlinear);
      connect(ui->vitmode,&QPushButton::clicked,this,&MainWindow::vit_linear_nonlinear);

      //window
      connect(ui->SETTINGS_BUT_3,&QPushButton::clicked,this,&MainWindow::footpedalwindow_show);

      //pdm mode
      connect(ui->pulseup_but,&QPushButton::clicked,this,&MainWindow::pulseup_mode);
      connect(ui->pulsedown_but,&QPushButton::clicked,this,&MainWindow::pulsedown_mode);
      //connect(ui->ocupulseup_but,&QPushButton::clicked,this,&MainWindow::ocupulseup_mode);
      //connect(ui->ocupulsedown_but,&QPushButton::clicked,this,&MainWindow::ocupulsedown_mode);
     // connect(ui->ocuburstup_but,&QPushButton::clicked,this,&MainWindow::ocuburstup_mode);
     // connect(ui->ocuburstdown_but,&QPushButton::clicked,this,&MainWindow::ocuburstdown_mode);
      connect(ui->singleburstup_but,&QPushButton::clicked,this,&MainWindow::singleburstup_mode);
      connect(ui->singleburstdown_but,&QPushButton::clicked,this,&MainWindow::singleburstdown_mode);
      connect(ui->multiburstup_but,&QPushButton::clicked,this,&MainWindow::multiburstup_mode);
      connect(ui->multiburstdown_but,&QPushButton::clicked,this,&MainWindow::multiburstdown_mode);
      connect(ui->coldphacoup_but,&QPushButton::clicked,this,&MainWindow::coldphacoup_mode);
      connect(ui->coldphacodown_but,&QPushButton::clicked,this,&MainWindow::coldphacodown_mode);
      connect(ui->coldphaco1up_but,&QPushButton::clicked,this,&MainWindow::coldphaco1up_mode);
      connect(ui->coldphaco1up_but,&QPushButton::clicked,this,&MainWindow::coldphaco1down_mode);
     //update mode linear or nonlinear
      us1_linear_nonlinear();
      us2_linear_nonlinear();
      us3_linear_nonlinear();
      us4_linear_nonlinear();
      ia1_linear_nonlinear();
      ia2_linear_nonlinear();
      vit_linear_nonlinear();
      on_us1vacmode_clicked();
      on_us2vacmode_clicked();
      on_us3vacmode_clicked();
      on_us4vacmode_clicked();
      on_vitvacmode_clicked();
//reterive value frm the sql;
      push(ui->comboBox_4->currentText());
      changesvaluesql();


ui->ULTRASONICBUT1->setEnabled(false);
ui->ULTRASONICBUT2->setEnabled(false);
ui->ULTRASONICBUT3->setEnabled(false);
ui->ULTRASONICBUT4->setEnabled(false);
ui->label_16->setStyleSheet(styleSheet5);
ui->label_17->setStyleSheet(styleSheet5);
ui->label_18->setStyleSheet(styleSheet5);
ui->label_27->setStyleSheet(styleSheet5);
}

//elapsed time
void MainWindow::updateElapsedTime()
{
    if (timer5.isValid()) {
        qint64 elapsed = timer5.elapsed();
        QString elapsedTimeStr = QString::number(elapsed) + " ms";
        ui->elapsed_time->setText(elapsedTimeStr);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    db.close();
}

//updateline edit when enter max value
void MainWindow::updateLineedit(QLineEdit *lineEdit, int prevValue, int value, int maxValue) {

    // Check if the entered value is greater than the maximum allowed value
       if (value > maxValue) {
           lineEdit->setText(QString::number(prevValue));
           messageline->setText(QString("Value must be between %1 and %2.").arg(prevValue).arg(maxValue));
           messageline->show();
           QMessageBox *msgBox = new QMessageBox(
               QMessageBox::Warning,
               "Invalid Input",
               QString("Value must be between %1 and %2.").arg(prevValue).arg(maxValue),
               QMessageBox::Ok,  // Ok button to close the message box
               nullptr           // Parent widget
           );
           msgBox->show();
           QTimer::singleShot(1000, msgBox, [msgBox]() {
               msgBox->hide();  // Hide the message box
               delete msgBox;   // Delete the message box to free memory
           });
           timermsg->start(3000);
       } else {
           lineEdit->setText(QString::number(value));
           if (messageline->isVisible()) {
               messageline->close();  // Close the message
               timermsg->stop();      // Stop the timer if it's running
           }
           prevValue = value;
       }
}

//keypad settings
bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
      if(object == ui->lineEdit_57 && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->resize(401,291);
            key->move(850,260);
            key->show();
            ui->lineEdit_57->setFocus();
            ui->lineEdit_55->clearFocus();
            ui->lineEdit_56->clearFocus();
            ui->lineEdit_57->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }
  }
      if(object == ui->lineEdit_55 && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->resize(401,291);
            key->move(100,260);
            key->show();
            ui->lineEdit_55->setFocus();
            ui->lineEdit_57->clearFocus();
            ui->lineEdit_56->clearFocus();
            ui->lineEdit_55->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }
  }

      if(object == ui->lineEdit_56 && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->resize(401,291);
            key->move(280,320);
            key->show();
            ui->lineEdit_56->setFocus();
            ui->lineEdit_57->clearFocus();
            ui->lineEdit_55->clearFocus();
            ui->lineEdit_56->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }
  }//ULTRASONIC 2
      if(object == ui->lineEdit_58 && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->resize(401,291);
         key->move(850,260);
            key->show();
            ui->lineEdit_58->setFocus();
            ui->lineEdit_60->clearFocus();
            ui->lineEdit_59->clearFocus();
            ui->lineEdit_58->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }
  }
      if(object == ui->lineEdit_60&& event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->resize(401,291);
            key->move(100,260);
            key->show();
            ui->lineEdit_60->setFocus();
            ui->lineEdit_58->clearFocus();
            ui->lineEdit_59->clearFocus();
            ui->lineEdit_60->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }
  }

      if(object == ui->lineEdit_59 && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->resize(401,291);
            key->move(280,320);
            key->show();
            ui->lineEdit_59->setFocus();
            ui->lineEdit_58->clearFocus();
            ui->lineEdit_60->clearFocus();
            ui->lineEdit_59->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }
  }
      //ULTRASONIC 3
      if(object == ui->lineEdit_61&& event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->resize(401,291);
         key->move(850,260);
            key->show();
            ui->lineEdit_61->setFocus();
            ui->lineEdit_63->clearFocus();
            ui->lineEdit_62->clearFocus();
            ui->lineEdit_61->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }
  }
      if(object == ui->lineEdit_63 && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->resize(401,291);
            key->move(100,260);
            key->show();
            ui->lineEdit_63->setFocus();
            ui->lineEdit_61->clearFocus();
            ui->lineEdit_62->clearFocus();
            ui->lineEdit_63->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }
  }

      if(object == ui->lineEdit_62 && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->resize(401,291);
            key->move(280,320);
            key->show();
            ui->lineEdit_62->setFocus();
            ui->lineEdit_61->clearFocus();
            ui->lineEdit_63->clearFocus();
            ui->lineEdit_62->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }
      }
        //ULTRASONIC 4
        if(object == ui->lineEdit_64 && event->type() == QEvent::MouseButtonPress) {
          QMouseEvent *k = static_cast<QMouseEvent *> (event);
          if( k->button() == Qt::LeftButton ) {
              key->resize(401,291);
           key->move(850,260);
              key->show();
              ui->lineEdit_64->setFocus();
              ui->lineEdit_66->clearFocus();
              ui->lineEdit_65->clearFocus();
              ui->lineEdit_64->setText("");

          } else if ( k->button() == Qt::RightButton ) {
          }
    }
        if(object == ui->lineEdit_66 && event->type() == QEvent::MouseButtonPress) {
          QMouseEvent *k = static_cast<QMouseEvent *> (event);
          if( k->button() == Qt::LeftButton ) {
              key->resize(401,291);
              key->move(100,260);
              key->show();
              ui->lineEdit_66->setFocus();
              ui->lineEdit_64->clearFocus();
              ui->lineEdit_65->clearFocus();
              ui->lineEdit_66->setText("");

          } else if ( k->button() == Qt::RightButton ) {
          }
    }

        if(object == ui->lineEdit_65 && event->type() == QEvent::MouseButtonPress) {
          QMouseEvent *k = static_cast<QMouseEvent *> (event);
          if( k->button() == Qt::LeftButton ) {
              key->resize(401,291);
              key->move(280,320);
              key->show();
              ui->lineEdit_65->setFocus();
              ui->lineEdit_64->clearFocus();
              ui->lineEdit_66->clearFocus();
              ui->lineEdit_65->setText("");

          } else if ( k->button() == Qt::RightButton ) {
          }
        }
          //IRRIGATION/ASPIRATION 1
        if(object == ui->lineEdit_70 && event->type() == QEvent::MouseButtonPress) {
          QMouseEvent *k = static_cast<QMouseEvent *> (event);
          if( k->button() == Qt::LeftButton ) {
              key->resize(401,291);
              key->move(100,260);
              key->show();
              ui->lineEdit_70->setFocus();
              ui->lineEdit_69->clearFocus();
             // ui->lineEdit_56->clearFocus();
              ui->lineEdit_70->setText("");

          } else if ( k->button() == Qt::RightButton ) {
          }
    }
        if(object == ui->lineEdit_69 && event->type() == QEvent::MouseButtonPress) {
          QMouseEvent *k = static_cast<QMouseEvent *> (event);
          if( k->button() == Qt::LeftButton ) {
              key->resize(401,291);
              key->move(280,320);
              key->show();
              ui->lineEdit_69->setFocus();
              ui->lineEdit_70->clearFocus();
              //ui->lineEdit_56->clearFocus();
              ui->lineEdit_69->setText("");

          } else if ( k->button() == Qt::RightButton ) {
          }
    }
        //IRRIGATION/ASPIRATION 2
        if(object == ui->lineEdit_68 && event->type() == QEvent::MouseButtonPress) {
          QMouseEvent *k = static_cast<QMouseEvent *> (event);
          if( k->button() == Qt::LeftButton ) {
              key->resize(401,291);
              key->move(100,260);
              key->show();
              ui->lineEdit_68->setFocus();
              ui->lineEdit_67->clearFocus();
              //ui->lineEdit_56->clearFocus();
              ui->lineEdit_68->setText("");

          } else if ( k->button() == Qt::RightButton ) {
          }
    }
        if(object == ui->lineEdit_67 && event->type() == QEvent::MouseButtonPress) {
          QMouseEvent *k = static_cast<QMouseEvent *> (event);
          if( k->button() == Qt::LeftButton ) {
              key->resize(401,291);
              key->move(280,320);
              key->show();
              ui->lineEdit_67->setFocus();
              ui->lineEdit_68->clearFocus();
              //ui->lineEdit_56->clearFocus();
              ui->lineEdit_67->setText("");

          } else if ( k->button() == Qt::RightButton ) {
          }
    }
        //VITRECTOMY

        if(object == ui->lineEdit_71 && event->type() == QEvent::MouseButtonPress) {
          QMouseEvent *k = static_cast<QMouseEvent *> (event);
          if( k->button() == Qt::LeftButton ) {
              key->close();
              key->resize(401,291);
             key->move(850,260);
              key->show();
              ui->lineEdit_71->setFocus();
              ui->lineEdit_73->clearFocus();
              ui->lineEdit_72->clearFocus();
              ui->lineEdit_71->setText("");

          } else if ( k->button() == Qt::RightButton ) {
          }
    }
        if(object == ui->lineEdit_73 && event->type() == QEvent::MouseButtonPress) {
          QMouseEvent *k = static_cast<QMouseEvent *> (event);
          if( k->button() == Qt::LeftButton ) {
               key->close();
              key->resize(401,291);
              key->move(100,260);
              key->show();
              ui->lineEdit_73->setFocus();
              ui->lineEdit_72->clearFocus();
              ui->lineEdit_71->clearFocus();
              ui->lineEdit_73->setText("");

          } else if ( k->button() == Qt::RightButton ) {
          }
    }
        if(object == ui->lineEdit_72 && event->type() == QEvent::MouseButtonPress) {
          QMouseEvent *k = static_cast<QMouseEvent *> (event);
          if( k->button() == Qt::LeftButton ) {
               key->close();
              key->resize(401,291);
              key->move(280,320);
              key->show();
              ui->lineEdit_72->setFocus();
              ui->lineEdit_73->clearFocus();
              ui->lineEdit_71->clearFocus();
              ui->lineEdit_72->setText("");

          } else if ( k->button() == Qt::RightButton ) {
          }
    }
  //DIATHERMY
        if(object == ui->lineEdit_74 && event->type() == QEvent::MouseButtonPress) {
          QMouseEvent *k = static_cast<QMouseEvent *> (event);
          if( k->button() == Qt::LeftButton ) {
              key->close();
              key->resize(401,291);
              key->move(280,320);
              key->show();
              ui->lineEdit_74->setFocus();
              ui->lineEdit_74->setText("");
          } else if ( k->button() == Qt::RightButton ) {
          }
    }
  return false;
}
//keypad entered value
void MainWindow::on_clicked(const QString& digit)
{//ULTRASONIC 1

  if(ui->lineEdit_57->focusWidget()) {
      ui->lineEdit_55->clearFocus();
      ui->lineEdit_56->clearFocus();
      ui->lineEdit_57->setFocus();
      int prevValue = ui->lineEdit_57->text().toInt();
      int value = (ui->lineEdit_57->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_57->setText(QString::number(5));
          return;}
          setRange(ui->lineEdit_57, prevValue, value, 100);
         // updateLineedit(ui->lineEdit_57,prevValue,value,100);
  }
  if(ui->lineEdit_55->focusWidget()) {
      ui->lineEdit_56->clearFocus();
      ui->lineEdit_57->clearFocus();
      ui->lineEdit_55->setFocus();
      int prevValue = ui->lineEdit_55->text().toInt();
      int value = (ui->lineEdit_55->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_55->setText(QString::number(5));
          return;}
      setRange(ui->lineEdit_55, prevValue, value, 500);
    //  updateLineedit(ui->lineEdit_55,prevValue,value,500);


  }
  if(ui->lineEdit_56->focusWidget()) {
      ui->lineEdit_55->clearFocus();
      ui->lineEdit_57->clearFocus();
      ui->lineEdit_56->setFocus();
      int prevValue = ui->lineEdit_56->text().toInt();
      int value = (ui->lineEdit_56->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_56->setText(QString::number(2));
          return;}
      setRange(ui->lineEdit_56, prevValue, value, 40);
     // updateLineedit(ui->lineEdit_56,prevValue,value,40);

  }
  //ULTRASONIC 2
   if(ui->lineEdit_58->focusWidget()) {
      ui->lineEdit_60->clearFocus();
      ui->lineEdit_59->clearFocus();
      ui->lineEdit_58->setFocus();
      int prevValue = ui->lineEdit_58->text().toInt();
      int value = (ui->lineEdit_58->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_58->setText(QString::number(5));
          return;}
      setRange(ui->lineEdit_58, prevValue, value, 100);
    //  updateLineedit(ui->lineEdit_58,prevValue,value,100);

  }
   if(ui->lineEdit_60->focusWidget()) {
      ui->lineEdit_58->clearFocus();
      ui->lineEdit_59->clearFocus();
      ui->lineEdit_60->setFocus();
      int prevValue = ui->lineEdit_60->text().toInt();
      int value =( ui->lineEdit_60->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_60->setText(QString::number(5));
          return;}
      setRange(ui->lineEdit_60, prevValue, value, 500);
     // updateLineedit(ui->lineEdit_60,prevValue,value,500);

  }
   if(ui->lineEdit_59->focusWidget()) {
      ui->lineEdit_58->clearFocus();
      ui->lineEdit_60->clearFocus();
      ui->lineEdit_59->setFocus();
      int prevValue = ui->lineEdit_59->text().toInt();
      int value = (ui->lineEdit_59->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_59->setText(QString::number(2));
          return;}
      setRange(ui->lineEdit_59, prevValue, value, 40);
     // updateLineedit(ui->lineEdit_59,prevValue,value,40);

  }
  //ULTRASONIC 3
   if(ui->lineEdit_61->focusWidget()) {
      ui->lineEdit_63->clearFocus();
      ui->lineEdit_62->clearFocus();
      ui->lineEdit_61->setFocus();
      int prevValue= ui->lineEdit_61->text().toInt();
      int value = (ui->lineEdit_61->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_61->setText(QString::number(5));
          return;}
      setRange(ui->lineEdit_61, prevValue, value, 100);

  }
  if(ui->lineEdit_63->focusWidget()) {
      ui->lineEdit_61->clearFocus();
      ui->lineEdit_62->clearFocus();
      ui->lineEdit_63->setFocus();
      int prevValue = ui->lineEdit_63->text().toInt();
      int value = (ui->lineEdit_63->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_63->setText(QString::number(5));
          return;}
      setRange(ui->lineEdit_63, prevValue, value, 500);

  }
 else if(ui->lineEdit_62->focusWidget()) {
      ui->lineEdit_61->clearFocus();
      ui->lineEdit_63->clearFocus();
      ui->lineEdit_62->setFocus();
      int prevValue = ui->lineEdit_62->text().toInt();
      int value = (ui->lineEdit_62->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_62->setText(QString::number(2));
          return;}
      setRange(ui->lineEdit_62, prevValue, value, 40);

  }
  //ULTRASONIC 4
   if(ui->lineEdit_64->focusWidget()) {
      ui->lineEdit_66->clearFocus();
      ui->lineEdit_65->clearFocus();
      ui->lineEdit_64->setFocus();
      int prevValue = ui->lineEdit_64->text().toInt();
      int value = (ui->lineEdit_64->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_64->setText(QString::number(5));
          return;}
      setRange(ui->lineEdit_64, prevValue, value, 100);



  }
  if(ui->lineEdit_66->focusWidget()) {
      ui->lineEdit_65->clearFocus();
      ui->lineEdit_64->clearFocus();
      ui->lineEdit_66->setFocus();
      int prevValue = ui->lineEdit_66->text().toInt();
      int value = (ui->lineEdit_66->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_66->setText(QString::number(5));
          return;}
      setRange(ui->lineEdit_66, prevValue, value, 500);

  }
  if(ui->lineEdit_65->focusWidget()) {
      ui->lineEdit_64->clearFocus();
      ui->lineEdit_66->clearFocus();
      ui->lineEdit_65->setFocus();
      int prevValue = ui->lineEdit_65->text().toInt();
      int value = (ui->lineEdit_65->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_65->setText(QString::number(2));
          return;}
      setRange(ui->lineEdit_65, prevValue, value, 40);

   }
  //IRRIGATION/ASPIRATION 1
  if(ui->lineEdit_70->focusWidget()) {
      ui->lineEdit_69->clearFocus();
      //ui->lineEdit_56->clearFocus();
      ui->lineEdit_70->setFocus();
      int prevValue = ui->lineEdit_70->text().toInt();
      int value = (ui->lineEdit_70->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_70->setText(QString::number(5));
          return;}
      setRange(ui->lineEdit_70, prevValue, value, 500);

  }
  if(ui->lineEdit_69->focusWidget()) {
      ui->lineEdit_70->clearFocus();
      //ui->lineEdit_57->clearFocus();
      ui->lineEdit_69->setFocus();
      int prevValue = ui->lineEdit_69->text().toInt();
      int value = (ui->lineEdit_69->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_69->setText(QString::number(2));
          return;}
      setRange(ui->lineEdit_69, prevValue, value, 40);

  }
  //IRRIGATION/ASPIRATION 2
   if(ui->lineEdit_68->focusWidget()) {
      ui->lineEdit_67->clearFocus();
      //ui->lineEdit_56->clearFocus();
      ui->lineEdit_68->setFocus();
      int prevValue = ui->lineEdit_68->text().toInt();
      int value = (ui->lineEdit_68->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_68->setText(QString::number(5));
          return;}
      setRange(ui->lineEdit_68, prevValue, value, 500);


  }
   if(ui->lineEdit_67->focusWidget()) {
      ui->lineEdit_68->clearFocus();
      //ui->lineEdit_57->clearFocus();
      ui->lineEdit_67->setFocus();
      int prevValue = ui->lineEdit_67->text().toInt();
      int value = (ui->lineEdit_67->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_67->setText(QString::number(2));
          return;}
      setRange(ui->lineEdit_67, prevValue, value, 40);


  }
  //VITRECTOMY
   if(ui->lineEdit_71->focusWidget()) {
      ui->lineEdit_73->clearFocus();
      ui->lineEdit_72->clearFocus();
      ui->lineEdit_71->setFocus();
      int prevValue = ui->lineEdit_71->text().toInt();
      int value = (ui->lineEdit_71->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_71->setText(QString::number(5));
          return;}
      setRange(ui->lineEdit_71, prevValue, value, 960);

  }
   if(ui->lineEdit_73->focusWidget()) {
      ui->lineEdit_71->clearFocus();
      ui->lineEdit_72->clearFocus();
      ui->lineEdit_73->setFocus();
      int prevValue = ui->lineEdit_73->text().toInt();
      int value = (ui->lineEdit_73->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_73->setText(QString::number(5));
          return;}
      setRange(ui->lineEdit_73, prevValue, value, 500);


  }
  if(ui->lineEdit_72->focusWidget()) {
      ui->lineEdit_71->clearFocus();
      ui->lineEdit_73->clearFocus();
      ui->lineEdit_72->setFocus();
      int prevValue = ui->lineEdit_72->text().toInt();
      int value = (ui->lineEdit_72->text()+digit).toInt();
      if(value==0){
          ui->lineEdit_72->setText(QString::number(2));
          return;}
      setRange(ui->lineEdit_72, prevValue, value, 40);

  }
  //DIATHERMY
  if (ui->lineEdit_74->focusWidget()) {
      ui->lineEdit_74->setFocus();
      int prevValue = ui->lineEdit_74->text().toInt();

      int value = (ui->lineEdit_74->text() + digit).toInt();
      if(value==0){
          ui->lineEdit_74->setText(QString::number(5));
          return;}
          setRange(ui->lineEdit_74, prevValue, value, 100);

      }

}
//when enter button is clicked they value is modified
void MainWindow::on_clickedenter()
{
   key->hide();
   // Maintain previous value for lineEdit
   QString text = ui->lineEdit_72->text().trimmed();
   if (!text.isEmpty()) {
       int input = text.toInt();
       int output = getvalue(input);  // Process the input to get the desired output
       ui->lineEdit_72->setText(QString::number(output));
       lastValidValues[ui->lineEdit_72] = output;  // Store the processed output as the last valid value
   } else {
       // Restore the last valid value if input is empty
       if (lastValidValues.contains(ui->lineEdit_72)) {
           ui->lineEdit_72->setText(QString::number(lastValidValues[ui->lineEdit_72]));
       }
   }

   // Process other QLineEdits for power and vacuum
   QList<QLineEdit*> lineEdits = {
       ui->lineEdit_57, ui->lineEdit_55, ui->lineEdit_58, ui->lineEdit_60,
       ui->lineEdit_61, ui->lineEdit_63, ui->lineEdit_64, ui->lineEdit_66,
       ui->lineEdit_70, ui->lineEdit_68, ui->lineEdit_73,ui->lineEdit_74
   };

   for (QLineEdit* lineEdit : lineEdits) {
       QString currentText = lineEdit->text().trimmed();

       if (!currentText.isEmpty()) {
           int input = currentText.toInt();
           int output = increasebutton(input);  // Process the input to get the desired output
           lineEdit->setText(QString::number(output));
           lastValidValues[lineEdit] = output;  // Update the last valid value
       } else {
           // Restore the last valid value if input is empty
           if (lastValidValues.contains(lineEdit)) {
               lineEdit->setText(QString::number(lastValidValues[lineEdit]));
           }
       }
   }

   // Process other QLineEdits for flow rate
   QList<QLineEdit*> lineEdits1 = {
       ui->lineEdit_56, ui->lineEdit_59, ui->lineEdit_62,
       ui->lineEdit_65, ui->lineEdit_69, ui->lineEdit_67, ui->lineEdit_72
   };

   for (QLineEdit* lineEdit1 : lineEdits1) {
       QString currentText1 = lineEdit1->text().trimmed();

       if (!currentText1.isEmpty()) {
           int input1 = currentText1.toInt();
           int output1 = decreasebutton(input1);  // Process the input to get the desired output
           lineEdit1->setText(QString::number(output1));
           lastValidValues1[lineEdit1] = output1;  // Update the last valid value
       } else {
           // Restore the last valid value if input is empty
           if (lastValidValues1.contains(lineEdit1)) {
               lineEdit1->setText(QString::number(lastValidValues1[lineEdit1]));
           }
       }
   }

}

void MainWindow::ULTRASONICBUT1()
{
    ui->tabWidget->setCurrentIndex(0);
    QString currentText = ui->CutMode_vitCom->currentText();
    updateTabsBasedOnComboBox(ui->CutMode_vitCom->currentText());
    ui->CutMode_vit->show();
    ui->CutMode_vitCom->show();
    ui->tabWidget_2->show();
    butname=1;
    handler->buzz();
    us1PdmMode = true;
       us2PdmMode = false;
       us3PdmMode = false;
       us4PdmMode = false;

}



void MainWindow::footpedalvalues(int &value1, int &value2, int &value3, int &value4)
{
qDebug()<<"values are received from the doctor window is"<<value1<<value2<<value3<<value4;
    nfpzero = static_cast<int>(value1*40.90);
    nfpone =  static_cast<int>(value2*40.90);
    nfptwo =  static_cast<int>(value3*40.90);
    nfpthree =  static_cast<int>(value4*40.90);


    qDebug()<<"that is the range"<<nfpzero<<nfpone<<nfptwo<<nfpthree;
footpedalcheck();

}

void MainWindow::rx_defaultvalues(int &value1, int &value2, int &value3, int &value4)
{
    qDebug()<<"values are received from the doctor window is"<<value1<<value2<<value3<<value4;
        nfpzero = static_cast<int>(value1/40.90);
        nfpone =  static_cast<int>(value2/40.90);
        nfptwo =  static_cast<int>(value3/40.90);
        nfpthree =  static_cast<int>(value4/40.90);


        qDebug()<<"that is the range"<<nfpzero<<nfpone<<nfptwo<<nfpthree;
    footpedalcheck();
}



void MainWindow::disablefunction()
{
   protimer->start(1);
   Tacutalsensor->start(1);

}


void MainWindow::ULTRASONICBUT2()
{
    ui->tabWidget->setCurrentIndex(1);
    QString currentText = ui->CutMode_vitCom_2->currentText();
    updateTabsBasedOnComboBox(currentText);

    ui->CutMode_vit->show();
    ui->CutMode_vitCom->show();
    ui->tabWidget_2->show();
    butname=2;
  handler->buzz();
  us1PdmMode = false;
     us2PdmMode = true;
     us3PdmMode = false;
     us4PdmMode = false;

}
void MainWindow::ULTRASONICBUT3()
{
    ui->tabWidget->setCurrentIndex(2);
      QString currentText = ui->CutMode_vitCom_3->currentText();

      updateTabsBasedOnComboBox(currentText);

    ui->CutMode_vit->show();
    ui->CutMode_vitCom->show();
    ui->tabWidget_2->show();
    butname=3;
    us1PdmMode = false;
       us2PdmMode = false;
       us3PdmMode = true;
       us4PdmMode = false;

}

void MainWindow::ULTRASONICBUT4()
{

    ui->tabWidget->setCurrentIndex(3);
    QString currentText = ui->CutMode_vitCom_4->currentText();
    updateTabsBasedOnComboBox(currentText);
     ui->CutMode_vit->show();
     ui->CutMode_vitCom->show();
     ui->tabWidget_2->show();
     butname=4;
     handler->buzz();
     us1PdmMode = false;
        us2PdmMode = false;
        us3PdmMode = false;
        us4PdmMode = true;

  }

void MainWindow::IRRIGATIONBUT1()
{
    ui->tabWidget->setCurrentIndex(4);
     ui->CutMode_vit->hide();
     ui->CutMode_vitCom->hide();
     ui->tabWidget_2->hide();
     butname=5;
     handler->buzz();
     ui->label_32->hide();
     ui->tabWidget_2->hide();

 }

void MainWindow::IRRIGATIONBUT2()
{
    ui->tabWidget->setCurrentIndex(5);
     ui->CutMode_vit->hide();
     ui->CutMode_vitCom->hide();
     ui->tabWidget_2->hide();
     butname=6;
     handler->buzz();
     ui->label_32->hide();
     ui->tabWidget_2->hide();
   }

void MainWindow::VITRECTOMYBUT()
{
    ui->tabWidget->setCurrentIndex(6);
    butname=7;
    handler->buzz();
    ui->label_32->hide();
    ui->tabWidget_2->hide();

}

void MainWindow::DIATHERMYBUT()
{
    handler->buzz();

  ui->tabWidget->setCurrentIndex(7);
    ui->CutMode_vit->hide();
    ui->CutMode_vitCom->hide();
    ui->tabWidget_2->hide();
    ui->label_32->hide();
    ui->tabWidget_2->hide();
    butname=0;

    current(7);

}
void MainWindow::diapowup()
{
    handler->buzz();
    int value=ui->lineEdit_74->text().toInt();
    value=value+5;

    if(value > 100)
    {
        value=100;

    }
    ui->lineEdit_74->setText(QString::number(value));

}

void MainWindow::diapowdown()
{
     handler->buzz();
    int value=ui->lineEdit_74->text().toInt();
    value=value-5;
    if(value < 5)
    {
        value=5;
    }
    ui->lineEdit_74->setText(QString::number(value));

}

void MainWindow::vitcutup()
{
     handler->buzz();
   int value=ui->lineEdit_71->text().toInt();
    value=value+60;
     if(value > 960)
    {
        value=960;
    }
    ui->lineEdit_71->setText(QString::number(value));


}

void MainWindow::vitcutdown()
{
     handler->buzz();
    int value=ui->lineEdit_71->text().toInt();
    value=value-60;
    if(value < 60)
    {
        value=60;
    }
    ui->lineEdit_71->setText(QString::number(value));

}

void MainWindow::vitvacup()
{
     handler->buzz();
    int value=ui->lineEdit_73->text().toInt();
    value=value+5;

    if(value > 500)
    {
        value=500;
    }

    ui->lineEdit_73->setText(QString::number(value));

}

void MainWindow::vitvacdown()
{
     handler->buzz();
    int value=ui->lineEdit_73->text().toInt();
    value=value-5;
    if(value < 5)
    {
        value=5;
    }

    ui->lineEdit_73->setText(QString::number(value));
}

void MainWindow::vitflowup()
{
     handler->buzz();
   int value=ui->lineEdit_72->text().toInt();
    value=value+2;

    if(value > 40)
    {
        value=40;
    }
    ui->lineEdit_72->setText(QString::number(value));


}

void MainWindow::vitflowdown()
{
     handler->buzz();
   int value=ui->lineEdit_72->text().toInt();
    value=value-2;
    if(value < 2)
    {
        value=2;
    }
    ui->lineEdit_72->setText(QString::number(value));

}

void MainWindow::ia2vacup()
{
     handler->buzz();
   int  value=ui->lineEdit_68->text().toInt();
    value=value+5;
    if(value > 500)
    {
        value=500;
    }
    ui->lineEdit_68->setText(QString::number(value));


}

void MainWindow::ia2vacdown()
{
     handler->buzz();
   int  value=ui->lineEdit_68->text().toInt();
    value=value-5;
    if(value < 5)
    {
        value=5;
    }
    ui->lineEdit_68->setText(QString::number(value));

}

void MainWindow::ia2flowup()
{
     handler->buzz();
  int value=ui->lineEdit_67->text().toInt();
    value=value+2;
    if(value > 40)
    {
        value=40;
    }
    ui->lineEdit_67->setText(QString::number(value));

}

void MainWindow::ia2flowdown()
{
     handler->buzz();
    int value=ui->lineEdit_67->text().toInt();
    value=value-2;
    if(value < 2)
    {
        value=2;
    }
    ui->lineEdit_67->setText(QString::number(value));
}

void MainWindow::ia1vacup()
{
     handler->buzz();
   int value=ui->lineEdit_70->text().toInt();
    value=value+5;
    if(value > 500)
    {
        value=500;
    }
    ui->lineEdit_70->setText(QString::number(value));
}

void MainWindow::ia1vacdown()
{
     handler->buzz();
   int value=ui->lineEdit_70->text().toInt();
    value=value-5;
    if(value < 5)
    {
        value=5;
    }
    ui->lineEdit_70->setText(QString::number(value));
}

void MainWindow::ia1flowup()
{
     handler->buzz();
   int value=ui->lineEdit_69->text().toInt();
    value=value+2;
    if(value > 40)
    {
        value=40;
    }
    ui->lineEdit_69->setText(QString::number(value));
}

void MainWindow::ia1flowdown()
{
     handler->buzz();
   int value=ui->lineEdit_69->text().toInt();
    value=value-2;
    if(value < 2)
    {
        value=2;
    }
    ui->lineEdit_69->setText(QString::number(value));
}

void MainWindow::US1POWUP()
{
     handler->buzz();
   int value=ui->lineEdit_57->text().toInt();
    value=value+5;
    if(value > 100)
    {
        value=100;
    }
    ui->lineEdit_57->setText(QString::number(value));
    handler->phaco_power(value);

}

void MainWindow::US1POWDOWN()
{
     handler->buzz();
  int  value=ui->lineEdit_57->text().toInt();
    value=value-5;
    if(value < 5)
    {
        value=5;
    }
    ui->lineEdit_57->setText(QString::number(value));
    handler->phaco_power(value);
}

void MainWindow::US1VACUP()
{
     handler->buzz();
    int value=ui->lineEdit_55->text().toInt();
    value=value+5;
      if(value > 500)
    {
        value=500;
    }
      ui->lineEdit_55->setText(QString::number(value));
 }

void MainWindow::US1VACDOWN()
{
     handler->buzz();
    int value=ui->lineEdit_55->text().toInt();
    value=value-5;

    if(value < 5)
    {
        value=5;
    }

    ui->lineEdit_55->setText(QString::number(value));
}

void MainWindow::US1FLOWUP()
{
     handler->buzz();
   int value=ui->lineEdit_56->text().toInt();
    value=value+2;
       if(value > 40)
    {
        value=40;
    }

    ui->lineEdit_56->setText(QString::number(value));
   }

void MainWindow::US1FLOWDOWN()
{
     handler->buzz();
   int value=ui->lineEdit_56->text().toInt();
    value=value-2;
    if(value < 2)
    {
        value=2;
    }

    ui->lineEdit_56->setText(QString::number(value));
}


void MainWindow::us2powup()
{
     handler->buzz();
   int value=ui->lineEdit_58->text().toInt();
    value=value+5;

    if(value > 100)
    {
        value=100;
    }

    ui->lineEdit_58->setText(QString::number(value));
    handler->phaco_power(value);

}

void MainWindow::us2powdown()
{
     handler->buzz();
  int  value=ui->lineEdit_58->text().toInt();
    value=value-5;
    if(value < 5)
    {
        value=5;
    }

    ui->lineEdit_58->setText(QString::number(value));
    handler->phaco_power(value);



}

void MainWindow::us2vacup()
{
     handler->buzz();
   int value=ui->lineEdit_60->text().toInt();
    value=value+5;

    if(value > 500)
    {
        value=500;
    }

    ui->lineEdit_60->setText(QString::number(value));


}

void MainWindow::us2vacdown()
{
     handler->buzz();
   int value=ui->lineEdit_60->text().toInt();
    value=value-5;
    if(value < 5)
    {
        value=5;
    }

    ui->lineEdit_60->setText(QString::number(value));

}

void MainWindow::us2flowup()
{
     handler->buzz();
  int  value=ui->lineEdit_59->text().toInt();
    value=value+2;

    if(value > 40)
    {
        value=40;
    }

    ui->lineEdit_59->setText(QString::number(value));

}

void MainWindow::us2flowdown()
{
     handler->buzz();
   int value=ui->lineEdit_59->text().toInt();
    value=value-2;
    if(value < 2)
    {
        value=2;
    }

    ui->lineEdit_59->setText(QString::number(value));
}


void MainWindow::us3powup()
{
     handler->buzz();
   int value=ui->lineEdit_61->text().toInt();
    value=value+5;

    if(value > 100)
    {
        value=100;
    }

    ui->lineEdit_61->setText(QString::number(value));
    handler->phaco_power(value);

}

void MainWindow::us3powdown()
{
     handler->buzz();
   int value=ui->lineEdit_61->text().toInt();
    value=value-5;
    if(value < 5)
    {
        value=5;
    }

    ui->lineEdit_61->setText(QString::number(value));
    handler->phaco_power(value);

}

void MainWindow::us3vacup()
{
     handler->buzz();
   int value=ui->lineEdit_63->text().toInt();
    value=value+5;

    if(value > 500)
    {
        value=500;
    }

    ui->lineEdit_63->setText(QString::number(value));
    }

void MainWindow::us3vacdown()
{
     handler->buzz();
   int value=ui->lineEdit_63->text().toInt();
    value=value-5;
    if(value < 5)
    {
        value=5;
    }

    ui->lineEdit_63->setText(QString::number(value));
}

void MainWindow::us3flowup()
{
     handler->buzz();
   int value=ui->lineEdit_62->text().toInt();
    value=value+2;
    if(value > 40)
    {
        value=40;
    }
    ui->lineEdit_62->setText(QString::number(value));
}

void MainWindow::us3flowdown()
{
     handler->buzz();
   int value=ui->lineEdit_62->text().toInt();
    value=value-2;
    if(value < 2)
    {
        value=2;
    }
    ui->lineEdit_62->setText(QString::number(value));
}

void MainWindow::us4powup()
{
     handler->buzz();
   int value=ui->lineEdit_64->text().toInt();
    value=value+5;
    if(value > 100)
    {
        value=100;
    }
    ui->lineEdit_64->setText(QString::number(value));
    handler->phaco_power(value);

}

void MainWindow::us4powdown()
{
     handler->buzz();
   int value=ui->lineEdit_64->text().toInt();
    value=value-5;
    if(value < 5)
    {
        value=5;
    }
    ui->lineEdit_64->setText(QString::number(value));
    handler->phaco_power(value);

}

void MainWindow::us4vacup()
{
     handler->buzz();
   int value=ui->lineEdit_66->text().toInt();
    value=value+5;
    if(value > 500)
    {
        value=500;
    }
    ui->lineEdit_66->setText(QString::number(value));

}

void MainWindow::us4vacdown()
{
     handler->buzz();
   int value=ui->lineEdit_66->text().toInt();
    value=value-5;
    if(value < 5)
    {
        value=5;
    }
    ui->lineEdit_66->setText(QString::number(value));
}

void MainWindow::us4flowup()
{
     handler->buzz();
   int value=ui->lineEdit_65->text().toInt();
    value=value+2;
    if(value > 40)
    {
        value=40;
    }
    ui->lineEdit_65->setText(QString::number(value));

}

void MainWindow::us4flowdown()
{
     handler->buzz();
  int  value=ui->lineEdit_65->text().toInt();
    value=value-2;
    if(value < 2)
    {
        value=2;
    }
    ui->lineEdit_65->setText(QString::number(value));
}

void MainWindow::setRange(QLineEdit* lineEdit, int prevValue, int value, int maxValue)
{
   if(value > maxValue)
   {
       lineEdit->setText(QString::number(prevValue));
   }
   else
   {
       lineEdit->setText(QString::number(value));
   }
}
void MainWindow::BACKBUT()
{
    handler->buzz();

    motoroff();
    handler->phaco_off();
    handler->fs_count(0);
    handler->freq_count(0);
    handler->phaco_power(0);
    handler->pinchvalve_off();
    handler->safetyvent_off();
    handler->vit_off();
    protimer->stop();
    Tacutalsensor->stop();
    this->close();
}
void MainWindow::current(int tab)
{
    QString styleSheet = "QPushButton {"
                         "    font-family: Ubuntu;"
                         "    font-size: 15pt;"
                         "    background-color: transparent;"
            "image: url(:/images/pushbutton.png);"
            "height:91;"
            "width:91;"
                         "    color: white;"
                         "    border-radius: 40px;"
                         "}";
    QString styleSheet1 = "QPushButton {"
                          "    font-family: Ubuntu;"
                          "    font-size: 15pt;"
                          "    background-color: transparent;"
            "image: url(:/images/glass.png);"
            "height:81;"
            "width:81;"
                          "    color: black;"
                          "    border-radius: 40px;"
                          "}";
    //locked and unlocked
    QString styleSheet3 = "QPushButton {"
                          "    font-family: Ubuntu;"
                          "    font-size: 13pt;"
                          "    background-color: transparent;"
            "height:81;"
            "width:81;"
                          "image: url(:/images/newbut.png);"
                          "    border-radius: 40px;"
                          "}";
    QString styleSheet4 = "QPushButton {"
                          "    font-family: Ubuntu;"
                          "    font-size: 13pt;"
                          "    background-color: transparent;"
            "height:81;"
            "width:81;"
            "image: url(:/images/irr.png);"
                          "    color: black;"
                          "    border-radius: 40px;"
                          "}";

    // Reset all buttons' styles to default (styleSheet1)
    ui->ULTRASONICBUT1->setStyleSheet(styleSheet1);
    ui->ULTRASONICBUT2->setStyleSheet(styleSheet1);
    ui->ULTRASONICBUT3->setStyleSheet(styleSheet1);
    ui->ULTRASONICBUT4->setStyleSheet(styleSheet1);
    ui->IA1BUT->setStyleSheet(styleSheet1);
    ui->IA2BUT->setStyleSheet(styleSheet1);
    ui->VITRECTOMYBUT->setStyleSheet(styleSheet1);
    ui->DIABUT->setStyleSheet(styleSheet1);
    ui->label_11->hide();
    ui->label_12->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_14->hide();
    ui->label_13->hide();
    ui->label_5->hide();
    ui->label_6->hide();
    ui->elapsed_time->hide();
    switch (tab) {
        case 0:
            ui->ULTRASONICBUT1->setStyleSheet(styleSheet);
            ui->label_3->show();
            ui->elapsed_time->show();
            ui->label_32->show();

            break;
        case 1:
            ui->ULTRASONICBUT2->setStyleSheet(styleSheet);
            ui->label_4->show();
            ui->elapsed_time->show();
             ui->label_32->show();
            break;
        case 2:
            ui->ULTRASONICBUT3->setStyleSheet(styleSheet);
            ui->label_14->show();
            ui->elapsed_time->show();
             ui->label_32->show();
            break;
        case 3:
            ui->ULTRASONICBUT4->setStyleSheet(styleSheet);
            ui->elapsed_time->show();
            ui->label_12->show();
            ui->label_32->show();
            break;
        case 4:
            ui->IA1BUT->setStyleSheet(styleSheet);
            ui->label_13->show();
            ui->label_32->hide();

            break;
        case 5:
            ui->IA2BUT->setStyleSheet(styleSheet);
            ui->label_5->show();
            ui->label_32->hide();

            break;
        case 6:
            ui->VITRECTOMYBUT->setStyleSheet(styleSheet);
            ui->label_6->show();
            ui->label_32->hide();

            break;
        case 7:
            ui->DIABUT->setStyleSheet(styleSheet);
            ui->label_11->show();
            ui->label_32->hide();

            break;
        default:
            break;
    }


}

//if tune is completed then only
void MainWindow::setTuneMode() {
    QString styleSheet4 = "QLabel {"
               "image: url(:/images/Tuned.png);"

                "background-color:transparent;"

                                     "}";
    QString styleSheet5 = "QLabel {"
               "image: url(:/images/not_Tuned.png);"

                "background-color:transparent;"

                                     "}";

// Set the flag based on the argument
 // Enable or disable the US buttons based on the flag
   //QMessageBox::information(nullptr,"Info","Tune is completed");
 ui->ULTRASONICBUT1->setEnabled(true);    // us1
    ui->label_16->setStyleSheet(styleSheet4);
    ui->label_17->setStyleSheet(styleSheet4);
    ui->label_18->setStyleSheet(styleSheet4);
    ui->label_27->setStyleSheet(styleSheet4);
    ui->ULTRASONICBUT2->setEnabled(true);  // us2
    ui->ULTRASONICBUT3->setEnabled(true);  // us3
    ui->ULTRASONICBUT4->setEnabled(true);  // us4
    ui->label_19->setStyleSheet("image: url(:/images/doubled.png); background-color:transparent;border:none;");


}
//power on off button when power is on off
void MainWindow::enableButtons(bool powerOn)
{
    // Style sheets for ON and OFF states
    QString styleSheetOn = "QPushButton {"
                           " font:20pt Ubuntu;"
                           " background-color: green;"
                           " color: black;"
                           " border:5px solid black;"
                           " border-radius:30px; font-weight: bold;"
                           "}";
    QString styleSheetOff = "QPushButton {"
                            " font:20pt Ubuntu;"
                            " background-color: red;"
                            " color: black;"
                            " border:5px solid black;"
                            " border-radius:30px; font-weight: bold;"
                            "}";

    // Enable or disable buttons and update their styles
    if (powerOn) {
         handler->buzz();
        ui->us1onoff->setStyleSheet(styleSheetOn);
        ui->us1onoff->setText("ON");
        ui->us1powup_but->setEnabled(true);
        ui->us1powdown_but->setEnabled(true);

        ui->us2onoff->setStyleSheet(styleSheetOn);
        ui->us2onoff->setText("ON");
        ui->us2powup_but->setEnabled(true);
        ui->us2powdown_but->setEnabled(true);

        ui->us3onoff->setStyleSheet(styleSheetOn);
        ui->us3onoff->setText("ON");
        ui->us3powup_but->setEnabled(true);
        ui->us3powdown_but->setEnabled(true);

        ui->us4onoff->setStyleSheet(styleSheetOn);
        ui->us4onoff->setText("ON");
        ui->us4powup_but->setEnabled(true);
        ui->us4powdown_but->setEnabled(true);

        ui->vitonoff->setStyleSheet(styleSheetOn);
        ui->vitonoff->setText("ON");
        ui->vitpowup_but->setEnabled(true);
        ui->vitpowdown_but->setEnabled(true);

        // Turn on the hardware features for all buttons
        int range = lfoot->convert(0x97);
        int pow1 = ui->lineEdit_57->text().toInt();
        handler->fs_count(range);
        handler->freq_count(2500);
        handler->phaco_on();
        handler->phaco_power(pow1);
        handler->pdm_mode(0);

    } else {
         handler->buzz();
        ui->us1onoff->setStyleSheet(styleSheetOff);
        ui->us1onoff->setText("OFF");
        ui->us1powup_but->setEnabled(false);
        ui->us1powdown_but->setEnabled(false);

        ui->us2onoff->setStyleSheet(styleSheetOff);
        ui->us2onoff->setText("OFF");
        ui->us2powup_but->setEnabled(false);
        ui->us2powdown_but->setEnabled(false);

        ui->us3onoff->setStyleSheet(styleSheetOff);
        ui->us3onoff->setText("OFF");
        ui->us3powup_but->setEnabled(false);
        ui->us3powdown_but->setEnabled(false);

        ui->us4onoff->setStyleSheet(styleSheetOff);
        ui->us4onoff->setText("OFF");
        ui->us4powup_but->setEnabled(false);
        ui->us4powdown_but->setEnabled(false);

        ui->vitonoff->setStyleSheet(styleSheetOff);
        ui->vitonoff->setText("OFF");
        ui->vitpowup_but->setEnabled(false);
        ui->vitpowdown_but->setEnabled(false);

        // Turn off the hardware features
        handler->fs_count(0);
        handler->freq_count(0);
        handler->phaco_off();
    }
}
void MainWindow::tabupdate(int index)
{
    ui->tabWidget_2->setCurrentIndex(index);
}

void MainWindow::speedofthelabe(QLabel *label)
{
    // Toggle label visibility and add a delay without freezing UI
    label->setVisible(!label->isVisible()); // Toggle label visibility

    QThread::msleep(10); // Sleep for 1000 ms (1 second)

    // Process pending events in the event loop
    QCoreApplication::processEvents();
}
void MainWindow::footpedalbeep()
{

}
void MainWindow::beepsound()
{
    if(vibon == "Vibration ON"){
    system("beep");
    ui->pushButton_2->setStyleSheet("image: url(:/images/vibrationon.png);border-radius:20px;border:none;");
    }else{
        system(0);
        ui->pushButton_2->setStyleSheet("image: url(:/images/vibrationoff.png);border-radius:20px;border:none;");
    }
   // qDebug()<<"beep";
//    QProcess p;
//    QString path = "/usr/bin/beep";
//    p.setProgram(path);
//    p.start();
//    p.waitForFinished();
}

// LINEAR
//footpedallllllllllllllllllllllllllll
void MainWindow::footpedalcheck()
{

        qint64 elapsed = currentTimer.elapsed();
        QString message;
    QString styleSheet3 = "QPushButton {"
           " font:20pt Ubuntu;"
          " background-color: green;"
            "color: black;"
          "image: url(:/images/ci.png);"
           " border-radius:30px;font-weight: bold;"
                                 "}";
    QString styleSheet4 = "QPushButton {"
           " font:20pt Ubuntu;"
          " background-color: red;"
            "color: black;"
           "image: url(:/images/ci.png);"
           " border:5px solid black;"
           " border-radius:30px;font-weight: bold;"
                                 "}";
    QString styleSheet1 = "QPushButton {"
           " font:20pt Ubuntu;"
          " background-color: green;"
            "color: black;"

           " border:5px solid black;"
           " border-radius:30px;font-weight: bold;"
                                 "}";

    QString styleSheet2 = "QPushButton {"
           " font:20pt Ubuntu;"
          " background-color: rgb(224, 27, 36);"
            "color: black;"
            //"image: url(:/ci.png);"
           " border:5px solid black;"
           " border-radius:30px;font-weight: bold;"
                                 "}";
    int range = lfoot->convert(0x97);
    QString text=ui->comboBox->currentText();
  QString powerdelivered=ui->CutMode_vitCom->currentText();
  QString powerdelivered_1=ui->CutMode_vitCom_2->currentText();
  QString powerdelivered_2=ui->CutMode_vitCom_3->currentText();
  QString powerdeliverd_3=ui->CutMode_vitCom_4->currentText();
  QString us1=ui->us1mode->text();
  QString ia1=ui->ia1mode->text();
  QString ia2=ui->ia2mode->text();
  QString vus1=ui->us1vacmode->text();
  QString us2=ui->us2mode->text();
  QString vus2=ui->us2vacmode->text();
  QString us3=ui->us3mode->text();
  QString vus3=ui->us3vacmode->text();
  QString us4=ui->us4mode->text();
  QString vus4=ui->us4vacmode->text();
  QString vit=ui->vitmode->text();
  QString vvac=ui->vitvacmode->text();

    switch(butname)
    {

    // dia
    case 0:
    {
        int pow = (ui->lineEdit_74->text().toInt() * 255) / 100;
        bool flag = true;
        static int currentCount = -1;
   if(flag){
            if (range > 0 && range < nfpzero) {
                ui->pushButton_42->setText("0");
                currentCount = 0;
                ui->dial_2->setValue(range);
                handler->dia_off();

                if (!overallci) {
                    ui->CI5_5->setStyleSheet(styleSheet4);
                    handler->pinchvalve_off();
                    handler->safetyvent_off();
                }

                if (!ventondia) {
                    handler->safetyvent_on();
                    QThread::msleep(100);
                    handler->safetyvent_off();
                    ventondia = true;
                }
                flag=true;
            }
            else if (range >= nfpzero && range < (nfpzero + nfpone + nfptwo + nfpthree)) {
                ui->pushButton_42->setText("1");
                currentCount = 1;
                ui->dial_2->setValue(range);
                handler->dia_on();
                handler->dia_count(pow);

                ui->CI5_5->setStyleSheet(styleSheet3);
                handler->pinchvalve_on();
                handler->safetyvent_on();
                if (!ventondia) {
                    handler->safetyvent_on();
                    QThread::msleep(100);
                    handler->safetyvent_off();
                    ventondia = true;
                }
            }
            if (currentCount > diacount) {
                beepsound();
            }
            if (currentCount != -1) {
                diacount = currentCount;
            }
   }
        break;
    }
  //us1
    case 1: {
        //they will not run
        int pow1 = ui->lineEdit_57->text().toInt(); // Get the power value from the line edit
        int vacline=ui->lineEdit_55->text().toInt();
        QString text=ui->us1onoff->text();
        bool flag1 = false;
        int us1currectcount=-1;

        if ((us1 == "Panel")||(vus1=="Panel")){
            if (range>0 && range<nfpzero) {
                ui->pushButton_42->setText("0");
  handler->speaker_off();
                ui->dial_2->setValue(range);
                    handler->freq_count(0);
                    handler->phaco_off();
                    handler->fs_count(0);
                     us1poweron=false;
                       if(!overallci){
                           ui->CI5_5->setStyleSheet(styleSheet4);
                           handler->pinchvalve_off();
                    }

                       if(ventonus1==false){
                        //   handler->pinchvalve_on();
                 handler->safetyvent_on();
                     QThread::msleep(100);
                       handler->pinchvalve_off();
                     handler->safetyvent_off();
                     ventonus1=true;
                       }
                       int pro=readsensorvalue();
                       ui->label_8->setText(QString::number(pro));
                   motoroff();

                ui->label_7->setText("0");
  handler->speaker_off();
  us1currectcount=0;
                flag1 = true; // Reset flag
}
            else if (range >= nfpzero & range < nfpzero+nfpone) {
                     ui->pushButton_42->setText("1");
                     ui->dial_2->setValue(range);
                     handler->pinchvalve_on();
                     handler->speaker_on(0,0,1,0);
                     if(ventonus1==false){
                     handler->safetyvent_on();
                   QThread::msleep(100);
                   handler->safetyvent_off();
                   ventonus1=true;
                     }

                  int pro=readsensorvalue();
                  ui->label_8->setText(QString::number(pro));
                  ui->CI5_5->setStyleSheet(styleSheet3);
                  //footpedalbeep();
                  motoroff();
                  ui->label_7->setText("0");
handler->speaker_on(0,0,1,0);
    handler->freq_count(0);
    handler->phaco_off();
    handler->fs_count(0);
us1poweron=false;
us1currectcount=1;
flag1 = true;
            }
            else if (range >=(nfpone+nfpzero) && range < (nfpzero+nfpone+nfptwo)) {
                ui->pushButton_42->setText("2");
                ui->label_8->show();
                ventonus1=false;
                ui->dial_2->setValue(range);
                ui->CI5_5->setStyleSheet(styleSheet3);
                handler->pinchvalve_on();
                handler->safetyvent_off();
                //footpedalbeep();
  if(vus1=="Panel"){
                int nonlinear_prevac = readsensorvalue(); // Assuming this function reads the current sensor value
                int nonlinear_vac = std::min(nonlinear_prevac, vacline);
                ui->label_8->setText(QString::number(nonlinear_vac));
                motoron(ui->lineEdit_69);
                //handler->speaker_on(nonlinear_vac,1,0,0);
                if (nonlinear_prevac >= vacline) {
                    motoroff(); // Turn off the motor
                    speedofthelabe(ui->label_8);
                   handler->speaker_on(nonlinear_vac,0,0,1);
                }
  }
                    handler->freq_count(0);
                    handler->phaco_off();
                    handler->fs_count(0);
                    us1poweron= false;
                    us1currectcount=2;

                flag1 = true; // Reset flag
            }
            else if (range >=(nfpzero+nfpone+nfptwo) && range < (nfpzero+nfpone+nfptwo+nfpthree)) {
                ui->pushButton_42->setText("3");
                 ui->dial_2->setValue(range);
                ui->label_7->show();
                //footpedalbeep();
                ventonus1=false;
                handler->pinchvalve_on();
                handler->safetyvent_off();
                ui->CI5_5->setStyleSheet(styleSheet3);
                if(vus1=="Panel"){
                              int nonlinear_prevac = readsensorvalue(); // Assuming this function reads the current sensor value
                              int nonlinear_vac = std::min(nonlinear_prevac, vacline);
                              ui->label_8->setText(QString::number(nonlinear_vac));
                              motoron(ui->lineEdit_69);
                             // handler->speaker_on(nonlinear_vac,0,1,0);
                              if (nonlinear_prevac >= vacline) {
                                  motoroff(); // Turn off the motor
                                  speedofthelabe(ui->label_8);
                                  handler->speaker_on(nonlinear_vac,0,0,1);

                              }
                }
                                  if(!us1poweron){
                                      us1poweron=true;
                       // Trigger relevant handlers
                       handler->fs_count(range);
                       handler->freq_count(2500);
                       handler->phaco_on();
                       ui->label_7->setText(QString::number(pow1));
                       handler->phaco_power(pow1);
                       elapsedTimeUS1 += elapsed;
                       message = "Effective time for US1: " + QString::number(elapsedTimeUS1 / 1000.0, 'f', 2) + " s";
}
                                     us1currectcount=3;
 }

  }
        else if ((us1 == "Surgeon")||( vus1 == "Surgeon")) {
            if (range > 0 && range < nfpzero) {
                ui->pushButton_42->setText("0");
                ui->dial_2->setValue(range);
                if (!overallci) {
                    ui->CI5_5->setStyleSheet(styleSheet4);
                    handler->pinchvalve_off();
                }
                if(ventonus1==false){
                   handler->safetyvent_on();
              QThread::msleep(100);
              handler->safetyvent_off();
              ventonus1=true;
                }
                motoroff();

                 handler->phaco_off();
                 handler->phaco_power(0);
                 handler->freq_count(0);
                 handler->fs_count(0);
                ui->label_8->setText("0");
                ui->label_7->setText("0");
                handler->speaker_off();
                us1currectcount=0;
                flag1 = true; // Reset flag

            } else if (range >= nfpzero && range < (nfpzero + nfpone)) {
                // Position 1 behavior
                ui->pushButton_42->setText("1");
                //footpedalbeep();
                ui->dial_2->setValue(range);
                handler->pinchvalve_on();

                if(ventonus1==false){
                    handler->safetyvent_on();
              QThread::msleep(100);
                handler->pinchvalve_off();
             handler->safetyvent_off();
              ventonus1=true;
                }

                ui->CI5_5->setStyleSheet(styleSheet3);
                motoroff();
                ui->label_7->setText("0");
                ui->label_8->setText("0");

                handler->phaco_off();
                handler->phaco_power(0);
                handler->freq_count(0);
                handler->fs_count(0);
                handler->speaker_on(0,0,1,0);
                us1currectcount=1;
                flag1 = true; // Reset flag

            } else if (range >= (nfpzero + nfpone) && range < (nfpzero + nfpone + nfptwo)) {
                // Position 2 behavior
                ui->pushButton_42->setText("2");
                //footpedalbeep();
                ui->dial_2->setValue(range);
                ventonus1 = false;

                handler->pinchvalve_on();
                handler->safetyvent_off();
                ui->CI5_5->setStyleSheet(styleSheet3);
        if(vus1=="Surgeon"){
                const int MIN_RANGE = nfpzero + nfpone;
                const int MAX_RANGE =nfpzero + nfpone + nfptwo;
                int divi =  MAX_RANGE-MIN_RANGE ; // Divider
                int calibration = range - MIN_RANGE; // Calibration value

                if (divi != 0) {
                    double final = static_cast<double>(vacline) / static_cast<double>(divi);
                    int presetvac = static_cast<int>(std::round(calibration * final));
                    int pro = readsensorvalue();
                     ui->label_8->setText(QString::number(pro));
                     handler->speaker_on(presetvac,1,0,0);
                     if (pro < presetvac) {
                        motoron(ui->lineEdit_56);
                        if (!motorus1) {
                            motoron(ui->lineEdit_56);
                            motorus1 = true;
                        }
                    } else if (motorus1) {
                        motoroff();
                        motorus1 = false;
                    }
                     if(pro>=vacline){
                          pro=static_cast<int>(vacline);
                           speedofthelabe(ui->label_8);
                           ui->label_8->setText(QString::number(pro));
                           handler->speaker_on(0,0,0,1);

              }
                }
  }


                handler->phaco_off();
                handler->phaco_power(0);
                handler->freq_count(0);
                handler->fs_count(0);
   us1currectcount=2;
                flag1 = true; // Reset flag

            } else if (range >= (nfpzero + nfpone + nfptwo) && range < (nfpzero + nfpone + nfptwo + nfpthree)) {
                ui->pushButton_42->setText("3");
                ui->dial_2->setValue(range);
                //footpedalbeep();
                ventonus1 = false;

                handler->pinchvalve_on();
                handler->safetyvent_off();
                ui->CI5_5->setStyleSheet(styleSheet3);
                if(vus1=="Surgeon"){
                              const int MIN_RANGE = nfpzero + nfpone+nfptwo;
                              const int MAX_RANGE =nfpzero + nfpone + nfptwo+nfpthree;
                              int divi =  MAX_RANGE-MIN_RANGE ; // Divider
                              int calibration = range - MIN_RANGE; // Calibration value

                              if (divi != 0) {
                                  double final = static_cast<double>(vacline) / static_cast<double>(divi);
                                  int presetvac = static_cast<int>(std::round(calibration * final));
                                  int pro = readsensorvalue();
                                   ui->label_8->setText(QString::number(pro));
                                   handler->speaker_on(presetvac,1,0,0);
                                   if (pro < presetvac) {
                                      motoron(ui->lineEdit_56);
                                      if (!motorus1) {
                                          motoron(ui->lineEdit_56);
                                          motorus1 = true;
                                      }
                                  } else if (motorus1) {
                                      motoroff();
                                      motorus1 = false;
                                  }if(pro>=vacline){
                                       pro=static_cast<int>(vacline);
                                       speedofthelabe(ui->label_8);
                                       ui->label_8->setText(QString::number(pro));
                                       handler->speaker_on(0,0,0,1);

                                   }

                              }
                }
                if (!us1poweron) {
                    us1poweron=true;
                    handler->phaco_on();
                    handler->freq_count(2500);
                    handler->fs_count(range);
                    updateTabsBasedOnComboBox(ui->CutMode_vitCom->currentText());
                    // If pushButton is ON
                    float progress4 = ((range - static_cast<float>(nfpone + nfptwo + nfpzero)) /
                                       (4096.0 - static_cast<float>(nfpone + nfptwo + nfpzero))) * pow1;
                   // qDebug()<<"the power is"<<progress4;
                     handler->phaco_power(std::round(progress4));
                    ui->label_7->setText(QString::number(std::round(progress4)));

                    if (progress4 == pow1) {
                        speedofthelabe(ui->label_7);
                    }

                }else {  // If pushButton is OFF
                   handler->phaco_off();
                   handler->freq_count(0);
                   handler->fs_count(0);
                   handler->phaco_power(0);
                }
          us1currectcount=3;


            }

        }
        if (us1currectcount> us1count) {
            beepsound();
        }
        if (us1currectcount != -1) {
            us1count = us1currectcount;
        }

        break;
    }
        // us2
    case 2: {
        int pow2 = ui->lineEdit_58->text().toInt(); // Get the power value from the line edit
        bool flag2 = true;
        double us2vac=ui->lineEdit_60->text().toInt();
        int us2currentcount=-1;

        if (us2 == "Panel" || vus2 == "Panel") {
            if (range > 0 && range < nfpzero) {
                ui->pushButton_42->setText("0");
                  ui->dial_2->setValue(range);
                if(!overallci){
                     ui->CI5_5->setStyleSheet(styleSheet4);
                    handler->pinchvalve_off();

                }
                if(ventonus2==false){
                    handler->safetyvent_on();
                                     QThread::msleep(100);
                                     handler->safetyvent_off();
                                     ventonus2=true;
                }
                int prous2=readsensorvalue();
                ui->label_93->setText(QString::number(prous2));
                motoroff();
                ui->label_92->setText("0");
                    handler->freq_count(0);
                    handler->phaco_off();
                    handler->fs_count(0);
                    us2poweron=false;
                    handler->speaker_off();
                    us2currentcount=0;
                 flag2 = true; // Reset flag
            }  else if (range >= nfpzero && range < (nfpzero + nfpone)) {
                ui->pushButton_42->setText("1");
                handler->speaker_on(0,0,1,0);
                //footpedalbeep();
                  ui->dial_2->setValue(range);
                  ui->CI5_5->setStyleSheet(styleSheet3);
                  handler->pinchvalve_on();
              if(ventonus2==false){
                       handler->safetyvent_on();
                       QThread::msleep(100);
                       handler->safetyvent_off();
                       ventonus2=true;
              }
                      motoroff();
                int prous2=readsensorvalue();
                ui->label_93->setText(QString::number(prous2));
                    handler->phaco_off();
                    ui->label_92->setText("0");
                    handler->freq_count(0);
                    handler->phaco_off();
                    handler->fs_count(0);
                   us2poweron = false;
                    us2currentcount=1;
                flag2 = true; // Reset flag
            } else if (range >= (nfpzero + nfpone) && range < (nfpzero + nfpone + nfptwo)) {
                ui->pushButton_42->setText("2");
                ventonus2=false;
                //footpedalbeep();
                  ui->dial_2->setValue(range);
                  ui->CI5_5->setStyleSheet(styleSheet3);
                handler->pinchvalve_on();
                handler->safetyvent_off();
                ui->label_92->setText("0");
                if(vus2=="Panel"){
                int nonlinear_prevac = readsensorvalue(); // Assuming this function reads the current sensor value
                int nonlinear_vac = std::min(nonlinear_prevac,static_cast<int>(us2vac));
                ui->label_93->setText(QString::number(nonlinear_vac));
                motoron(ui->lineEdit_59);
                handler->speaker_on(nonlinear_prevac,1,0,0);
                if (nonlinear_prevac >= us2vac) {
                    motoroff(); // Turn off the motor
                    speedofthelabe(ui->label_93);
                    handler->speaker_on(0,0,0,1);
                }
                }
                    handler->freq_count(0);
                    handler->phaco_off();
                    handler->fs_count(0);
                    us2poweron=false;
                    us2currentcount=2;
                flag2 = true; // Reset flag
            } else if (range >= (nfpzero + nfpone + nfptwo) && range < (nfpzero + nfpone + nfptwo + nfpthree))  {
                ui->pushButton_42->setText("3");
                ventonus2=false;
                //footpedalbeep();
                  ui->dial_2->setValue(range);
              ui->CI5_5->setStyleSheet(styleSheet3);
                handler->pinchvalve_on();
                handler->safetyvent_off();
                if(vus2=="Panel"){
                int nonlinear_prevac = readsensorvalue(); // Assuming this function reads the current sensor value
                int nonlinear_vac = std::min(nonlinear_prevac,static_cast<int>(us2vac));
                ui->label_93->setText(QString::number(nonlinear_vac));
                handler->speaker_on(nonlinear_vac,1,0,0);
                motoron(ui->lineEdit_59);
                if (nonlinear_prevac >= us2vac) {
                    motoroff(); // Turn off the motor
                    speedofthelabe(ui->label_93);
                    handler->speaker_on(0,0,0,1);
                }
                }
                if (!us2poweron) {
                    us2poweron=true;
                   updateTabsBasedOnComboBox(powerdelivered_1);
                    handler->freq_count(2500);
                    handler->phaco_on();
                    handler->fs_count(range);
                    ui->label_92->setText(QString::number(pow2));
                    handler->phaco_power(pow2);
                    elapsedTimeUS2 += elapsed;
                    message = "Effective time for US2: " + QString::number(elapsedTimeUS2 / 1000.0, 'f', 2) + " s";
                }
                us2currentcount=3;
            }
        } else if (us2 == "Surgeon" || vus2 == "Surgeon") {//panel
             if (range > 0 && range < nfpzero) {
                ui->pushButton_42->setText("0");
                  ui->dial_2->setValue(range);
          if(!overallci){
                ui->CI5_5->setStyleSheet(styleSheet4);
                handler->pinchvalve_off();
            }
                   if(ventonus2==false){
                     handler->safetyvent_on();
                     QThread::msleep(100);
                     handler->safetyvent_off();
                     ventonus2=true;
}

                motoroff();
                int prous2=readsensorvalue();
                ui->label_93->setText(QString::number(prous2));
                ui->label_92->setText("0");
                    handler->freq_count(0);
                    handler->phaco_off();
                    handler->fs_count(0);
                    handler->pdm_mode(0);
                    us2poweron=false;
                     handler->speaker_off();
                     us2currentcount=0;
                flag2 = true; // Reset flag
            }  else if (range >= nfpzero && range < (nfpzero + nfpone)) {
                ui->pushButton_42->setText("1");
                handler->speaker_on(0,0,1,0);
                //footpedalbeep();
                  ui->dial_2->setValue(range);
                 ui->CI5_5->setStyleSheet(styleSheet3);

                handler->pinchvalve_on();
                if(ventonus2==false){
                    handler->safetyvent_on();
                    QThread::msleep(100);
                   handler->safetyvent_off();
                  ventonus2=true;
                }
                                    motoroff();
                handler->phaco_off();
                ui->label_92->setText("0");
                int prous2=readsensorvalue();
                ui->label_93->setText(QString::number(prous2));
                    handler->freq_count(0);
                    handler->phaco_off();
                    handler->fs_count(0);
                    handler->pdm_mode(0);
                   us2poweron=false;
                   us2currentcount=1;
                flag2 = true; // Reset flag
            } else if (range >= (nfpzero + nfpone) && range < (nfpzero + nfpone + nfptwo)) {
                ui->pushButton_42->setText("2");

                //footpedalbeep();
                  ui->dial_2->setValue(range);
                  ui->CI5_5->setStyleSheet(styleSheet3);

                handler->pinchvalve_on();
                handler->safetyvent_off();
                ui->label_92->setText("0");
                if(vus2=="Surgeon"){
                              const int MIN_RANGE = nfpzero + nfpone;
                              const int MAX_RANGE =nfpzero + nfpone + nfptwo;
                              int divi =  MAX_RANGE-MIN_RANGE ; // Divider
                              int calibration = range - MIN_RANGE; // Calibration value

                              if (divi != 0) {
                                  double final = static_cast<double>(us2vac) / static_cast<double>(divi);
                                  int presetvac = static_cast<int>(std::round(calibration * final));
                                  int pro = readsensorvalue();
                                   ui->label_93->setText(QString::number(pro));
                                   handler->speaker_on(presetvac,1,0,0);
                                   if (pro < presetvac) {
                                      motoron(ui->lineEdit_59);
                                      if (!motorus2) {
                                          motoron(ui->lineEdit_59);
                                          motorus2 = true;
                                      }
                                  } else if (motorus2) {
                                      motoroff();
                                      motorus2 = false;
                                  }
                                   if(pro>=us2vac){
                                       pro=static_cast<int>(us2vac);
                                       speedofthelabe(ui->label_93);
                                       ui->label_93->setText(QString::number(pro));
                                       handler->speaker_on(0,0,0,1);

                                   }
                              }
                }
                    handler->freq_count(0);
                    handler->phaco_off();
                    handler->fs_count(0);
                    handler->pdm_mode(0);
             us2poweron=false;
             us2currentcount=2;
                flag2 = true; // Reset flag
            } else if (range >= (nfpzero + nfpone + nfptwo) && range < (nfpzero + nfpone + nfptwo + nfpthree))  {
                ui->pushButton_42->setText("3");
                //footpedalbeep();
                  ui->dial_2->setValue(range);
                ui->CI5_5->setStyleSheet(styleSheet3);
                handler->pinchvalve_on();
                handler->safetyvent_off();
                if(vus2=="Surgeon"){
                              const int MIN_RANGE = nfpzero + nfpone+nfptwo;
                              const int MAX_RANGE =nfpzero + nfpone + nfptwo+nfpthree;
                              int divi =  MAX_RANGE-MIN_RANGE ; // Divider
                              int calibration = range - MIN_RANGE; // Calibration value

                              if (divi != 0) {
                                  double final = static_cast<double>(us2vac) / static_cast<double>(divi);
                                  int presetvac = static_cast<int>(std::round(calibration * final));
                                  int pro = readsensorvalue();
                                   ui->label_93->setText(QString::number(pro));
                                   handler->speaker_on(presetvac,1,0,0);
                                   if (pro < presetvac) {
                                      motoron(ui->lineEdit_59);
                                      if (!motorus2) {
                                          motoron(ui->lineEdit_59);
                                          motorus2 = true;
                                      }
                                  } else if (motorus2) {
                                      motoroff();
                                      motorus2 = false;
                                  }
                                   if(pro>=us2vac){
                                       pro=static_cast<int>(us2vac);
                                       speedofthelabe(ui->label_93);
                                       ui->label_93->setText(QString::number(pro));
                                       handler->speaker_on(0,0,0,1);

                                   }
                              }
                }

                if (!us2poweron) {
                    us2poweron=true;
                   updateTabsBasedOnComboBox(powerdelivered_1);
                    handler->freq_count(2500);
                    handler->phaco_on();
                    handler->fs_count(range);
                    handler->phaco_power(pow2);
                    elapsedTimeUS2 += elapsed;
                    message = "Effective time for US2: " + QString::number(elapsedTimeUS2 / 1000.0, 'f', 2) + " s";
                    // If pushButton is ON
                    float progress4 = ((range - static_cast<float>(nfpone + nfptwo + nfpzero)) /
                                       (4096.0 - static_cast<float>(nfpone + nfptwo + nfpzero))) * pow2;
                  //  qDebug()<<"the power is"<<progress4;
                     handler->phaco_power(std::round(progress4));
                    ui->label_93->setText(QString::number(std::round(progress4)));
                    if (progress4 == pow2) {
                        speedofthelabe(ui->label_93);
                    }
                }else {  // If pushButton is OFF
                   handler->phaco_off();
                   handler->freq_count(0);
                   handler->fs_count(0);
                   handler->phaco_power(0);
                }
                  us2currentcount=3;
            }

        }
        if (us2currentcount> us2count) {
            beepsound();
        }
        if (us2currentcount != -1) {
            us2count = us2currentcount;
        }

        break;
    }
        //us 3

    case 3: {
        int pow3 = ui->lineEdit_61->text().toInt(); // Get the power value from the line edit
        bool flag3 = true;
        double us3vacline=ui->lineEdit_63->text().toInt();
        int us3currentcount=-1;

        if (us3 == "Panel" || vus3 == "Panel") {
            if (range > 0 && range < nfpzero) {
                ui->pushButton_42->setText("0");
                  ui->dial_2->setValue(range);
                if(!overallci){
                     ui->CI5_5->setStyleSheet(styleSheet4);
                    handler->pinchvalve_off();
                }
                if(ventonus3==false){
                    handler->safetyvent_on();
                                     QThread::msleep(100);
                                     handler->safetyvent_off();
                                     ventonus3=true;}

                motoroff();

                handler->phaco_off();
                ui->label_98->setText("0");
                int prous3=readsensorvalue();
                ui->label_99->setText(QString::number(prous3));

                    handler->freq_count(0);
                    handler->phaco_off();
                    handler->fs_count(0);
                    handler->pdm_mode(0);
                    us3poweron=false;
                     handler->speaker_off();
                     us3currentcount=0;
                flag3 = true; // Reset flag
            }  else if (range >= nfpzero && range < (nfpzero + nfpone)) {
                ui->pushButton_42->setText("1");
                handler->speaker_on(0,0,1,0);

                //footpedalbeep();
                  ui->dial_2->setValue(range);
                   ui->CI5_5->setStyleSheet(styleSheet3);
                   if(ventonus3==false){
                       handler->safetyvent_on();
                       QThread::msleep(100);
                       handler->safetyvent_off();
                       ventonus3=true;
                   }
                handler->pinchvalve_on();
                motoroff();
                handler->phaco_off();
                ui->label_98->setText("0");
                int prous3=readsensorvalue();
                ui->label_99->setText(QString::number(prous3));

                    handler->freq_count(0);
                    handler->phaco_off();
                    handler->fs_count(0);
                    handler->pdm_mode(0);
                    us3poweron=false;
                    us3currentcount=1;
                    flag3 = true; // Reset flag
            } else if (range >= (nfpzero + nfpone) && range < (nfpzero + nfpone + nfptwo)) {
                ui->pushButton_42->setText("2");
                //footpedalbeep();
                  ui->dial_2->setValue(range);
             ventonus3=false;
                  ui->CI5_5->setStyleSheet(styleSheet3);
                handler->safetyvent_off();
                handler->pinchvalve_on();
                if(vus3=="Panel"){
                              int nonlinear_prevac = readsensorvalue(); // Assuming this function reads the current sensor value
                              int nonlinear_vac = std::min(nonlinear_prevac, static_cast<int>(us3vacline));
                              ui->label_99->setText(QString::number(nonlinear_vac));
                              motoron(ui->lineEdit_62);
                              handler->speaker_on(nonlinear_prevac,1,0,0);
                              if (nonlinear_prevac >= us3vacline) {
                                  motoroff(); // Turn off the motor
                                  speedofthelabe(ui->label_8);
                                  handler->speaker_on(0,0,0,1);

                              }
                }

                ui->label_98->setText("0");

                    handler->freq_count(0);
                    handler->phaco_off();
                    handler->fs_count(0);
            us3poweron=false;
            us3currentcount=2;
                flag3 = true; // Reset flag
            } else if (range >= (nfpzero + nfpone + nfptwo) && range < (nfpzero + nfpone + nfptwo + nfpthree))  {
                ui->pushButton_42->setText("3");
                ventonus3=false;
                //footpedalbeep();
                  ui->dial_2->setValue(range);
                ui->CI5_5->setStyleSheet(styleSheet3);
                handler->safetyvent_off();
                handler->pinchvalve_on();
                elapsedTimeUS3 += elapsed;
                message = "Effective time for US3: " + QString::number(elapsedTimeUS3 / 1000.0, 'f', 2) + " s";
                if(vus3=="Panel"){
                              int nonlinear_prevac = readsensorvalue(); // Assuming this function reads the current sensor value
                              int nonlinear_vac = std::min(nonlinear_prevac, static_cast<int>(us3vacline));
                              ui->label_99->setText(QString::number(nonlinear_vac));
                              motoron(ui->lineEdit_62);
                              handler->speaker_on(nonlinear_prevac,1,0,0);
                              if (nonlinear_prevac >= us3vacline) {
                                  motoroff(); // Turn off the motor
                                  speedofthelabe(ui->label_8);
                                  handler->speaker_on(0,0,0,1);

                              }
                }
                if (!us3poweron) {
                      us3poweron= true;
                    handler->freq_count(2500);
                    handler->phaco_on();
                    handler->fs_count(range);
                    ui->label_98->setText(QString::number(pow3));
                    handler->phaco_power(pow3);
                    updateTabsBasedOnComboBox(powerdelivered_2);


            }
us3currentcount=3;
            }
        } else if (us3 == "Surgeon" || vus3 == "Surgeon") {
             if (range > 0 && range < nfpzero) {
                ui->pushButton_42->setText("0");
                  ui->dial_2->setValue(range);
                if(!overallci){
                   ui->CI5_5->setStyleSheet(styleSheet4);
                    handler->pinchvalve_off();
                }
                if(ventonus3==false){
                    handler->safetyvent_on();
                                     QThread::msleep(100);
                                     handler->safetyvent_off();
                                     ventonus3=true;
                }
                motoroff();

                handler->phaco_off();
                ui->label_98->setText("0");
                int prous3=readsensorvalue();
                ui->label_99->setText(QString::number(prous3));
                    handler->freq_count(0);
                    handler->phaco_off();
                    handler->fs_count(0);
                    handler->pdm_mode(0);
                    us3poweron=false;
 handler->speaker_off();
 us3currentcount=0;
                flag3 = true; // Reset flag
            }  else if (range >= nfpzero && range < (nfpzero + nfpone)) {
                ui->pushButton_42->setText("1");
                handler->speaker_on(0,0,1,0);

                //footpedalbeep();
                  ui->dial_2->setValue(range);
                 ui->CI5_5->setStyleSheet(styleSheet3);
                 if(ventonus3==false){
                     handler->safetyvent_on();
                                      QThread::msleep(100);
                                      handler->safetyvent_off();
                                      ventonus3=true;}
                 handler->pinchvalve_on();
                motoroff();
                handler->phaco_off();
                ui->label_98->setText("0");
                int prous3=readsensorvalue();
                ui->label_99->setText(QString::number(prous3));

                    handler->freq_count(0);
                    handler->phaco_off();
                    handler->fs_count(0);
                    handler->pdm_mode(0);
        us3poweron=false;
        us3currentcount=1;
                flag3 = true; // Reset flag
            } else if (range >= (nfpzero + nfpone) && range < (nfpzero + nfpone + nfptwo)) {
                ui->pushButton_42->setText("2");
                //footpedalbeep();
                  ui->dial_2->setValue(range);
        ui->CI5_5->setStyleSheet(styleSheet3);
                handler->safetyvent_off();
                ventonus3=false;
               handler->pinchvalve_on();

               if(vus3=="Surgeon"){
                             const int MIN_RANGE = nfpzero + nfpone;
                             const int MAX_RANGE =nfpzero + nfpone + nfptwo;
                             int divi =  MAX_RANGE-MIN_RANGE ; // Divider
                             int calibration = range - MIN_RANGE; // Calibration value

                             if (divi != 0) {
                                 double final = static_cast<double>(us3vacline) / static_cast<double>(divi);
                                 int presetvac = static_cast<int>(std::round(calibration * final));
                                 int pro = readsensorvalue();
                                  ui->label_99->setText(QString::number(pro));
                                  handler->speaker_on(presetvac,1,0,0);
                                  if (pro < presetvac) {
                                     motoron(ui->lineEdit_62);
                                     if (!motorus3) {
                                         motoron(ui->lineEdit_62);
                                         motorus3 = true;
                                     }
                                 } else if (motorus3) {
                                     motoroff();
                                     motorus3 = false;
                                 }
                                  if(pro>=us3vacline){
                                      pro=static_cast<int>(us3vacline);
                                      speedofthelabe(ui->label_99);
                                      ui->label_99->setText(QString::number(pro));
                                      handler->speaker_on(0,0,0,1);

                                  }
                             }
               }

                ui->label_98->setText("0");

                    handler->freq_count(0);
                    handler->phaco_off();
                    handler->fs_count(0);
                    handler->pdm_mode(0);
                   us3poweron=false;
                   us3currentcount=2;
                flag3 = true; // Reset flag
            } else if (range >= (nfpzero + nfpone + nfptwo) && range < (nfpzero + nfpone + nfptwo + nfpthree))  {
                ui->pushButton_42->setText("3");
                //footpedalbeep();
                  ui->dial_2->setValue(range);
                 ui->CI5_5->setStyleSheet(styleSheet3);
                handler->safetyvent_off();
                ventonus3=false;
                handler->pinchvalve_on();
                if(vus3=="Surgeon"){
                              const int MIN_RANGE = nfpzero + nfpone+nfptwo;
                              const int MAX_RANGE =nfpzero + nfpone + nfptwo+nfpthree;
                              int divi =  MAX_RANGE-MIN_RANGE ; // Divider
                              int calibration = range - MIN_RANGE; // Calibration value

                              if (divi != 0) {
                                  double final = static_cast<double>(us3vacline) / static_cast<double>(divi);
                                  int presetvac = static_cast<int>(std::round(calibration * final));
                                  int pro = readsensorvalue();
                                   ui->label_99->setText(QString::number(pro));
                                   handler->speaker_on(presetvac,1,0,0);

                                   if (pro < presetvac) {
                                      motoron(ui->lineEdit_62);
                                      if (!motorus3) {
                                          motoron(ui->lineEdit_62);
                                          motorus3 = true;
                                      }
                                  } else if (motorus3) {
                                      motoroff();
                                      motorus3 = false;
                                  }
                                   if(pro>=us3vacline){
                                       pro=static_cast<int>(us3vacline);
                                       speedofthelabe(ui->label_99);
                                       ui->label_99->setText(QString::number(pro));
                                       handler->speaker_on(0,0,0,1);

                                   }
                              }
                }

                if (!us3poweron) {
                    us3poweron=true;
                    handler->freq_count(2500);
                    handler->phaco_on();
                    handler->fs_count(range);
                    handler->phaco_power(pow3);
                    updateTabsBasedOnComboBox(powerdelivered_2);
                    // If pushButton is ON
                    float progress4 = ((range - static_cast<float>(nfpone + nfptwo + nfpzero)) /
                                       (4096.0 - static_cast<float>(nfpone + nfptwo + nfpzero))) * pow3;
                    //qDebug()<<"the power is"<<progress4;
                     handler->phaco_power(std::round(progress4));
                    ui->label_99->setText(QString::number(std::round(progress4)));
                    if (progress4 == pow3) {
                        speedofthelabe(ui->label_99);
                    }
                }else {  // If pushButton is OFF
                   handler->phaco_off();
                   handler->freq_count(0);
                   handler->fs_count(0);
                   handler->phaco_power(0);
                }
us3currentcount=3;

            }
        }
        if (us3currentcount> us3count) {
            beepsound();
        }
        if (us3currentcount != -1) {
            us3count = us3currentcount;
        }

        break;
    }

        // us4
    case 4: {
        int pow4 = ui->lineEdit_64->text().toInt(); // Get the power value from the line edit
        double us4vacline=ui->lineEdit_66->text().toInt();
        int us4currentcount=-1;
        if (us4 == "Panel" || vus4 == "Panel") { // surgeon
            if (range > 0 && range < nfpzero) {
                ui->pushButton_42->setText("0");
                  ui->dial_2->setValue(range);
                if(!overallci){
                 ui->CI5_5->setStyleSheet(styleSheet4);
                    handler->pinchvalve_off();
                }
                if(ventonus4==false){
                    handler->safetyvent_on();
                                     QThread::msleep(100);
                                     handler->safetyvent_off();
                                     ventonus4=true;
                }

                //handler->pinchvalve_off();
                motoroff();
                handler->phaco_off();
               // ui->CI4->setStyleSheet(styleSheet4);
                ui->label_105->setText("0");
                int prous4=readsensorvalue();
                ui->label_104->setText(QString::number(prous4));

                    handler->freq_count(0);
                    handler->phaco_off();
                    handler->fs_count(0);
                    handler->pdm_mode(0);
             us4poweron=false;
             us4currentcount=0;
              handler->speaker_off();
            }  else if (range >= nfpzero && range < (nfpzero + nfpone)) {
                handler->speaker_on(0,0,1,0);
                ui->pushButton_42->setText("1");
                //footpedalbeep();
                  ui->dial_2->setValue(range);
                ui->CI5_5->setStyleSheet(styleSheet3);
                if(ventonus4==false){
                    handler->safetyvent_on();
                                     QThread::msleep(100);
                                     handler->safetyvent_off();
                                     ventonus4=true;
                }
                                    handler->pinchvalve_on();
                motoroff();
                handler->phaco_off();
                int prous4=readsensorvalue();
                ui->label_104->setText(QString::number(prous4));
                ui->label_105->setText("0");
                handler->freq_count(0);
                handler->phaco_off();
                handler->fs_count(0);
                handler->pdm_mode(0);
         us4poweron=false;
         us4currentcount=1;
            }
            else if (range >= (nfpzero + nfpone) && range < (nfpzero + nfpone + nfptwo)) {
                ui->pushButton_42->setText("2");
                //footpedalbeep();
                ui->dial_2->setValue(range);
                ui->CI5_5->setStyleSheet(styleSheet3);
                handler->safetyvent_off();
                handler->pinchvalve_on();
                ventonus4=false;
                handler->phaco_off();
                ui->label_105->setText("0");
                if(vus4=="Panel"){
                              int nonlinear_prevac = readsensorvalue(); // Assuming this function reads the current sensor value
                              int nonlinear_vac = std::min(nonlinear_prevac,static_cast<int>(us4vacline));
                              ui->label_104->setText(QString::number(nonlinear_vac));
                              handler->speaker_on(nonlinear_prevac,1,0,0);
                              motoron(ui->lineEdit_65);
                              if (nonlinear_prevac >= us4vacline) {
                                  motoroff(); // Turn off the motor
                                  speedofthelabe(ui->label_104);
                                  handler->speaker_on(0,0,0,1);
                              }
                }


                              handler->freq_count(0);
                              handler->phaco_off();
                              handler->fs_count(0);
                              handler->pdm_mode(0);
                              us4currentcount=2;
                       us4poweron=false;
            }else if (range >= (nfpzero + nfpone + nfptwo) && range < (nfpzero + nfpone + nfptwo + nfpthree))  {
                ui->pushButton_42->setText("3");
                //footpedalbeep();
                ventonus4=false;
                  ui->dial_2->setValue(range);
                ui->CI5_5->setStyleSheet(styleSheet3);
                handler->safetyvent_off();
                handler->pinchvalve_on();
 ui->CI5_5->setStyleSheet(styleSheet3);

                if (!us4poweron) {
                        us4poweron = true;
                    handler->freq_count(2500);
                    handler->phaco_on();
                    handler->fs_count(range);
                    ui->label_105->setText(QString::number(pow4));
                    handler->phaco_power(pow4);
                    handler->pdm_mode(1);
              updateTabsBasedOnComboBox(powerdeliverd_3);
              if(vus4=="Panel"){
                            int nonlinear_prevac = readsensorvalue(); // Assuming this function reads the current sensor value
                            int nonlinear_vac = std::min(nonlinear_prevac,static_cast<int>(us4vacline));
                            ui->label_104->setText(QString::number(nonlinear_vac));
                            motoron(ui->lineEdit_65);
                            handler->speaker_on(nonlinear_prevac,1,0,0);
                            if (nonlinear_prevac >= us4vacline) {
                                motoroff(); // Turn off the motor
                                speedofthelabe(ui->label_104);
                                handler->speaker_on(0,0,0,1);
                            }
              }
                    elapsedTimeUS4 += elapsed;
                    message = "Effective time for US4: " + QString::number(elapsedTimeUS4 / 1000.0, 'f', 2) + " s";
                }
us4currentcount=3;
            }
        } else if (us4 == "Surgeon" || vus4 == "Surgeon") {
            if (range > 0 && range < nfpzero) {
                ui->pushButton_42->setText("0");
                  ui->dial_2->setValue(range);
                if(!overallci){
                   ui->CI5_5->setStyleSheet(styleSheet4);
                    handler->pinchvalve_off();
                }
                if(ventonus4==false){
                    handler->safetyvent_on();
                                     QThread::msleep(100);
                                     handler->safetyvent_off();
                                     ventonus4=true;}
                int prous4=readsensorvalue();
                ui->label_104->setText(QString::number(prous4));
                motoroff();
                handler->freq_count(0);
                handler->phaco_off();
                handler->fs_count(0);
                handler->pdm_mode(0);
         us4poweron=false; handler->speaker_off();
         us4currentcount=0;
            }  else if (range >= nfpzero && range < (nfpzero + nfpone)) {
                ui->pushButton_42->setText("1");
                handler->speaker_on(0,0,1,0);
                //footpedalbeep();
                  ui->dial_2->setValue(range);

                 ui->CI5_5->setStyleSheet(styleSheet3);
                 if(ventonus4==false){
                     handler->safetyvent_on();
                                      QThread::msleep(100);
                                      handler->safetyvent_off();
                                      ventonus4=true;}
                                     handler->pinchvalve_on();


                motoroff();
                handler->phaco_off();
                ui->label_105->setText("0");
                int prous4=readsensorvalue();
                ui->label_104->setText(QString::number(prous4));
                handler->freq_count(0);
                handler->phaco_off();
                handler->fs_count(0);
                handler->pdm_mode(0);
         us4poweron=false;
         us4currentcount=1;
            } else if (range >= (nfpzero + nfpone) && range < (nfpzero + nfpone + nfptwo)) {
                ui->pushButton_42->setText("2");
                //footpedalbeep();
                  ui->dial_2->setValue(range);
ventonus4=false;
                handler->safetyvent_off();
                handler->pinchvalve_on();
 ui->CI5_5->setStyleSheet(styleSheet3);


                handler->phaco_off();
                ui->label_105->setText("0");

                if(vus4=="Surgeon"){
                              const int MIN_RANGE = nfpzero + nfpone;
                              const int MAX_RANGE =nfpzero + nfpone + nfptwo;
                              int divi =  MAX_RANGE-MIN_RANGE ; // Divider
                              int calibration = range - MIN_RANGE; // Calibration value

                              if (divi != 0) {
                                  double final = static_cast<double>(us4vacline) / static_cast<double>(divi);
                                  int presetvac = static_cast<int>(std::round(calibration * final));
                                  int pro = readsensorvalue();
                                   ui->label_104->setText(QString::number(pro));
                                   handler->speaker_on(presetvac,1,0,0);
                                   if (pro <= presetvac) {
                                      motoron(ui->lineEdit_65);
                                      if (!motorus4) {
                                          motoron(ui->lineEdit_65);
                                          motorus4 = true;
                                      }
                                  } else if (motorus4) {
                                      motoroff();
                                      motorus4 = false;
                                  }
                                   if (pro >= us4vacline) {
                                       pro = static_cast<int>(us4vacline);
                                       ui->label_104->setText(QString::number(pro));
                                       speedofthelabe(ui->label_104);
                                       motoroff();
                                       handler->speaker_on(0,0,0,1);
                                   }

                              }
                }


                                handler->freq_count(0);
                                handler->phaco_off();
                                handler->fs_count(0);
                                handler->pdm_mode(0);
                                us4currentcount=2;
                         us4poweron=false;
            } else if (range >= (nfpzero + nfpone + nfptwo) && range < (nfpzero + nfpone + nfptwo + nfpthree))  {
                ui->pushButton_42->setText("3");
                //footpedalbeep();
                  ui->dial_2->setValue(range);
                  ventonus4=false;

                handler->safetyvent_off();
                handler->pinchvalve_on();
 ui->CI5_5->setStyleSheet(styleSheet3);
 if(vus4=="Surgeon"){
               const int MIN_RANGE = nfpzero + nfpone+nfptwo;
               const int MAX_RANGE =nfpzero + nfpone + nfptwo+nfpthree;
               int divi =  MAX_RANGE-MIN_RANGE ; // Divider
               int calibration = range - MIN_RANGE; // Calibration value

               if (divi != 0) {
                   double final = static_cast<double>(us4vacline) / static_cast<double>(divi);
                   int presetvac = static_cast<int>(std::round(calibration * final));
                   int pro = readsensorvalue();
                    ui->label_104->setText(QString::number(pro));
                    handler->speaker_on(presetvac,1,0,0);
                    if (pro < presetvac) {
                       motoron(ui->lineEdit_65);
                       if (!motorus4) {
                           motoron(ui->lineEdit_65);
                           motorus4 = true;
                       }
                   } else if (motorus4) {
                       motoroff();
                       motorus4 = false;
                   }
                    // If pro exceeds vitpreset, cap it
                    if (pro >= us4vacline) {
                        pro = static_cast<int>(us4vacline);
                        ui->label_104->setText(QString::number(pro));
                        speedofthelabe(ui->label_104);
                        motoroff();
                        handler->speaker_on(0,0,0,0);
                    }
               }
 }

                if (!us4poweron) {
                    us4poweron = true;
                    handler->freq_count(2500);
                    handler->phaco_on();
                    handler->fs_count(range);
                    handler->phaco_power(pow4);
                   updateTabsBasedOnComboBox(powerdeliverd_3);
                   elapsedTimeUS4 += elapsed;
                   message = "Effective time for US4: " + QString::number(elapsedTimeUS4 / 1000.0, 'f', 2) + " s";
                   float progress4 = ((range - static_cast<float>(nfpone + nfptwo + nfpzero)) /
                                      (4096.0 - static_cast<float>(nfpone + nfptwo + nfpzero))) * pow4;
                   //qDebug()<<"the power is"<<progress4;
                    handler->phaco_power(std::round(progress4));
                   ui->label_104->setText(QString::number(std::round(progress4)));

                   if (progress4 == pow4) {
                       speedofthelabe(ui->label_104);
                   }

               }else {  // If pushButton is OFF
                  handler->phaco_off();
                  handler->freq_count(0);
                  handler->fs_count(0);
                  handler->phaco_power(0);
               }
                us4currentcount=3;
            }
        }
        if (us4currentcount> us4count) {
            beepsound();
        }
        if (us4currentcount != -1) {
            us4count = us4currentcount;
        }

        break;
    }

        // ia1
        case 5:
    {
bool flag6=false;
double ia1preset=ui->lineEdit_70->text().toDouble();
QString ia1=ui->ia2mode->text();
 int ia1currentcount=-1;
        if (ia1 == "Panel") { // surgeon
            if (range >= 0 && range < nfpzero) {
                ui->pushButton_42->setText("0");
                ui->dial_2->setValue(range);

                if (!overallci) {
                    ui->CI5_5->setStyleSheet(styleSheet4);
                    handler->pinchvalve_off();
                }
                if(ventonia1 == false) {
                    handler->safetyvent_on();
                    QThread::msleep(200);
                    handler->safetyvent_off();
                    ventonia1 = true;
                }
               handler->speaker_off();
                motoroff();
                int pro = readsensorvalue();
                ui->label_113->setText(QString::number(pro));
                ia1currentcount=0;
                flag6=true;
            }

            else if (range >= nfpzero && range < (nfpone + nfpzero)) {
                ui->pushButton_42->setText("1");
                handler->speaker_on(0,0,1,0);

               beepsound();
                ui->dial_2->setValue(range);
                ui->CI5_5->setStyleSheet(styleSheet3);
                if(ventonia1 == false) {
                    handler->safetyvent_on();
                    QThread::msleep(200);
                    handler->safetyvent_off();
                    ventonia1 = true;
                }
                handler->pinchvalve_on();
                motoroff();
                int pro = readsensorvalue();
                ui->label_113->setText(QString::number(pro));
ia1currentcount=1;
                 flag6=true;
            }

            else if (range >= (nfpzero + nfpone) && range < (nfpzero + nfpone + nfptwo + nfpthree)) {
                ui->pushButton_42->setText("2");
               beepsound();
                ventonia1 = false;
                handler->safetyvent_off();
                ui->dial_2->setValue(range);
                ui->CI5_5->setStyleSheet(styleSheet3);
                handler->pinchvalve_on();
               int nonlinear_prevac = readsensorvalue(); // Assuming this function reads the current sensor value
               int nonlinear_vac = std::min(nonlinear_prevac, static_cast<int>(ia1preset));
               ui->label_113->setText(QString::number(nonlinear_vac));
               motoron(ui->lineEdit_69);
               //handler->speaker_on(nonlinear_prevac,1,0,0);
               if (nonlinear_prevac >= ia1preset) {
                         motoroff(); // Turn off the motor
                         speedofthelabe(ui->label_113);
                         handler->speaker_on(0,0,0,1);

                    }
               ia1currentcount=2;
           }
        }

        else if(ia1 == "Surgeon"){
            if(range>=0 && range<nfpzero){
                ui->pushButton_42->setText("0");
                  ui->dial_2->setValue(range);
                if(!overallci){
             ui->CI5_5->setStyleSheet(styleSheet4);
                    handler->pinchvalve_off();
                }
                if(ventonia1 == false) {
                    handler->safetyvent_on();
                    QThread::msleep(200);
                    handler->safetyvent_off();
                    ventonia1 = true;
                }

                  motoroff();
                    int pro = readsensorvalue();
                 ui->label_113->setText(QString::number(pro));
                 handler->speaker_off();
                 ia1currentcount=0;
                  flag6=true;


            }
            else if(range>=(nfpzero) && range<(nfpone+nfpzero)){
                ui->pushButton_42->setText("1");
                handler->speaker_on(0,0,1,0);

                beepsound();
                  ui->dial_2->setValue(range);
             ui->CI5_5->setStyleSheet(styleSheet3);
                handler->pinchvalve_on();
                  motoroff();
                  int pro = readsensorvalue();
               ui->label_113->setText(QString::number(pro));


               if(ventonia1 == false) {
                   handler->safetyvent_on();
                   QThread::msleep(200);
                   handler->safetyvent_off();
                   ventonia1 = true;
               }
               ia1currentcount=1;
             flag6=true;
}
            if (range >= (nfpzero + nfpone) && range < (nfpzero + nfpone + nfptwo + nfpthree)) {
                ui->pushButton_42->setText("2");
                ventonia1=false;
                beepsound();
                ui->dial_2->setValue(range);
                ui->CI5_5->setStyleSheet(styleSheet3);
handler->safetyvent_off();
handler->pinchvalve_on();
                const int MIN_RANGE = nfpzero + nfpone;
                const int MAX_RANGE =nfpzero + nfpone + nfptwo + nfpthree;
                int divi =  MAX_RANGE-MIN_RANGE ; // Divider
                int calibration = range - MIN_RANGE; // Calibration value

                if (divi != 0) {
                    double final =ia1preset / divi;
                    int presetvac = static_cast<int>(std::round(calibration * final));
                    int pro =readsensorvalue();
                   handler->speaker_on(presetvac,1,0,0);
                    ui->label_113->setText(QString::number(pro));
                    const int tolerance = 5; // Example tolerance
                    if (pro < (presetvac - tolerance)) {
                        motoron(ui->lineEdit_69);
                        if (!motoria1) {
                            motoron(ui->lineEdit_69);
                            motoria1 = true;
                        }
                    } else if (pro >= presetvac || motoria1) {
                        motoroff();
                        motoria1 = false;
                    } // If pro exceeds vitpreset, cap it
                    if (pro >= ia1preset) {
                        pro = static_cast<int>(ia1preset);
                        ui->label_113->setText(QString::number(pro));
                        speedofthelabe(ui->label_113);
                      handler->speaker_on(0,0,0,1);
                        motoroff();
                    }

            }
                ia1currentcount=2;

}
        }
        if (ia1currentcount>ia1count) {
            beepsound();
        }
        if (ia1currentcount != -1) {
            ia1count = ia1currentcount;
        }
    break;
    }
        // ia2
        case 6:
        {
  bool flag6=true;
  ia2=ui->ia1mode->text();
  double ia2vacline=ui->lineEdit_68->text().toInt();
  int ia2currentcount=-1;


            if(ia2 == "Panel"){
            if(range>=0 && range<nfpzero){
                ui->pushButton_42->setText("0");
                  ui->dial_2->setValue(range);
                if(!overallci){
            ui->CI5_5->setStyleSheet(styleSheet4);
                    handler->pinchvalve_off();
                }
                if(ventonia2==false){
                    handler->safetyvent_on();
                    QThread::msleep(100);
                    handler->safetyvent_off();
                   ventonia2=true;
                }
                int ia2pro=readsensorvalue();
                ui->label_109->setText(QString::number(ia2pro));
 motoroff();
  handler->speaker_off();
  ia2currentcount=0;
 flag6=true;
            }
            if(range>=nfpzero && range<(nfpzero+nfpone)){
                ui->pushButton_42->setText("1");
                handler->speaker_on(0,0,1,0);

                //footpedalbeep();
                ui->dial_2->setValue(range);
                ui->CI5_5->setStyleSheet(styleSheet3);
                handler->pinchvalve_on();
                 motoroff();
            if(ventonia2==false){
                handler->safetyvent_on();
                QThread::msleep(100);
                handler->safetyvent_off();
                ventonia2=true;
            }
            ia2currentcount=1;
}
            else if(range>=(nfpzero+nfpone) && range <(nfpzero+nfpone+nfptwo+nfpthree)){
                ui->pushButton_42->setText("2");
                ventonia2=false;
                //footpedalbeep();
                ui->dial_2->setValue(range);
                ui->CI5_5->setStyleSheet(styleSheet3);
                handler->pinchvalve_on();

                              int nonlinear_prevac = readsensorvalue(); // Assuming this function reads the current sensor value
                              int nonlinear_vac = std::min(nonlinear_prevac, static_cast<int>(ia2vacline));
                              ui->label_109->setText(QString::number(nonlinear_vac));
                              motoron(ui->lineEdit_67);
                              handler->speaker_on(nonlinear_prevac,1,0,0);
                              if (nonlinear_prevac >= ia2vacline) {
                                  motoroff(); // Turn off the motor
                                  handler->speaker_on(0,0,0,1);
                              }
ia2currentcount=2;

            }

            }
            else if(ia2 == "Surgeon"){
                if(range>=0 && range<nfpzero){
                    ui->pushButton_42->setText("0");
                      ui->dial_2->setValue(range);
                    if(!overallci){
                 ui->CI5_5->setStyleSheet(styleSheet4);
                        handler->pinchvalve_off();
                    } handler->speaker_off();
                      motoroff();
                      if(ventonia2==false){
                        handler->safetyvent_on();
                        QThread::msleep(100);
                        handler->safetyvent_off();
                        ventonia2=true;
                      }
                      int ia2pro=readsensorvalue();
                      ui->label_109->setText(QString::number(ia2pro));
                      ia2currentcount=0;
                }
                if(range>=nfpzero && range<(nfpone+nfpzero)){
                    ui->pushButton_42->setText("1");
                    handler->speaker_on(0,0,1,0);

                    //footpedalbeep();
                      ui->dial_2->setValue(range);
                 ui->CI5_5->setStyleSheet(styleSheet3);
                    handler->pinchvalve_on();
                      motoroff();
                      if(ventonia2==false){
                          handler->safetyvent_on();
                                           QThread::msleep(100);
                                           handler->safetyvent_off();
                                           ventonia2=true;
                      }
                      int ia2pro=readsensorvalue();
                      ui->label_109->setText(QString::number(ia2pro));
                      ia2currentcount=1;
                }
                else if(range>=(nfpzero+nfpone) && range <(nfpone+nfpzero+nfptwo+nfpthree)){
                    ui->pushButton_42->setText("2");
                    ventonia2=false;
                    //footpedalbeep();
                      ui->dial_2->setValue(range);
                  ui->CI5_5->setStyleSheet(styleSheet3);
                    handler->safetyvent_off();
                    handler->pinchvalve_on();
                    const int MIN_RANGE = nfpzero + nfpone;
                    const int MAX_RANGE =nfpzero + nfpone + nfptwo + nfpthree;
                    int divi =  MAX_RANGE-MIN_RANGE ; // Divider
                    int calibration = range - MIN_RANGE; // Calibration value

                    if (divi != 0) {
                        double final =ia2vacline / divi;
                        int presetvac = static_cast<int>(std::round(calibration * final));
                        int pro =readsensorvalue();
                        handler->speaker_on(presetvac,1,0,0);
                        ui->label_109->setText(QString::number(pro));
                        if (pro < presetvac) {
                            motoron(ui->lineEdit_67);
                            if (!motoria2) {
                                motoron(ui->lineEdit_67);
                                motoria2 = true;
                            }
                        } else if (motoria2) {
                            motoroff();
                            motoria2 = false;
                        }
                        // If pro exceeds vitpreset, cap it
                        if (pro >= ia2vacline) {
                            pro = static_cast<int>(ia2vacline);
                            ui->label_118->setText(QString::number(pro));
                            motoroff();
                            handler->speaker_on(0,0,0,1);
                        }
                }
                    ia2currentcount=2;
                      }



       }
            if (ia2currentcount>ia2count) {
                beepsound();
            }
            if (ia2currentcount != -1) {
                ia2count = ia2currentcount;
            }
            break;
        }
        // vit
    case 7: {
        int pow7 = ui->lineEdit_71->text().toInt();
       QString vvac = ui->vitvacmode->text();  // "Surgeon" or "Panel"
        double vitpreset = ui->lineEdit_73->text().toDouble();  // vitpreset should be double for better precision
   int vitcurrentcount=-1;
        if (range > 0 && range < nfpzero) {
            // State 0
            ui->pushButton_42->setText("0");
            ui->dial_2->setValue(range);

            if (!overallci) {
                ui->CI5_5->setStyleSheet(styleSheet4);
                handler->pinchvalve_off();
            }
            if (!ventonvit) {
                handler->safetyvent_on();
                QThread::msleep(100);
                handler->safetyvent_off();
                ventonvit = true;
            }

            motoroff();
            ui->label_119->setText("0");
            int vitpro = readsensorvalue();
            ui->label_118->setText(QString::number(vitpro));
 handler->speaker_off();
 vitcurrentcount=0;
            handler->vit_off();
        } else if (range >= nfpzero && range < (nfpzero + nfpone)) {
            // State 1
            ui->pushButton_42->setText("1");
            handler->speaker_on(0,0,1,0);
            //footpedalbeep();
            ui->dial_2->setValue(range);
            ui->CI5_5->setStyleSheet(styleSheet3);

            if (!ventonvit) {
                handler->safetyvent_on();
                QThread::msleep(100);
                handler->safetyvent_off();
                ventonvit = true;
            }

            handler->pinchvalve_on();
            motoroff();
            int vitpro = readsensorvalue();
            ui->label_118->setText(QString::number(vitpro));
            ui->label_119->setText("0");
            handler->vit_off();
            vitcurrentcount=1;
        } else if (range >= (nfpzero + nfpone) && range < (nfpzero + nfpone + nfptwo)) {
            // State 2
            ui->pushButton_42->setText("2");
            ventonvit = false;
            //footpedalbeep();
            ui->dial_2->setValue(range);
            ui->CI5_5->setStyleSheet(styleSheet3);
            handler->safetyvent_off();
            handler->pinchvalve_on();

            if (vvac == "Surgeon") {
                const int MIN_RANGE = nfpzero + nfpone;
                const int MAX_RANGE = nfpzero + nfpone + nfptwo;
                int divi = MAX_RANGE - MIN_RANGE;  // Divider
                int calibration = range - MIN_RANGE;  // Calibration value

                if (divi != 0) {
                    double final = static_cast<double>(vitpreset) / static_cast<double>(divi);
                    int presetvac = static_cast<int>(std::round(calibration * final));
                    int pro = readsensorvalue();
                    ui->label_118->setText(QString::number(pro));
                    handler->speaker_on(presetvac,1,0,0);

                    // Motor control based on preset value
                    if (pro < presetvac) {
                        motoron(ui->lineEdit_72);
                        if (!motorvit) {
                            motorvit = true;
                        }
                    } else if (motorvit) {
                        motoroff();
                        motorvit = false;
                    }

                    // If pro exceeds vitpreset, cap it
                    if (pro >= vitpreset) {
                        pro = static_cast<int>(vitpreset);
                        ui->label_118->setText(QString::number(pro));
                        motoroff();
                        handler->speaker_on(0,0,0,1);
                    }
                }

            } else if (vvac == "Panel") {
                int nonlinear_prevac = readsensorvalue();  // Current sensor value
                int nonlinear_vac = std::min(nonlinear_prevac, static_cast<int>(vitpreset));
                ui->label_118->setText(QString::number(nonlinear_vac));
                handler->speaker_on(nonlinear_prevac,1,0,0);

                // Motor control for Panel
                motoron(ui->lineEdit_72);
                if (nonlinear_prevac >= vitpreset) {
                    motoroff();
                    handler->speaker_on(0,0,0,1);

                }
            }

            ui->label_119->setText("0");
            handler->vit_off();
             vitcurrentcount=2;
        } else if (range >= (nfpzero + nfpone + nfptwo) && range < (nfpzero + nfpone + nfptwo + nfpthree)) {
            // State 3
            ui->pushButton_42->setText("3");
            ventonvit = false;
            //footpedalbeep();
            ui->dial_2->setValue(range);
            ui->CI5_5->setStyleSheet(styleSheet3);
            handler->safetyvent_off();
            handler->pinchvalve_on();

            if (vitonoff) {  // vitonoff check for vit actions
                float progress4 = static_cast<float>(range - (nfpzero + nfpone + nfptwo)) /
                                  static_cast<float>(4096 - (nfpzero + nfpone + nfptwo)) * static_cast<float>(pow7);
                ui->label_119->setText(QString::number(static_cast<int>(progress4)));

                handler->vit_on(static_cast<float>(pow7));
                handler->vit_ontime(30);
            }

            if (vvac == "Surgeon") {
                const int MIN_RANGE = nfpzero + nfpone + nfptwo;
                const int MAX_RANGE = nfpzero + nfpone + nfptwo + nfpthree;
                int divi = MAX_RANGE - MIN_RANGE;
                int calibration = range - MIN_RANGE;

                if (divi != 0) {
                    double final = static_cast<double>(vitpreset) / static_cast<double>(divi);
                    int presetvac = static_cast<int>(std::round(calibration * final));
                    int pro = readsensorvalue();
                    ui->label_118->setText(QString::number(pro));
                    handler->speaker_on(presetvac,1,0,0);
                    if (pro < presetvac) {
                        motoron(ui->lineEdit_72);
                        if (!motorvit) {
                            motorvit = true;
                        }
                    } else if (motorvit) {
                        motoroff();
                        motorvit = false;
                    }

                    if (pro >= vitpreset) {
                        pro = static_cast<int>(vitpreset);
                        ui->label_118->setText(QString::number(pro));
                        motoroff();
                    }
                }
            } else if (vvac == "Panel") {
                int nonlinear_prevac = readsensorvalue();
                int nonlinear_vac = std::min(nonlinear_prevac, static_cast<int>(vitpreset));
                ui->label_118->setText(QString::number(nonlinear_vac));
                handler->speaker_on(nonlinear_prevac,1,0,0);
                motoron(ui->lineEdit_72);
                if (nonlinear_prevac >= vitpreset) {
                    motoroff();
                }
            }
             vitcurrentcount=3;
        }
        if (vitcurrentcount>vitcount) {
            beepsound();
        }
        if (vitcurrentcount != -1) {
            vitcount = vitcurrentcount;
        }
        break;
    }


    currentTimer.restart();
    updateLabel();
    }

}
//us1 non linear and linear
void MainWindow::us1_linear_nonlinear()
{
     handler->buzz();
     // ui->us1mode->setText("SURGEON");
     us1=ui->us1mode->text();

       if(us1 == "Surgeon" )
       {
           ui->us1mode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
            ui->us1mode->setText("Panel");

             footpedalcheck();


       }
       else if(us1 == "Panel" )
       {
           ui->us1mode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
           ui->us1mode->setText("Surgeon");


     footpedalcheck();

}
}
void MainWindow::on_us1vacmode_clicked()
{
     handler->buzz();
    vus1 = ui->us1vacmode->text();
       if(vus1== "Surgeon")
       {

            ui->us1vacmode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
            ui->us1vacmode->setText("Panel");

             footpedalcheck();
       }
       else if(vus1=="Panel")
       {

           ui->us1vacmode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
           ui->us1vacmode->setText("Surgeon");
         //  int pow2=ui->lineEdit_58->text().toInt();
     footpedalcheck();
}

}


void MainWindow::us2_linear_nonlinear()
{
     handler->buzz();
    us2 = ui->us2mode->text();


       if(us2 == "Surgeon" )
       {

            ui->us2mode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
             ui->us2mode->setText("Panel");


              footpedalcheck();
               }
       else if(us2 == "Panel")
       {
            ui->us2mode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
           ui->us2mode->setText("Surgeon");

         footpedalcheck();
       }

}
void MainWindow::on_us2vacmode_clicked()
{
     handler->buzz();
    vus2=ui->us2vacmode->text();
    if(vus2 == "Surgeon")
    {

      ui->us2vacmode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
          ui->us2vacmode->setText("Panel");

           footpedalcheck();
            }
    else if(vus2 == "Panel")
    {

        ui->us2vacmode->setText("Surgeon");
         ui->us2vacmode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
      footpedalcheck();
    }

}

void MainWindow::us3_linear_nonlinear()
{
     handler->buzz();
    us3 = ui->us3mode->text();


       if(us3 == "Surgeon")
       {
            ui->us3mode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
             ui->us3mode->setText("Panel");

              footpedalcheck();
       }
       else if(us3 == "Panel")
       {
             ui->us3mode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
           ui->us3mode->setText("Surgeon");

           footpedalcheck();
       }

}
void MainWindow::on_us3vacmode_clicked()
{
     handler->buzz();
     vus3=ui->us3vacmode->text();
     if(vus3 =="Surgeon")
     {

           ui->us3vacmode->setText("Panel");
            ui->us3vacmode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
            footpedalcheck();
     }
     else if(vus3 == "Panel")
     {

         ui->us3vacmode->setText("Surgeon");
         ui->us3vacmode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
         footpedalcheck();
     }

}

void MainWindow::us4_linear_nonlinear()
{
     handler->buzz();
    us4 = ui->us4mode->text();

       if(us4 == "Surgeon")
       {

            ui->us4mode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
             ui->us4mode->setText("Panel");

             footpedalcheck();
                }
       else if(us4 == "Panel")
       {
           ui->us4mode->setText("Surgeon");
              ui->us4mode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");


               footpedalcheck();
       }

}

void MainWindow::on_us4vacmode_clicked()
{
     handler->buzz();
    vus4=ui->us4vacmode->text();
    if(vus4 == "Surgeon")
    {


          ui->us4vacmode->setText("Panel");
           ui->us4vacmode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
          footpedalcheck();
             }
    else if(vus4 == "Panel")
    {

           ui->us4vacmode->setText("Surgeon");
            ui->us4vacmode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
            footpedalcheck();
    }

}



void MainWindow::ia1_linear_nonlinear()
{
     handler->buzz();
    ia1= ui->ia1mode->text();

       if(ia1== "Surgeon")
       {

            ui->ia1mode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
             ui->ia1mode->setText("Panel");
            footpedalcheck();
               }
       else if(ia1 == "Panel")
       {
            ui->ia1mode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
           ui->ia1mode->setText("Surgeon");
           footpedalcheck();
               }
   }
void MainWindow::ia2_linear_nonlinear()
{
     handler->buzz();
    ia2 = ui->ia2mode->text();

       if(ia2 == "Surgeon")
       {
           ui->ia2mode->setText("Panel");
            ui->ia2mode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
             footpedalcheck();
                }
       else if(ia2 == "Panel")
       {
           ui->ia2mode->setText("Surgeon");
            ui->ia2mode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
 footpedalcheck();
       }

}
void MainWindow::vit_linear_nonlinear()
{
     handler->buzz();
    vit = ui->vitmode->text();


       if(vit == "Surgeon" )
       {

           ui->vitmode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
            ui->vitmode->setText("Panel");

            footpedalcheck();
               }
       else if(vit == "Panel")
       {
              ui->vitmode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
           ui->vitmode->setText("Surgeon");

           footpedalcheck();
              }
      }
void MainWindow::on_vitvacmode_clicked()
{
     handler->buzz();
      vvit=ui->vitvacmode->text();

      if(vvit =="Surgeon")
      {


           ui->vitvacmode->setText("Panel");
           ui->vitvacmode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
           footpedalcheck();
              }
      else if(vvit == "Panel")
      {

          ui->vitvacmode->setText("Surgeon");
          ui->vitvacmode->setStyleSheet("font:15pt;image: url(:/images/panelbutton.png);background-color:transparent;border:none");
          footpedalcheck();
             }

}

//pulse
void MainWindow::pulseup_mode()
{
 handler->buzz();
    pulse = ui->lineEdit_75->text().toInt();
    pulse += 1;
    if (pulse > 15) {
        pulse = 15;
    }
    ui->lineEdit_75->setText(QString::number(pulse));
    handler->freq_count(2500);
    handler->fs_count(nfpzero+nfpone+nfptwo);
    handler->pulse_count(pulse);

  handler->pdm_mode(PULSE_MODE);
  //qDebug()<<pulse;
}

void MainWindow::pulsedown_mode()
{
 handler->buzz();
    pulse = ui->lineEdit_75->text().toInt();
   pulse -= 1;
    if (pulse < 1) {
        pulse = 1;
    }
    ui->lineEdit_75->setText(QString::number(pulse));
    handler->freq_count(2500);
    handler->fs_count(nfpzero+nfpone+nfptwo);
    handler->pulse_count(pulse);
handler->pdm_mode(PULSE_MODE);
//qDebug()<<pulse;
}
//ocuburst
void MainWindow::ocuburstup_mode()
{


}

void MainWindow::ocuburstdown_mode()
{


}
//singleburst
void MainWindow::singleburstup_mode()
{
 handler->buzz();
    singleburst = ui->lineEdit_78->text().toInt();
    singleburst += 10;
    if (singleburst > 400) {
        singleburst = 400;
    }
    ui->lineEdit_78->setText(QString::number(singleburst));
    handler->freq_count(2500);
    handler->fs_count(nfpzero+nfpone+nfptwo);
     handler->pdm_mode(SINGLE_BURST);
    handler->burst_length(singleburst);
    handler->burst_off_length(singleburst);
      //qDebug()<<singleburst;
}

void MainWindow::singleburstdown_mode()
{
 handler->buzz();
    singleburst = ui->lineEdit_78->text().toInt();
    singleburst -= 10;
    if (singleburst < 10) {
        singleburst = 10;
    }
    ui->lineEdit_78->setText(QString::number(singleburst));
    handler->freq_count(2500);
    handler->fs_count(nfpzero+nfpone+nfptwo);
     handler->pdm_mode(SINGLE_BURST);
    handler->burst_length(singleburst);
    handler->burst_off_length(singleburst);
    //qDebug()<<singleburst;
}
//multiburst
void MainWindow::multiburstup_mode()
{
 handler->buzz();
    multiburst = ui->lineEdit_79->text().toInt();
    multiburst += 10;
    if (multiburst > 400) {
        multiburst = 400;
    }
    ui->lineEdit_79->setText(QString::number(multiburst));
    handler->fs_count(nfpzero+nfpone+nfptwo);
    handler->freq_count(2500);
    handler->pdm_mode(MULTI_BURST);
    handler->burst_length(multiburst);
    handler->burst_off_length(multiburst);
}

void MainWindow::multiburstdown_mode()
{
 handler->buzz();
    multiburst = ui->lineEdit_79->text().toInt();
    multiburst -= 10;
    if (multiburst < 10) {
        multiburst = 10;
    }
    ui->lineEdit_79->setText(QString::number(multiburst));
    handler->fs_count(nfpzero+nfpone+nfptwo);
    handler->freq_count(2500);
    handler->pdm_mode(MULTI_BURST);
    handler->burst_length(multiburst);
    handler->burst_off_length(multiburst);
}
//ocupulse
void MainWindow::ocupulseup_mode()
{



}

void MainWindow::ocupulsedown_mode()
{

}
//coldphaco
void MainWindow::coldphacoup_mode()
{
 handler->buzz();
    coldphaco = ui->lineEdit_80->text().toInt();
    coldphaco += 1;
    if (coldphaco > 15) {
        coldphaco = 15;
    }
    ui->lineEdit_80->setText(QString::number(coldphaco));
    handler->cold_pulse(coldphaco);
    handler->pdm_mode(COLD_PHACO);
}

void MainWindow::coldphacodown_mode()
{
     handler->buzz();
    coldphaco = ui->lineEdit_80->text().toInt();
    coldphaco -= 1;
    if (coldphaco < 1) {
        coldphaco = 1;
    }
    ui->lineEdit_80->setText(QString::number(coldphaco));
    handler->cold_pulse(coldphaco);
     handler->pdm_mode(COLD_PHACO);
}

void MainWindow::coldphaco1up_mode()
{
    handler->buzz();
    coldphaco1 = ui->lineEdit_81->text().toInt();
    coldphaco1 += 5;
    if (coldphaco1 > 40) {
        coldphaco1 = 40;
    }
    ui->lineEdit_81->setText(QString::number(coldphaco1));
    handler->cold_pulse(coldphaco1);
     handler->pdm_mode(COLD_PHACO);
}

void MainWindow::coldphaco1down_mode()
{
    handler->buzz();
    coldphaco1 = ui->lineEdit_81->text().toInt();
    coldphaco1 -= 5;
    if (coldphaco1 < 5) {
        coldphaco1 = 5;
    }
    ui->lineEdit_81->setText(QString::number(coldphaco1));
    handler->cold_pulse(coldphaco1);
     handler->pdm_mode(COLD_PHACO);
}


//handpiece connected or not
void MainWindow::exportGPIO(int pin)
{
    QFile file("/sys/class/gpio/export");
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << pin;
        file.close();
      }
}

void MainWindow::setGPIODirection(const QString &direction,int pin)
{
    QFile file(QString("/sys/class/gpio/gpio%1/direction").arg(pin));
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << direction;
        file.close();
   }
}

int MainWindow::readGPIOValue(int pin)
{
    QFile file(QString("/sys/class/gpio/gpio%1/value").arg(pin));
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        int value;
        stream >> value;
        file.close();
        ////qDebug() << "Read value from GPIO" << gpioNumber << ":" << value;
        return value;
    }
}
void MainWindow::updatehandpieceStatus()
{
    int status = readGPIOValue(960);


    QString styleSheet4 = "QPushButton {"
                "image: url(:/images/handpiece1.png);"
            "background-color:green;"

                "border: 4px solid black;"
                "border-radius: 20px;"
                                     "}";


    QString styleSheet5 = "QPushButton {"
                "image: url(:/images/handpiece1.png);"

             "background-color: rgb(192, 28, 40);"
                "border: 4px solid black;"
                "border-radius: 20px;"
                                     "}";
    QString styleSheet6 = "QLabel {"
               "image: url(:/images/locked.png);"
 "background-color:transparent;"

                                     "}";
    QString styleSheet7 = "QLabel {"
               "image: url(:/images/unlocked.png);"

                "background-color:transparent;"

                                     "}";
   if(status==0)
   {
   ui->pushButton->setStyleSheet(styleSheet4);


   }
   else if(status==1)
   {
     ui->pushButton->setStyleSheet(styleSheet5);

   }
}
void MainWindow::onUpdateStatusTimeout(){
    updatehandpieceStatus();
}

void MainWindow::linearvaccum()
{
    QString mode=ui->ia1mode->text();
    int ia1preset=ui->lineEdit_70->text().toInt();
    //int pro=readsensorvalue();
    if(mode=="Surgeon"){
        // Calculate calibration parameters


        }
        motoroff();
        //timerfoot->stop();



}
void MainWindow::updateProgressBar() {

}

void MainWindow::motoron(QLineEdit *lineEdit)
{
    int linevalue=lineEdit->text().toInt();
    handler->write_motor(0x01,0x03,linevalue);
    ////qDebug()<<"motor working";
}

void MainWindow::motoroff()
{
    handler->write_motor(0x00,0x00,0);
}

void MainWindow::motorccwon()
{
    handler->write_motor(0x01,0x02,40);
}
//roundoff value

int MainWindow::increasebutton(int input)
{
    if (input < 10) {
          // For single-digit values, set it to 5
          return 5;
      } else {
          int remainder = input % 5; // Calculate the remainder when dividing by 5

          if (remainder == 0 || remainder == 1) {
              // Round down to the nearest multiple of 5
              return (input / 5) * 5;
          } else {
              // Round up to the next multiple of 5
              return ((input / 5) + 1) * 5;
          }
      }
  }


int MainWindow::decreasebutton(int input)
{
    if (input % 2 != 0) {
          input++;  // Increment if odd to make it even
      }
      return input;
}



void MainWindow::sensor2()
{

//    int sensor = vac->convert(0xD7);
//     int vac1=ui->lineEdit_55->text().toInt();
//      int vac2=ui->lineEdit_60->text().toInt();
//       int vac3=ui->lineEdit_63->text().toInt();
//        int vac4=ui->lineEdit_66->text().toInt();
//         int vac5=ui->lineEdit_70->text().toInt();
//          int vac6=ui->lineEdit_68->text().toInt();
//           int vac7=ui->lineEdit_73->text().toInt();
//           QString mode=ui->CutMode_vitCom->currentText();
//           QString mode1=ui->CutMode_vitCom_2->currentText();
//           QString mode2=ui->CutMode_vitCom_3->currentText();
//           QString mode3=ui->CutMode_vitCom_4->currentText();
////    if (sensor > 0 && sensor < 4096) {
////      //  int pro = sensor * vac5 / 4096+50;
////        double pro=sensor*vac5/4090;
////       ui->label_113->setText(QString::number(static_cast<int>(pro)));
////        if(vac5==pro){
////        motoroff();
////          handler->speaker_on(pro,0,0,1);
////    }
////    }
//    if (sensor > 0 && sensor < 4096) {
//        int pro1 = sensor * vac6 / 4096;
//        ui->label_109->setText(QString::number(pro1));
//        if(vac6 == pro1){
//            motoroff();
//              handler->speaker_on(pro1,0,0,1);
//        }
//    }
//    if (sensor > 0 && sensor < 4096) {
//        int pro3 = sensor * vac7 / 4096;
//        ui->label_118->setText(QString::number(pro3));
//        if(vac7 == pro3){
//           motoroff();
//              handler->speaker_on(pro3,0,0,1);
//        }
//    }
//    //us4
//    if (sensor > 0 && sensor < 4096) {
//        int pro4 = sensor * 500 / 4096;
//        pro4=std::min(vac4,pro4);
//        ui->label_104->setText(QString::number(pro4));
//        if(vac4 == pro4){
//   motoroff();
//            if(mode3 == "Ocupulse"){
//              handler->pdm_mode(OCUPULSE);
//              ocupulseup_mode();
//              ocupulsedown_mode();
//            }
//            else if(mode3=="Ocuburst"){
//                handler->pdm_mode(OCUBURST);
//                ocuburstup_mode();
//                ocuburstdown_mode();
//            }
//            else if(mode3 == "Multi burst"){
//                handler->pdm_mode(MULTI_BURST);
//                multiburstup_mode();
//                multiburstdown_mode();

//            }

//               handler->speaker_on(pro4,0,0,1);
//        }
//    }
//    //us3
//    if (sensor > 0 && sensor < 4096) {
//        int pro5 = sensor * 500 / 4096;
//        pro5=std::min(vac3,pro5);
//        ui->label_99->setText(QString::number(pro5));
//        if(vac3 == pro5){
//    motoroff();
//            if(mode3 == "Ocupulse"){
//              handler->pdm_mode(OCUPULSE);
//              ocupulseup_mode();
//              ocupulsedown_mode();
//            }
//            else if(mode3=="Ocuburst"){
//                handler->pdm_mode(OCUBURST);
//                ocuburstup_mode();
//                ocuburstdown_mode();
//            }
//            else if(mode3 == "Multi burst"){
//                handler->pdm_mode(MULTI_BURST);
//                multiburstup_mode();
//                multiburstdown_mode();

//            }

//               handler->speaker_on(pro5,0,0,1);
//        }
//    }
//    //us2
//    if (sensor > 0 && sensor < 4096) {
//        int pro6 = sensor * 500 / 4096;
//        pro6=std::min(vac2,pro6);
//        ui->label_93->setText(QString::number(pro6));
//        if(vac2==pro6){
//           motoroff();

//            if(mode3 == "Ocupulse"){
//              handler->pdm_mode(OCUPULSE);
//              ocupulseup_mode();
//              ocupulsedown_mode();
//            }
//            else if(mode3=="Ocuburst"){
//                handler->pdm_mode(OCUBURST);
//                ocuburstup_mode();
//                ocuburstdown_mode();
//            }
//            else if(mode3 == "Multi burst"){
//                handler->pdm_mode(MULTI_BURST);
//                multiburstup_mode();
//                multiburstdown_mode();

//            }
//             handler->speaker_on(pro6,0,0,1);

//        }
//    }
    //us1
//    if (sensor > 0 && sensor < 4096) {
//        int pro7 = sensor *vac1 / 4096;
//        ui->label_8->setText(QString::number(pro7));
//        if(vac1 == pro7){
//            motoroff();
//            if(mode3 == "Ocupulse"){
//                handler->freq_count(2500);
//                handler->fs_count(3000);
//            handler->pdm_mode(PULSE_MODE);
//              ocupulseup_mode();
//              ocupulsedown_mode();
//            }
//            else if(mode3=="Ocuburst"){
//                handler->freq_count(2500);
//                handler->fs_count(3000);
//                handler->pdm_mode(PULSE_MODE);
//                ocuburstup_mode();
//                ocuburstdown_mode();
//            }
//            else if(mode3 == "Multi burst"){
//                handler->freq_count(2500);
//                handler->fs_count(3000);
//                 handler->pdm_mode(PULSE_MODE);
//                multiburstup_mode();
//                multiburstdown_mode();

//            }
//             handler->speaker_on(pro7,0,0,1);


//    }
//    }

    }

void MainWindow::updateTimers()
{
    if (currentButton != -1) {
        qint64 elapsed = currentTimer.elapsed();
        qint64 totalTime = elapsedTimeUS1 + elapsedTimeUS2 + elapsedTimeUS3 + elapsedTimeUS4;

        switch (currentButton) {
            case 1:
                totalTime += elapsed;
                break;
            case 2:
                totalTime += elapsed;
                break;
            case 3:
                totalTime += elapsed;
                break;
            case 4:
                totalTime += elapsed;
                break;
        }

        ui->elapsed_time->setText(QString::number(totalTime / 1000.0, 'f', 2) + " s");
    }
}
void MainWindow::updateLabel()
{
    qint64 totalTime = elapsedTimeUS1 + elapsedTimeUS2 + elapsedTimeUS3 + elapsedTimeUS4;
    ui->elapsed_time->setText(QString::number(totalTime / 1000.0, 'f', 2) + " s");
}

void MainWindow::changesvaluesql()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(PATH);

    if (!db.open()) {
        //qDebug() << "SQL database failed to open";
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE phacohigh SET "
                  "Quadvacmode = :Quadvacmode, Quadpowermethod = :Quadpowermethod, "
                  "Quadpowmode = :Quadpowmode, "
                  "Chopvacmode = :Chopvacmode, Choppowermethod = :Choppowermethod, Choppowmode = :Choppowmode, "
                  "Sculptvacmode = :Sculptvacmode, Sculptpowermethod = :Sculptpowermethod, Sculptpowmode = :Sculptpowmode, "
                  "cortexvacmode = :cortexvacmode, "
                  "polishvacmode = :polishvacmode, "
                  "vitcutmode = :vitcutmode, vitvacmode = :vitvacmode, "
                  "Epinpowermethod = :Epinpowermethod, Epinpowmode = :Epinpowmode, Epinvacmode = :Epinvacmode "
                  "WHERE surgeon = :surgeon");

    // Binding values from UI elements to the query
  query.bindValue(":Quadvacmode", ui->us2vacmode->text());
    query.bindValue(":Quadpowermethod", ui->CutMode_vitCom_2->currentText());
  query.bindValue(":Quadpowmode", ui->us2mode->text());

   query.bindValue(":Chopvacmode", ui->us3vacmode->text());
    query.bindValue(":Choppowermethod", ui->CutMode_vitCom_3->currentText());
    query.bindValue(":Choppowmode", ui->us3mode->text());

    query.bindValue(":Sculptvacmode", ui->us4vacmode->text());
    query.bindValue(":Sculptpowermethod", ui->CutMode_vitCom_4->currentText());
    query.bindValue(":Sculptpowmode", ui->us4mode->text());

    query.bindValue(":cortexvacmode", ui->ia1mode->text());  // Assuming a corresponding UI element
    query.bindValue(":polishvacmode", ui->ia2mode->text());  // Assuming a corresponding UI element
    query.bindValue(":vitcutmode", ui->vitmode->text());        // Assuming a corresponding UI element
    query.bindValue(":vitvacmode", ui->vitvacmode->text());        // Assuming a corresponding UI element

    query.bindValue(":Epinpowermethod", ui->CutMode_vitCom->currentText());
    query.bindValue(":Epinpowmode", ui->us1mode->text());
    query.bindValue(":Epinvacmode", ui->us1vacmode->text());

    query.bindValue(":surgeon", ui->comboBox_4->currentText());

    // Execute the query and check for errors
    if (!query.exec()) {
        //qDebug() << "Update failed: " << query.lastError();
    } else {
        //qDebug() << "Update successful";
    }

    db.close(); // Close the database connection
}

void MainWindow::changebuttonstyle()
{
    ui->label_19->show();
}

void MainWindow::moved(int gpio)
{
    QPushButton *buttons[] = {
        ui->DIABUT, ui->ULTRASONICBUT1, ui->ULTRASONICBUT2,
        ui->ULTRASONICBUT3, ui->ULTRASONICBUT4, ui->IA1BUT,
        ui->IA2BUT, ui->VITRECTOMYBUT
    };
    int totalButtons = sizeof(buttons) / sizeof(buttons[0]);

    if (gpio == 0) {
        if (currentButtonIndex == -1) {
            currentButtonIndex = 0;
        } else {
            currentButtonIndex = (currentButtonIndex + 1) % totalButtons;
        }
    }
    // Simulate a click on the current button
    buttons[currentButtonIndex]->click();
    buttons[currentButtonIndex]->setFocus();
}

void MainWindow::movePushButtonBottomToTop(int gpio) {
    QPushButton *buttons[] = {
        ui->DIABUT, ui->ULTRASONICBUT1, ui->ULTRASONICBUT2,
        ui->ULTRASONICBUT3, ui->ULTRASONICBUT4, ui->IA1BUT,
        ui->IA2BUT, ui->VITRECTOMYBUT
    };
    int totalButtons = sizeof(buttons) / sizeof(buttons[0]);

    if (gpio == 0) {
        if (currentButtonIndex == -1) {
            currentButtonIndex = totalButtons - 1;
        } else {
            currentButtonIndex = (currentButtonIndex - 1 + totalButtons) % totalButtons;
        }
    }
    buttons[currentButtonIndex]->click();
     buttons[currentButtonIndex]->setFocus();
}

void MainWindow::onPdmModeSelected(int gpio) {
    if (gpio == 0 && us1PdmMode) {
        moveTab(1);  // Only move tabs if us1 is in PDM mode
        int index=ui->CutMode_vitCom->currentIndex();
        onCutMode_vitComChanged(index);
    }
}

void MainWindow::onPdmModeSelected1(int gpio) {
    if (gpio == 0 && us2PdmMode) {
        moveTab1(2);  // Only move tabs if us2 is in PDM mode
        int index1=ui->CutMode_vitCom_2->currentIndex();
        onCutMode_vitComChanged1(index1);
    }
}

void MainWindow::onPdmModeSelected2(int gpio) {
    if (gpio == 0 && us3PdmMode) {
        moveTab2(3);  // Only move tabs if us3 is in PDM mode
        int index=ui->CutMode_vitCom_3->currentIndex();
        onCutMode_vitComChanged2(index);
    }
}

void MainWindow::onPdmModeSelected3(int gpio) {
    if (gpio == 0 && us4PdmMode) {
        moveTab3(4);  // Only move tabs if us4 is in PDM mode
        int index=ui->CutMode_vitCom_4->currentIndex();
        onCutMode_vitComChanged3(index);
    }
}

void MainWindow::moveTab(int usIndex) {
    if (!ui->tabWidget_2) {
           //qDebug() << "tabWidget_2 is null!";
           return;
       }

       int currentIndex = ui->tabWidget_2->currentIndex();
       int tabCount = ui->tabWidget_2->count();

       if (tabCount == 0) {
           //qDebug() << "No tabs available!";
           return;
       }

       // Update tab index based on usIndex
       if (usIndex == 1 || usIndex == 2) {
           currentIndex = (currentIndex + 1) % tabCount;
       } else if (usIndex == 3 || usIndex == 4) {
           currentIndex = (currentIndex - 1 + tabCount) % tabCount;
       } else {
           //qDebug() << "Invalid usIndex value:" << usIndex;
           return;
       }


       // Update combo boxes based on usIndex
       if (usIndex == 1) {
           if (ui->CutMode_vitCom) {
               ui->CutMode_vitCom->setCurrentIndex(currentIndex);

onCutMode_vitComChanged(ui->CutMode_vitCom->currentIndex());
ui->tabWidget_2->setCurrentIndex(currentIndex);

           }

       }

       //qDebug() << "Moved to tab index:" << currentIndex << " and updated combo box for usIndex:" << usIndex;

}

void MainWindow::moveTab1(int usIndex) {
    if (!ui->tabWidget_2) {
        //qDebug() << "tabWidget_2 is null!";
        return;
    }

    int currentIndex = ui->tabWidget_2->currentIndex();
    int tabCount = ui->tabWidget_2->count();

    if (tabCount == 0) {
        //qDebug() << "No tabs available!";
        return;
    }

    // Update tab index based on usIndex
    if (usIndex == 1 || usIndex == 2) {
        currentIndex = (currentIndex + 1) % tabCount;
    } else if (usIndex == 3 || usIndex == 4) {
        currentIndex = (currentIndex - 1 + tabCount) % tabCount;
    } else {
        //qDebug() << "Invalid usIndex value:" << usIndex;
        return;
    }



    // Update combo boxes based on usIndex
   if (usIndex == 2) {
        if (ui->CutMode_vitCom_2) {
            ui->CutMode_vitCom_2->setCurrentIndex(currentIndex);
onCutMode_vitComChanged1(ui->CutMode_vitCom_2->currentIndex());
ui->tabWidget_2->setCurrentIndex(currentIndex);
        }
    }

    //qDebug() << "Moved to tab index:" << currentIndex << " and updated combo box for usIndex:" << usIndex;
}
void MainWindow::moveTab2(int usIndex) {
    if (!ui->tabWidget_2) {
        //qDebug() << "tabWidget_2 is null!";
        return;
    }

    int currentIndex = ui->tabWidget_2->currentIndex();
    int tabCount = ui->tabWidget_2->count();

    if (tabCount == 0) {
        //qDebug() << "No tabs available!";
        return;
    }

    // Update tab index based on usIndex
    if (usIndex == 1 || usIndex == 2) {
        currentIndex = (currentIndex + 1) % tabCount;
    } else if (usIndex == 3 || usIndex == 4) {
        currentIndex = (currentIndex - 1 + tabCount) % tabCount;
    } else {
        //qDebug() << "Invalid usIndex value:" << usIndex;
        return;
    }



    // Update combo boxes based on usIndex
   if (usIndex == 3) {
        if (ui->CutMode_vitCom_3) {
            ui->CutMode_vitCom_3->setCurrentIndex(currentIndex);
onCutMode_vitComChanged2(ui->CutMode_vitCom_3->currentIndex());
             ui->tabWidget_2->setCurrentIndex(currentIndex);
        }
    }

    //qDebug() << "Moved to tab index:" << currentIndex << " and updated combo box for usIndex:" << usIndex;
}
void MainWindow::moveTab3(int usIndex) {
    if (!ui->tabWidget_2) {
        //qDebug() << "tabWidget_2 is null!";
        return;
    }

    int currentIndex = ui->tabWidget_2->currentIndex();
    int tabCount = ui->tabWidget_2->count();

    if (tabCount == 0) {
        //qDebug() << "No tabs available!";
        return;
    }

    // Update tab index based on usIndex
    if (usIndex == 1 || usIndex == 2) {
        currentIndex = (currentIndex + 1) % tabCount;
    } else if (usIndex == 3 || usIndex == 4) {
        currentIndex = (currentIndex - 1 + tabCount) % tabCount;
    } else {
        //qDebug() << "Invalid usIndex value:" << usIndex;
        return;
    }


    // Update combo boxes based on usIndex
   if (usIndex == 4) {
        if (ui->CutMode_vitCom_4) {
            ui->CutMode_vitCom_4->setCurrentIndex(currentIndex);
    onCutMode_vitComChanged3(ui->CutMode_vitCom_4->currentIndex());

            ui->tabWidget_2->setCurrentIndex(currentIndex);

        }
    }

    //qDebug() << "Moved to tab index:" << currentIndex << " and updated combo box for usIndex:" << usIndex;
}
// Member variable to keep track of the active combo box



void MainWindow::footreflux(int gpio)
{
    bool flag=gpio;
    if(flag==0){
  motorccwon();
  flag=1;
  }
    else{
        motoroff();
        flag=0;
    }
}

void MainWindow::continousirrigation(int value) {
    QString styleSheetOn = "QPushButton {"
                           "font: 20pt Ubuntu;"
                           "background-color: green;"
                           "color: black;"
                           "image: url(:/images/ci.png);"
                           "border: 5px solid black;"
                           "border-radius: 30px;"
                           "font-weight: bold;"
                           "}";

    QString styleSheetOff = "QPushButton {"
                            "font: 20pt Ubuntu;"
                            "background-color: red;"
                            "image: url(:/images/ci.png);"
                            "border: 5px solid black;"
                            "border-radius: 30px;"
                            "font-weight: bold;"
                            "}";
 con=value;
    if (con == 0) {
        if (!overallci) {
            // Turn on Continuous Irrigation
            ui->CI5_5->setStyleSheet(styleSheetOn);
            ui->CI5_5->update();
            overallci = true;
            handler->pinchvalve_on();
            handler->safetyvent_on();
            //qDebug() << "Continuous Irrigation turned ON, button color set to green.";
            con=1;
        }
    } else {
        if (overallci) {
            // Turn off Continuous Irrigation
            ui->CI5_5->setStyleSheet(styleSheetOff);
                        ui->CI5_5->update();
            overallci = false;
            handler->pinchvalve_off();
            handler->safetyvent_off();
            //qDebug() << "Continuous Irrigation turned OFF, button color set to red.";
            con=0;
        }
    }


}

void MainWindow::poweronoff(int gpio)
{
bool flag=gpio;
    if (flag == 0) {
          enableButtons(true);
          flag=1;
       }else{
        enableButtons(false);
        flag=0;
    }

}
void MainWindow::on_us1onoff_clicked()
{
    QString styleSheet1 = "QPushButton {"
                          " font:20pt Ubuntu;"
                          " background-color: green;"
                          " color: black;"
                          " border:5px solid black;"
                          " border-radius:30px;font-weight: bold;"
                          "}";

    QString styleSheet2 = "QPushButton {"
                          " font:20pt Ubuntu;"
                          " background-color: rgb(224, 27, 36);"
                          " color: black;"
                          " border:5px solid black;"
                          " border-radius:30px;font-weight: bold;"
                          "}";


    us1poweron = !us1poweron;

    if (us1poweron) {
        ui->us1onoff->setText("ON");  // Turn ON
        ui->us1onoff->setStyleSheet(styleSheet1);
        ui->us2onoff->setText("ON");  // Turn ON
        ui->us2onoff->setStyleSheet(styleSheet1);
        us2poweron=true;
        ui->us3onoff->setText("ON");  // Turn ON
        ui->us3onoff->setStyleSheet(styleSheet1);
        us3poweron=true;
        ui->us4onoff->setText("ON");  // Turn ON
        ui->us4onoff->setStyleSheet(styleSheet1);
        us4poweron=true;
        ui->us1powup_but->setEnabled(true);
        ui->us1powdown_but->setEnabled(true);
        ui->us2powup_but->setEnabled(true);
        ui->us2powdown_but->setEnabled(true);
        ui->us3powup_but->setEnabled(true);
        ui->us3powdown_but->setEnabled(true);
        ui->us4powup_but->setEnabled(true);
        ui->us4powdown_but->setEnabled(true);

        //qDebug()<<"on";
    } else {
        ui->us1onoff->setText("OFF");  // Turn OFF
        ui->us1onoff->setStyleSheet(styleSheet2);
        ui->us2onoff->setText("OFF");  // Turn OFF
        ui->us2onoff->setStyleSheet(styleSheet2);
        us2poweron=false;
        ui->us3onoff->setText("OFF");  // Turn OFF
        ui->us3onoff->setStyleSheet(styleSheet2);
        us3poweron=false;
        ui->us4onoff->setText("OFF");  // Turn OFF
        ui->us4onoff->setStyleSheet(styleSheet2);
        us4poweron=false;
        ui->us1powup_but->setEnabled(false);
        ui->us1powdown_but->setEnabled(false);
        ui->us2powup_but->setEnabled(false);
        ui->us2powdown_but->setEnabled(false);
        ui->us3powup_but->setEnabled(false);
        ui->us3powdown_but->setEnabled(false);
        ui->us4powup_but->setEnabled(false);
        ui->us4powdown_but->setEnabled(false);
        ui->vitpowup_but->setEnabled(false);
        ui->vitpowdown_but->setEnabled(false);
         //qDebug()<<"off";
    }
}




void MainWindow::on_us2onoff_clicked()
{
    QString text = ui->us2onoff->text();


    QString styleSheet1 = "QPushButton {"
                          " font:20pt Ubuntu;"
                          " background-color: green;"
                          " color: black;"
                          " border:5px solid black;"
                          " border-radius:30px;font-weight: bold;"
                          "}";

    QString styleSheet2 = "QPushButton {"
                          " font:20pt Ubuntu;"
                          " background-color: rgb(224, 27, 36);"
                          " color: black;"
                          " border:5px solid black;"
                          " border-radius:30px;font-weight: bold;"
                          "}";

    us2poweron = !us2poweron;

    if (us2poweron) {
        ui->us2onoff->setText("ON");  // Turn ON
         ui->us2onoff->setStyleSheet(styleSheet1);
        ui->us1onoff->setText("ON");  // Turn ON
        ui->us1onoff->setStyleSheet(styleSheet1);
        us1poweron=true;
        ui->us3onoff->setText("ON");  // Turn ON
        ui->us3onoff->setStyleSheet(styleSheet1);
        us3poweron=true;
        ui->us4onoff->setText("ON");  // Turn ON
        ui->us4onoff->setStyleSheet(styleSheet1);
        us4poweron=true;
        ui->us1powup_but->setEnabled(true);
        ui->us1powdown_but->setEnabled(true);
        ui->us2powup_but->setEnabled(true);
        ui->us2powdown_but->setEnabled(true);
        ui->us3powup_but->setEnabled(true);
        ui->us3powdown_but->setEnabled(true);
        ui->us4powup_but->setEnabled(true);
        ui->us4powdown_but->setEnabled(true);
        //qDebug()<<"on";
    } else {

        ui->us2onoff->setText("OFF");  // Turn OFF
        ui->us2onoff->setStyleSheet(styleSheet2);
        ui->us1onoff->setText("OFF");  // Turn OFF
        ui->us1onoff->setStyleSheet(styleSheet2);
        us1poweron=false;
        ui->us3onoff->setText("OFF");  // Turn OFF
        ui->us3onoff->setStyleSheet(styleSheet2);
        us3poweron=false;
        ui->us4onoff->setText("OFF");  // Turn OFF
        ui->us4onoff->setStyleSheet(styleSheet2);
        us4poweron=false;
        ui->us1powup_but->setEnabled(false);
        ui->us1powdown_but->setEnabled(false);
        ui->us2powup_but->setEnabled(false);
        ui->us2powdown_but->setEnabled(false);
        ui->us3powup_but->setEnabled(false);
        ui->us3powdown_but->setEnabled(false);
        ui->us4powup_but->setEnabled(false);
        ui->us4powdown_but->setEnabled(false);
        ui->vitpowup_but->setEnabled(false);
        ui->vitpowdown_but->setEnabled(false);
         //qDebug()<<"off";
    }
}


void MainWindow::on_us3onoff_clicked()
{
    QString text=ui->us3onoff->text();

    QString styleSheet1 = "QPushButton {"
           " font:20pt Ubuntu;"
          " background-color: green;"
            "color: black;"

           " border:5px solid black;"
           " border-radius:30px;font-weight: bold;"
                                 "}";

    QString styleSheet2 = "QPushButton {"
           " font:20pt Ubuntu;"
          " background-color: rgb(224, 27, 36);"
            "color: black;"
            //"image: url(:/ci.png);"
           " border:5px solid black;"
           " border-radius:30px;font-weight: bold;"

           "}";
    us3poweron = !us3poweron;

    if (us3poweron) {
        ui->us1onoff->setText("ON");  // Turn ON
        ui->us1onoff->setStyleSheet(styleSheet1);
        ui->us2onoff->setText("ON");  // Turn ON
         ui->us2onoff->setStyleSheet(styleSheet1);
            us2poweron=true;
        ui->us3onoff->setText("ON");  // Turn ON
        ui->us3onoff->setStyleSheet(styleSheet1);
                    us3poweron=true;
        ui->us4onoff->setText("ON");  // Turn ON
        ui->us4onoff->setStyleSheet(styleSheet1);
              us4poweron=true;
              ui->us1powup_but->setEnabled(true);
              ui->us1powdown_but->setEnabled(true);
              ui->us2powup_but->setEnabled(true);
              ui->us2powdown_but->setEnabled(true);
              ui->us3powup_but->setEnabled(true);
              ui->us3powdown_but->setEnabled(true);
              ui->us4powup_but->setEnabled(true);
              ui->us4powdown_but->setEnabled(true);
        //qDebug()<<"on";
    } else {
        ui->us1onoff->setText("OFF");  // Turn OFF
        ui->us1onoff->setStyleSheet(styleSheet2);
        ui->us2onoff->setText("OFF");  // Turn OFF
        ui->us2onoff->setStyleSheet(styleSheet2);
        us2poweron=false;
        ui->us3onoff->setText("OFF");  // Turn OFF
        ui->us3onoff->setStyleSheet(styleSheet2);
        us3poweron=false;
        ui->us4onoff->setText("OFF");  // Turn OFF
        ui->us4onoff->setStyleSheet(styleSheet2);
        us4poweron=false;
        ui->us1powup_but->setEnabled(false);
        ui->us1powdown_but->setEnabled(false);
        ui->us2powup_but->setEnabled(false);
        ui->us2powdown_but->setEnabled(false);
        ui->us3powup_but->setEnabled(false);
        ui->us3powdown_but->setEnabled(false);
        ui->us4powup_but->setEnabled(false);
        ui->us4powdown_but->setEnabled(false);
        ui->vitpowup_but->setEnabled(false);
        ui->vitpowdown_but->setEnabled(false);
         //qDebug()<<"off";
    }
}

void MainWindow::on_us4onoff_clicked()
{
    QString text=ui->us4onoff->text();

    QString styleSheet1 = "QPushButton {"
           " font:20pt Ubuntu;"
          " background-color: green;"
            "color: black;"

           " border:5px solid black;"
           " border-radius:30px;font-weight: bold;"
                                 "}";

    QString styleSheet2 = "QPushButton {"
           " font:20pt Ubuntu;"
          " background-color: rgb(224, 27, 36);"
            "color: black;"
            //"image: url(:/ci.png);"
           " border:5px solid black;"
           " border-radius:30px;font-weight: bold;"

           "}";
    us4poweron = !us4poweron;

    if (us4poweron) {
        ui->us1onoff->setText("ON");  // Turn ON
        ui->us1onoff->setStyleSheet(styleSheet1);
        ui->us2onoff->setText("ON");  // Turn ON
         ui->us2onoff->setStyleSheet(styleSheet1);
            us2poweron=true;
        ui->us3onoff->setText("ON");  // Turn ON
        ui->us3onoff->setStyleSheet(styleSheet1);
                    us3poweron=true;
        ui->us4onoff->setText("ON");  // Turn ON
        ui->us4onoff->setStyleSheet(styleSheet1);
              us4poweron=true;
              ui->us1powup_but->setEnabled(true);
              ui->us1powdown_but->setEnabled(true);
              ui->us2powup_but->setEnabled(true);
              ui->us2powdown_but->setEnabled(true);
              ui->us3powup_but->setEnabled(true);
              ui->us3powdown_but->setEnabled(true);
              ui->us4powup_but->setEnabled(true);
              ui->us4powdown_but->setEnabled(true);
        //qDebug()<<"on";
    } else {
        ui->us1onoff->setText("OFF");  // Turn OFF
        ui->us1onoff->setStyleSheet(styleSheet2);
        ui->us2onoff->setText("OFF");  // Turn OFF
        ui->us2onoff->setStyleSheet(styleSheet2);
        us2poweron=false;
        ui->us3onoff->setText("OFF");  // Turn OFF
        ui->us3onoff->setStyleSheet(styleSheet2);
        us3poweron=false;
        ui->us4onoff->setText("OFF");  // Turn OFF
        ui->us4onoff->setStyleSheet(styleSheet2);
        us4poweron=false;
        ui->us1powup_but->setEnabled(false);
        ui->us1powdown_but->setEnabled(false);
        ui->us2powup_but->setEnabled(false);
        ui->us2powdown_but->setEnabled(false);
        ui->us3powup_but->setEnabled(false);
        ui->us3powdown_but->setEnabled(false);
        ui->us4powup_but->setEnabled(false);
        ui->us4powdown_but->setEnabled(false);
        ui->vitpowup_but->setEnabled(false);
        ui->vitpowdown_but->setEnabled(false);
         //qDebug()<<"off";
    }
}

void MainWindow::on_vitonoff_clicked()
{
    QString text = ui->vitonoff->text();
    // Define the style for "ON" state (green background)
    QString styleSheet1 = "QPushButton {"
                          " font:20pt Ubuntu;"
                          " background-color: green;"
                          " color: black;"
                          " border:5px solid black;"
                          " border-radius:30px;"
                          " font-weight: bold;"
                          "}";

    // Define the style for "OFF" state (red background)
    QString styleSheet2 = "QPushButton {"
                          " font:20pt Ubuntu;"
                          " background-color: rgb(224, 27, 36);"
                          " color: black;"
                          " border:5px solid black;"
                          " border-radius:30px;"
                          " font-weight: bold;"
                          "}";

    // Toggle the vitonoff state
    vitonoff = !vitonoff;

    // Check the current state and update the button text and styles accordingly
    if (vitonoff) {
        ui->vitonoff->setText("ON");
        ui->vitonoff->setStyleSheet(styleSheet1);
        ui->vitpowup_but->setEnabled(true);
        ui->vitpowdown_but->setEnabled(true);

    } else {
        ui->vitonoff->setText("OFF");
        ui->vitonoff->setStyleSheet(styleSheet2);
        handler->vit_off();
        ui->vitpowup_but->setEnabled(false);
        ui->vitpowdown_but->setEnabled(false);
    }
}

//connect doctor window to mainwindow
void MainWindow::doctorwindow_show(QString Surgeonname)
{
    Surgeonname=ui->comboBox_4->currentText();

}
void MainWindow::footpedalwindow_show()
{
    //foot->show();
}

void MainWindow::onCutMode_vitComChanged(int index) {
    QString modeText = ui->CutMode_vitCom->currentText();
    updateTabsBasedOnComboBox(modeText);


}

void MainWindow::onCutMode_vitComChanged1(int index) {
    QString currentText = ui->CutMode_vitCom_2->currentText();
    updateTabsBasedOnComboBox(currentText);
}

void MainWindow::onCutMode_vitComChanged2(int index) {
    QString currentText = ui->CutMode_vitCom_3->currentText();
    updateTabsBasedOnComboBox(currentText);

}

void MainWindow::onCutMode_vitComChanged3(int index) {
    QString currentText = ui->CutMode_vitCom_4->currentText();
    updateTabsBasedOnComboBox(currentText);

}

void MainWindow::updateTabsBasedOnComboBox(const QString &selected) {
    ////qDebug() << "Selected Mode as Text:" << selected;

    // Default action: Log unknown mode
    bool modeFound = false;

    // Call the appropriate function based on the selected mode
    if (selected == "Continuous") {
        handler->pdm_mode(CONTINOUS);
        ui->tabWidget_2->setCurrentIndex(0);
      //  ui->comboBox->setCurrentText("Continuous");  // Update combo box text
        modeFound = true;
        ui->label_32->hide();

    } else if (selected == "Pulse") {
        handler->pdm_mode(PULSE_MODE);
        pulseup_mode();
        pulsedown_mode();
        ui->tabWidget_2->setCurrentIndex(1);
     //   ui->comboBox->setCurrentText("Pulse");  // Update combo box text
        modeFound = true;
        ui->label_32->show();

    } else if (selected == "Ocupulse") {
        handler->pdm_mode(CONTINOUS);
        on_ocupulseup_but_clicked();
      on_ocupulsedown_but_clicked();
        ui->tabWidget_2->setCurrentIndex(2);
       // ui->comboBox->setCurrentText("Ocupulse");  // Update combo box text
        modeFound = true;
        ui->label_32->show();


    } else if (selected == "Ocuburst") {
        handler->pdm_mode(CONTINOUS);
on_ocuburstup_but_clicked();
on_ocuburstdown_but_clicked();
        ui->tabWidget_2->setCurrentIndex(3);
    //    ui->comboBox->setCurrentText("Ocuburst");  // Update combo box text
        modeFound = true;
        ui->label_32->show();


    } else if (selected == "Single burst") {
        handler->pdm_mode(SINGLE_BURST);
        singleburstup_mode();
        singleburstdown_mode();
        ui->tabWidget_2->setCurrentIndex(4);
        //ui->comboBox->setCurrentText("Single burst");  // Update combo box text
        modeFound = true;
        ui->label_32->show();


    } else if (selected == "Multi burst") {
        handler->pdm_mode(CONTINOUS);
        multiburstup_mode();
        multiburstdown_mode();
        ui->tabWidget_2->setCurrentIndex(5);
       // ui->comboBox->setCurrentText("Multi burst");  // Update combo box text
        modeFound = true;
        ui->label_32->show();


    } else if (selected == "Cold phaco") {
        handler->pdm_mode(COLD_PHACO);
        coldphacoup_mode();
        coldphacodown_mode();
        coldphaco1up_mode();
        coldphaco1down_mode();
        ui->tabWidget_2->setCurrentIndex(6);
    //    ui->comboBox->setCurrentText("Cold phaco");  // Update combo box text
        modeFound = true;
        ui->label_32->show();

    }

    if (!modeFound) {
        ////qDebug() << "Unknown mode selected!";
    }

    // Print to the debug output
    ////qDebug() << "Mode is working: " << selected;

    // Update multiple line edits with the selected value

}


int MainWindow::getvalue(int input)
{
    if(input<=60){
        return 60;
    }else{
        return ((input-1)/60+1)*60;
    }
}

void MainWindow::on_CI4_2_clicked()
{
    QString styleSheet3 = "QPushButton {"
           " font:20pt Ubuntu;"
          " background-color: green;"
            "color: black;"
      "image: url(:/images/ci.png);"
           " border:5px solid black;"
           " border-radius:30px;"
            "font-weight: bold;"
                                 "}";
    QString styleSheet4 = "QPushButton {"
           " font:20pt Ubuntu;"
          " background-color: red;"

            "image: url(:/images/ci.png);"
           " border:5px solid black;"
           " border-radius:30px;"
            "font-weight: bold;"
                                 "}";
    if(!overallci){
        ui->CI5_5->setStyleSheet(styleSheet3);

            handler->pinchvalve_on();
        handler->safetyvent_on();
        handler->speaker_on(0,0,1,0);
    }
    else{
    ui->CI5_5->setStyleSheet(styleSheet4);

        handler->safetyvent_off();
        handler->pinchvalve_off();
        handler->speaker_off();
    }
    overallci=!overallci;
}

void MainWindow::receivecombo(const QString &text)
{
ui->comboBox->setCurrentText(text);
}

void MainWindow::performpump(const QString &text)
{
ui->comboBox->setCurrentText(text);
////qDebug()<<"the combo box current text is"<<text;
  QString value=ui->comboBox->currentText();
    if(value == "Peristaltic"){
// ui->pushButton->setStyleSheet("background-color:green;");
        //us1
        ui->label_87->show();
        ui->lineEdit_56->show();
        ui->us1flowup_but->show();
        ui->us1flowdown_but->show();

        ui->textEdit_41->show();

        //us2
        ui->label_91->show();
        ui->lineEdit_59->show();
        ui->us2flowup_but->show();
        ui->us2flowdown_but->show();

        ui->textEdit_44->show();
        //us3
         ui->label_96->show();
        ui->lineEdit_62->show();
        ui->us3flowup_but->show();
        ui->us3flowdown_but->show();

        ui->textEdit_48->show();

        //us4
        ui->label_103->show();
        ui->lineEdit_65->show();
        ui->us4flowup_but->show();
        ui->us4flowdown_but->show();

        ui->textEdit_50->show();

        //ia 1
        ui->label_112->show();
        ui->lineEdit_69->show();
        ui->ia1flowup_but->show();
        ui->ia1flowdown_but->show();

        ui->textEdit_55->show();

        //ia2
        ui->label_108->show();
        ui->lineEdit_67->show();
        ui->ia2flowup_but->show();
        ui->ia2flowdown_but->show();

        ui->textEdit_53->show();

        //vit
        ui->label_122->show();
        ui->lineEdit_72->show();
        ui->vitflowup_but->show();
        ui->vitflowdown_but->show();

        ui->textEdit_58->show();
      

    }
    else if(value == "Venturi"){
       // ui->pushButton_2->setStyleSheet("background-color:green;");
        //us1
        ui->label_87->hide();
        ui->lineEdit_56->hide();
        ui->us1flowup_but->hide();
        ui->us1flowdown_but->hide();

        ui->textEdit_41->hide();
        //us2
        ui->label_91->hide();
        ui->lineEdit_59->hide();
        ui->us2flowup_but->hide();
        ui->us2flowdown_but->hide();

        ui->textEdit_44->hide();
        //us3
         ui->label_96->hide();
        ui->lineEdit_62->hide();
        ui->us3flowup_but->hide();
        ui->us3flowdown_but->hide();

        ui->textEdit_48->hide();
        //us4
        ui->label_103->hide();
        ui->lineEdit_65->hide();
        ui->us4flowup_but->hide();
        ui->us4flowdown_but->hide();

        ui->textEdit_50->hide();
        //ia 1
        ui->label_112->hide();
        ui->lineEdit_69->hide();
        ui->ia1flowup_but->hide();
        ui->ia1flowdown_but->hide();

        ui->textEdit_55->hide();
        //ia2
        ui->label_108->hide();
        ui->lineEdit_67->hide();
        ui->ia2flowup_but->hide();
        ui->ia2flowdown_but->hide();

        ui->textEdit_53->hide();
        //vit
        ui->label_122->hide();
        ui->lineEdit_72->hide();
        ui->vitflowup_but->hide();
        ui->vitflowdown_but->hide();

        ui->textEdit_58->hide();
    }

}

void MainWindow::receiveValues(const QString &comboBoxValue,const QString &combo, int dia, int us1pow, int us1vac, int us1asp, int us2pow, int us2vac, int us2asp, int us3pow, int us3vac, int us3asp, int us4pow, int us4vac, int us4asp, int ia1vac, int ia1asp, int ia2vac, int ia2asp, int vitcut, int vitvac, int vitasp, const QString &powmode, const QString &vacmode, const QString &powmethod, const QString &us2powmode, const QString &us2vacmode, const QString &us2powermethod, const QString &us3powmode, const QString &us3vacmode, const QString &us3powermethod, const QString &us4powmode, const QString &us4vacmode, const QString &us4powermethod, const QString &ia1mode, const QString &ia2mode, const QString &vitmode, const QString &vitvacmode)
{
    surgicalData.comboBoxValue = comboBoxValue;
    surgicalData.combo =combo;
        surgicalData.dia = dia;
        surgicalData.us1pow = us1pow;
        surgicalData.us1vac = us1vac;
        surgicalData.us1asp = us1asp;
        surgicalData.us2pow = us2pow;
        surgicalData.us2vac = us2vac;
        surgicalData.us2asp = us2asp;
        surgicalData.us3pow = us3pow;
        surgicalData.us3vac = us3vac;
        surgicalData.us3asp = us3asp;
        surgicalData.us4pow = us4pow;
        surgicalData.us4vac = us4vac;
        surgicalData.us4asp = us4asp;
        surgicalData.ia1vac = ia1vac;
        surgicalData.ia1asp = ia1asp;
        surgicalData.ia2vac = ia2vac;
        surgicalData.ia2asp = ia2asp;
        surgicalData.vitcut = vitcut;
        surgicalData.vitvac = vitvac;
        surgicalData.vitasp = vitasp;
        surgicalData.powmode = powmode;
        surgicalData.vacmode = vacmode;
        surgicalData.powmethod = powmethod;
        surgicalData.us2powmode = us2powmode;
        surgicalData.us2vacmode = us2vacmode;
        surgicalData.us2powermethod = us2powermethod;
        surgicalData.us3powmode = us3powmode;
        surgicalData.us3vacmode = us3vacmode;
        surgicalData.us3powermethod = us3powermethod;
        surgicalData.us4powmode = us4powmode;
        surgicalData.us4vacmode = us4vacmode;
        surgicalData.us4powermethod = us4powermethod;
        surgicalData.ia1mode = ia1mode;
        surgicalData.ia2mode = ia2mode;
        surgicalData.vitmode = vitmode;
        surgicalData.vitvacmode = vitvacmode;
        ui->comboBox_4->setCurrentText(surgicalData.combo);
        ui->comboBox->setCurrentText(surgicalData.comboBoxValue);
      ui->lineEdit_74->setText(QString::number(surgicalData.dia));
      //us1
      ui->lineEdit_57->setText(QString::number(surgicalData.us1pow));
      ui->lineEdit_55->setText(QString::number(surgicalData.us1vac));
      ui->lineEdit_56->setText(QString::number(surgicalData.us1asp));
      ui->us1mode->setText(surgicalData.powmode);
      ui->us1vacmode->setText(surgicalData.vacmode);
      ui->CutMode_vitCom->setCurrentText(surgicalData.powmethod);
      //us2
      ui->lineEdit_58->setText(QString::number(surgicalData.us2pow));
      ui->lineEdit_60->setText(QString::number(surgicalData.us2vac));
      ui->lineEdit_59->setText(QString::number(surgicalData.us2asp));
      ui->us2mode->setText(surgicalData.us2powmode);
      ui->us2vacmode->setText(surgicalData.us2vacmode);
      ui->CutMode_vitCom_2->setCurrentText(surgicalData.us2powermethod);
      //us3
      ui->lineEdit_61->setText(QString::number(surgicalData.us3pow));
      ui->lineEdit_63->setText(QString::number(surgicalData.us3vac));
      ui->lineEdit_62->setText(QString::number(surgicalData.us3asp));
      ui->us3mode->setText(surgicalData.us3powmode);
      ui->us3vacmode->setText(surgicalData.us3vacmode);
      ui->CutMode_vitCom_3->setCurrentText(surgicalData.us3powermethod);
      //us4
      ui->lineEdit_64->setText(QString::number(surgicalData.us4pow));
      ui->lineEdit_66->setText(QString::number(surgicalData.us4vac));
      ui->lineEdit_65->setText(QString::number(surgicalData.us4asp));
      ui->us4mode->setText(surgicalData.us4powmode);
      ui->us4vacmode->setText(surgicalData.us4vacmode);
      ui->CutMode_vitCom_4->setCurrentText(surgicalData.us4powermethod);
      //ia1
      ui->lineEdit_70->setText(QString::number(surgicalData.ia1vac));
      ui->lineEdit_69->setText(QString::number(surgicalData.ia1asp));
      ui->lineEdit_68->setText(QString::number(surgicalData.ia2vac));
      ui->lineEdit_67->setText(QString::number(surgicalData.ia2asp));
      ui->lineEdit_71->setText(QString::number(surgicalData.vitcut));
      ui->lineEdit_73->setText(QString::number(surgicalData.vitvac));
      ui->lineEdit_72->setText(QString::number(surgicalData.vitasp));
      //qDebug()<<surgicalData.ia1asp<<surgicalData.ia2asp<<surgicalData.ia1vac<<surgicalData.ia2vac<<"these are sended from the doctor windiow";


}

void MainWindow::on_SETTINGS_BUT_2_clicked()
{
    qDebug() << "Disabling functions.";
    handler->buzz();
    motoroff();
    handler->phaco_off();
    handler->fs_count(0);
    handler->freq_count(0);
    handler->phaco_power(0);
    handler->pinchvalve_off();
    handler->safetyvent_off();
    handler->vit_off();
    protimer->stop();
    Tacutalsensor->stop();
    in->show();
    in->exec();


}

void MainWindow::onComboBoxIndexChanged(int index)
{


    if (!db.isValid()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(PATH);

        if (!db.open()) {
            //qDebug() << "Error: connection with database failed:";
            return;
        }
    }

    QSqlQuery query(db);

    // Step 1: Update the last selected index
    query.prepare("UPDATE phacohigh SET lastupdate = :index");
    query.bindValue(":index", index);

    if (!query.exec()) {
//        //qDebug() << "Error updating last selected index:";
        return;
    } else {
//        qDebug() << "Last selected index updated to" << index;
    }


    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}
void MainWindow::setLastSelectedValue()
{

    if (!db.isValid()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(PATH);

        if (!db.open()) {
//            qDebug() << "Error: connection with database failed:";
            return;
        }
    }

    // Prepare and execute the query
    QSqlQuery query(db);
    query.prepare("SELECT lastupdate FROM phacohigh LIMIT 1");
    if (!query.exec()) {
//        qDebug() << "Error retrieving last index:";
        db.close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
        return;
    }

    if (query.next()) {
           int lastIndex = query.value(0).toInt();

           if (lastIndex >= 0 && lastIndex <= 4) {  // Ensure index is within valid range
               ui->comboBox_4->setCurrentIndex(lastIndex);
               QString surgeonName = ui->comboBox_4->currentText();
   foot->updateFootpedalComboBoxes(surgeonName);
//               qDebug() << "Last selected surgeon index set to" << lastIndex << "corresponding to surgeon:" << surgeonName;

                // push(ui->comboBox_4->currentText());
           }else {
//        qDebug() << "No index found in the database. Verify the data in the table.";
    }

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }
}

void MainWindow::rx_viberationvalue(const QString &text)
{
    vibon=text;
    if(vibon == "Vibration ON"){
        ui->pushButton_2->setStyleSheet("image: url(:/images/vibrationon.png);border-radius:20px;border:none;");
        }else{
            ui->pushButton_2->setStyleSheet("image: url(:/images/vibrationoff.png);border-radius:20px;border:none;");
        }
    }


int MainWindow::readsensorvalue()
{
    int sensorvalue=vac->convert(0XD7);
    int actualsensor=sensorvalue*0.17;
    return actualsensor;

}

void MainWindow::push(const QString &surgeonName) {

   //qDebug() << "Attempting to fetch data for surgeon:" << surgeonName;
emit sendsurgeon(surgeonName);
    // Check and display available drivers
//    qDebug() << "Available SQL drivers:" << QSqlDatabase::drivers();

    if (!db.isValid()) {
        db = QSqlDatabase::addDatabase("QSQLITE","PATH");
        db.setDatabaseName(PATH); // Ensure PATH is correctly set
    }

    if (!db.isOpen()) {
        if (!db.open()) {
//            //qDebug() << "Failed to open database. Error:" << db.lastError().text();
            return;
        } else {
//            qDebug() << "Database connection opened successfully.";
        }
    } else {
//        qDebug() << "Database is already open.";
    }

    // Prepare a single query to fetch all required data
    QSqlQuery query(db);
    query.prepare(
          "SELECT diapowmax, pump, viberationoff, Epinaspmax, Epinvacmax, Epinpowmax, "
          "quadpowmax, quadvacmax, quadaspmax, Quadvacmode, Quadpowermethod, Quadpowmode, "
          "caspmax, cvacmax, cpowmax, Chopvacmode, Choppowermethod, Choppowmode, "
          "saspmax, svacmax, spowmax, Sculptvacmode, Sculptpowermethod, Sculptpowmode, "
          "cortexaspmode, cortexvacmode, polishaspmode, polishvacmode, "
          "ia1aspmax, ia1vacmax, ia2aspmax, ia2vacmax, "
          "vitcutmax, vitvacmax, vitaspmax, vitcutmode, vitvacmode, "
          "Epinpowermethod, Epinpowmode, Epinvacmode, "
          "footleft, footright, footbottomleft, footbottomright, "
          "fzero, fone, ftwo, fthree "
          "FROM phacohigh "
          "WHERE surgeon = :surgeon"
      );
    query.bindValue(":surgeon", surgeonName);

    // Execute the query
    if (!query.exec()) {
//        qDebug() << "Failed to execute query. Error:" << query.lastError().text();
        return;
    }

    // Process the results
    if (query.next()) {



        // Retrieve the values from the query result
        int phacoPowerMax = query.value("diapowmax").toInt();
        QString pump = query.value("pump").toString();
        QString vibeonoff = query.value("viberationoff").toString();
        // US1 (Epinucleus) parameters
        int us1power = query.value("Epinpowmax").toInt();
        int us1vacmax = query.value("Epinvacmax").toInt();
        int us1flowmax = query.value("Epinaspmax").toInt();
        QString us1mode = query.value("Epinpowmode").toString();
        QString us1vacmode = query.value("Epinvacmode").toString();
        QString us1powermethod = query.value("Epinpowermethod").toString();

        // US2 (Quadrant) parameters
        int us2power = query.value("quadpowmax").toInt();
        int us2vacmax = query.value("quadvacmax").toInt();
        int us2aspmax = query.value("quadaspmax").toInt();
        QString us2mode = query.value("Quadpowmode").toString();
        QString us2vacmode = query.value("Quadvacmode").toString();
        QString us2powermethod = query.value("Quadpowermethod").toString();

        // US3 (Chop) parameters
        int us3power = query.value("cpowmax").toInt();
        int us3vacmax = query.value("cvacmax").toInt();
        int us3aspmax = query.value("caspmax").toInt();
        QString us3mode = query.value("Choppowmode").toString();
        QString us3vacmode = query.value("Chopvacmode").toString();
        QString us3powermethod = query.value("Choppowermethod").toString();

        // US4 (Sculpt) parameters
        int us4power = query.value("spowmax").toInt();
        int us4vacmax = query.value("svacmax").toInt();
        int us4aspmax = query.value("saspmax").toInt();
        QString us4mode = query.value("Sculptpowmode").toString();
        QString us4vacmode = query.value("Sculptvacmode").toString();
        QString us4powermethod = query.value("Sculptpowermethod").toString();

        // IA (Irrigation/Aspiration) parameters
        int ia1vacmax = query.value("ia1vacmax").toInt();
        int ia1aspmax = query.value("ia1aspmax").toInt();
        int ia2vacmax = query.value("ia2vacmax").toInt();
        int ia2aspmax = query.value("ia2aspmax").toInt();
        QString ia1mode = query.value("cortexvacmode").toString();
        QString ia2mode = query.value("polishvacmode").toString();

 // qDebug()<<ia1mode<<ia2mode;
        // Vitrectomy parameters
        int vitcutmax = query.value("vitcutmax").toInt();
        int vitvacmax = query.value("vitvacmax").toInt();
        int vitaspmax = query.value("vitaspmax").toInt();
        QString vitcutmode = query.value("vitcutmode").toString();
        QString vitvacmode = query.value("vitvacmode").toString();
        QString footleft = query.value("footleft").toString();
        QString footright = query.value("footright").toString();
        QString footbleft = query.value("footbottomleft").toString();
        QString footbright = query.value("footbottomright").toString();

        // Update UI components with the retrieved values
        ui->lineEdit_74->setText(QString::number(phacoPowerMax));
       ui->comboBox->setCurrentText(pump);
        // Update US1 UI components
        ui->lineEdit_57->setText(QString::number(us1power));  // Power
        ui->lineEdit_55->setText(QString::number(us1vacmax)); // Vacuum
        ui->lineEdit_56->setText(QString::number(us1flowmax)); // Flow
        ui->us1mode->setText(us1mode);
        ui->us1vacmode->setText(us1vacmode);
        ui->CutMode_vitCom->setCurrentText(us1powermethod);
        // Update US2 UI components
        ui->lineEdit_58->setText(QString::number(us2power)); // Power
        ui->lineEdit_60->setText(QString::number(us2vacmax)); // Vacuum
        ui->lineEdit_59->setText(QString::number(us2aspmax)); // Aspiration
        ui->us2mode->setText(us2mode);
        ui->us2vacmode->setText(us2vacmode);
        ui->CutMode_vitCom_2->setCurrentText(us2powermethod);
        // Update US3 UI components
        ui->lineEdit_61->setText(QString::number(us3power)); // Power
        ui->lineEdit_63->setText(QString::number(us3vacmax)); // Vacuum
        ui->lineEdit_62->setText(QString::number(us3aspmax)); // Aspiration
        ui->us3mode->setText(us3mode);
        ui->us3vacmode->setText(us3vacmode);
        ui->CutMode_vitCom_3->setCurrentText(us3powermethod);

        // Update US4 UI components
        ui->lineEdit_64->setText(QString::number(us4power)); // Power
        ui->lineEdit_66->setText(QString::number(us4vacmax)); // Vacuum
        ui->lineEdit_65->setText(QString::number(us4aspmax)); // Aspiration
        ui->us4mode->setText(us4mode);
        ui->us4vacmode->setText(us4vacmode);
        ui->CutMode_vitCom_4->setCurrentText(us4powermethod);
        // Update IA UI components
        ui->lineEdit_67->setText(QString::number(ia2aspmax)); // IA1 Vacuum
        ui->lineEdit_69->setText(QString::number(ia1aspmax)); // IA1 Aspiration
        ui->lineEdit_70->setText(QString::number(ia1vacmax)); // IA2 Vacuum
        ui->lineEdit_68->setText(QString::number(ia2vacmax)); // IA2 Aspiration
        ui->ia1mode->setText(ia1mode);
        ui->ia2mode->setText(ia2mode);
        // Update Vitrectomy UI components
        ui->lineEdit_71->setText(QString::number(vitcutmax)); // Vitrectomy Cut Max
        ui->lineEdit_73->setText(QString::number(vitvacmax)); // Vitrectomy Vacuum Max
        ui->lineEdit_72->setText(QString::number(vitaspmax)); // Vitrectomy Aspiration Max
        ui->vitmode->setText(vitcutmode);
        ui->vitvacmode->setText(vitvacmode);
        ui->us1mode->update();
        ui->us1vacmode->update();
        ui->us2mode->update();
        ui->us2vacmode->update();
        ui->us3mode->update();
        ui->us3vacmode->update();
        ui->us4mode->update();
        ui->us4vacmode->update();
        ui->vitmode->update();
        ui->vitvacmode->update();
        int nfpo = query.value("fzero").toInt();
        int nfp1 = query.value("fone").toInt();
        int nfp2 = query.value("ftwo").toInt();
        int nfp3 = query.value("fthree").toInt();
       // qDebug()<<nfpo<<nfp1<<nfp2<<nfp3<<"that is the sql value before calibration";
        nfpzero = static_cast<int>((nfpo / 100.0) * 4090);
        nfpone = static_cast<int>((nfp1 / 100.0) * 4090);
        nfptwo = static_cast<int>((nfp2 / 100.0) * 4090);
        nfpthree = static_cast<int>((nfp3 / 100.0) * 4090);
        vibon=vibeonoff;

        //qDebug()<<nfpzero<<nfpone<<nfptwo<<nfpthree<<"that is sql value";
          emit left_foot(footleft);
        emit right_foot(footright);
        emit bottom_left(footbleft);
        emit bottom_right(footbright);
        //qDebug()<<"pump is reterived from the sql"<<pump;
        performpump(pump);
        query.clear();
                //qDebug()<<ia1aspmax<<ia1vacmax<<ia2aspmax<<ia2vacmax<<"these are retereived from the sql";
    } else {
        //qDebug() << "No data found for surgeon:" << surgeonName;
    }
    db.close();
    QSqlDatabase::removeDatabase("PATH");
}

void MainWindow::on_pushButton_42_clicked()
{
   // qDebug() << "Disabling functions.";
    motoroff();
    handler->phaco_off();
    handler->fs_count(0);
    handler->freq_count(0);
    handler->phaco_power(0);
    handler->pinchvalve_off();
    handler->safetyvent_off();
    handler->vit_off();
    protimer->stop();
    Tacutalsensor->stop();
    foot->show();

}

void MainWindow::on_pushButton_clicked()
{
   // qDebug() << "Disabling functions.";
    motoroff();
    handler->phaco_off();
    handler->fs_count(0);
    handler->freq_count(0);
    handler->phaco_power(0);
    handler->pinchvalve_off();
    handler->safetyvent_off();
    handler->vit_off();
    protimer->stop();
    Tacutalsensor->stop();
    this->close();


}


void MainWindow::on_ocuburstdown_but_clicked()
{
    ocuburst = ui->lineEdit_77->text().toInt();
    ocuburst -= 1;
    if (ocuburst < 1) {
        ocuburst = 1;
    }
    ui->lineEdit_77->setText(QString::number(ocuburst));
    handler->freq_count(2500);
    handler->fs_count(nfpzero+nfpone+nfptwo);
    handler->burst_length(ocuburst);
    handler->burst_off_length(ocuburst);
     handler->pdm_mode(CONTINOUS);
    // qDebug()<<ocuburst;
}

void MainWindow::on_ocuburstup_but_clicked()
{
    ocuburst = ui->lineEdit_77->text().toInt();
    ocuburst += 1;
    if (ocuburst > 15) {
        ocuburst = 15;
    }
    ui->lineEdit_77->setText(QString::number(ocuburst));
    handler->freq_count(2500);
    handler->fs_count(nfpzero+nfpone+nfptwo);
    handler->burst_length(ocuburst);
    handler->burst_off_length(ocuburst);
    handler->pdm_mode(CONTINOUS);
   // qDebug()<<ocuburst;
}

void MainWindow::on_ocupulsedown_but_clicked()
{
    ocupulse = ui->lineEdit_76->text().toInt();
    ocupulse -= 5;
    if (ocupulse < 5) {
        ocupulse = 5;
    }
    ui->lineEdit_76->setText(QString::number(ocupulse));
    handler->freq_count(2500);
    handler->fs_count(nfpzero+nfpone+nfptwo);
    handler->pdm_mode(CONTINOUS);
    handler->pulse_count(ocupulse);
}

void MainWindow::on_ocupulseup_but_clicked()
{
    ocupulse = ui->lineEdit_76->text().toInt();
    ocupulse += 5;
    if (ocupulse > 170) {
        ocupulse = 170;
    }
    ui->lineEdit_76->setText(QString::number(ocupulse));

    handler->freq_count(2500);
    handler->fs_count(nfpzero+nfpone+nfptwo);
    handler->pdm_mode(CONTINOUS);
    handler->pulse_count(ocupulse);

}
