#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"footpedal.h"
#include "tuning.h"
#include <QTimer>
#include<cmath>
#include<QStyleFactory>
#include<QSqlError>
#include<QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , butname(-1)
    ,isTuneEnabled(false)
    ,powerOn1(false)
    ,overallci(false)
    ,us1poweron(false)
    ,us2poweron(false)
    ,us3poweron(false)
    ,us4poweron(false)
    ,vitonoff(false)
    , pinchvalve2Flag(false)  // Initialize to false
        , m_phacoFiring(false)
        , m_eptTotalMs(0)
        , m_usdTotal(0.0)
        , m_currentPowerPct(0.0f)


{
    ui->setupUi(this);//class initialisation
    vac=new Vaccum;
    handler=new hwhandler;
    foot=new footpedal;
    in=new doctor;
    //s=new settings;
    key=new keypad;
    footsensor=new QTimer;
    elapsedTimer=new QElapsedTimer;

    handler->phaco_off();
    handler->vibrator_off();
    handler->phaco_power(0);
    handler->convert_dac(0x30, 0);
    handler->fs_count_limit(nfpzero+nfpone+nfptwo);

    handler->freq_count(0);
    handler->speaker_off();
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
    updateTimer->start(500); // Update every 100 milliseconds

//footpedalcheck
    protimer=new QTimer;
    connect(protimer,&QTimer::timeout,this,&MainWindow::footpedalcheck);
    protimer->start(1);

    //range

//update sensor for linear
       sensortimer = new QTimer(this);
      // connect(sensortimer, SIGNAL(timeout()), this, SLOT(sensor2()));
       sensortimer->start(100); // Update labels every 1000 milliseconds (1 second)
 Tacutalsensor=new QTimer;
 connect(Tacutalsensor,&QTimer::timeout,this,&MainWindow::readsensorvalue);
 Tacutalsensor->start(1);
//update handpiece status when handpiece is connected or not
   statusUpdateTimer = new QTimer(this);
   connect(statusUpdateTimer, &QTimer::timeout, this, &MainWindow::updatehandpieceStatus);
   statusUpdateTimer->start(1000); // Update every second

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
      // connect(this,&MainWindow::sendfootvalues,foot,&footpedal::receivedval);
      connect(foot,&footpedal::activatemain,this,&MainWindow::disablefunction);
       connect(in,&doctor::tx_speakeronoff,this,&MainWindow::rx_speakeronoff);
  connect(in,&doctor::togglbutton,this,&MainWindow::mainus1button);
       connect(in,&doctor::activatemainwindow,this,&MainWindow::disablefunction);

       QString styleSheet5 = "QLabel {"
                  "image: url(:/images/not_Tuned.png);"
                   "background-color:transparent;"
                                        "}";
       connect(foot,&footpedal::sendleftfootdoc,in,&doctor::receivetopleft);
       connect(foot,&footpedal::sendrightfootdoc,in,&doctor::receivetopright);
       connect(foot,&footpedal::sendbottomleftddoc,in,&doctor::receivebottomleft);
       connect(foot,&footpedal::sendbottomrightdoc,in,&doctor::receivebottomright);
//update footpedal value from doctor window to mainwindow and mainwindow to footpedal screen
       connect(in,&doctor::sendleftfootvalues,foot,&footpedal::combobox1);
       connect(in,&doctor::sendrightfootvalues,foot,&footpedal::combobox2);
       connect(in,&doctor::sendbleftfootvalues,foot,&footpedal::combobox3);
       connect(in,&doctor::sendbrightfootvalues,foot,&footpedal::combobox4);
       connect(in,&doctor::tx_swaponoff,this,&MainWindow::rx_swaponoff);
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
    ui->vitpowup_but->setEnabled(true);
    ui->vitpowdown_but->setEnabled(true);

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
   ui->lineEdit_71->setText("2520");
   ui->lineEdit_73->setText("500");
   ui->lineEdit_72->setText("40");
   //dia
  ui->lineEdit_74->setText("100");

  // keypad connetion
    connect(key,&keypad::textsignal,this,&MainWindow::on_clicked);
    connect(key,&keypad::entersignal,this,&MainWindow::on_clickedenter);
    connect(key, &keypad::clearsignal, this, &MainWindow::on_clickedclear);
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
     connect(ui->CutMode_vitCom, &QComboBox::currentTextChanged, this, &MainWindow::updateTabsBasedOnComboBox);
    connect(ui->CutMode_vitCom_2, &QComboBox::currentTextChanged, this, &MainWindow::updateTabsBasedOnComboBox);
     connect(ui->CutMode_vitCom_3, &QComboBox::currentTextChanged, this, &MainWindow::updateTabsBasedOnComboBox);
     connect(ui->CutMode_vitCom_4, &QComboBox::currentTextChanged, this, &MainWindow::updateTabsBasedOnComboBox);


   // connect(ui->CutMode_vitCom_2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onCutMode_vitComChanged1);


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
      //connect(ui->coldphacoup_but,&QPushButton::clicked,this,&MainWindow::coldphacoup_mode);
     // connect(ui->coldphacodown_but,&QPushButton::clicked,this,&MainWindow::coldphacodown_mode);
      //connect(ui->coldphaco1up_but,&QPushButton::clicked,this,&MainWindow::coldphaco1up_mode);
    //  connect(ui->coldphaco1down_but,&QPushButton::clicked,this,&MainWindow::coldphaco1down_mode);
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
     // changesvaluesql();


ui->ULTRASONICBUT1->setEnabled(false);
ui->ULTRASONICBUT2->setEnabled(false);
ui->ULTRASONICBUT3->setEnabled(false);
ui->ULTRASONICBUT4->setEnabled(false);
ui->label_16->setStyleSheet(styleSheet5);
ui->label_17->setStyleSheet(styleSheet5);
ui->label_18->setStyleSheet(styleSheet5);
ui->label_27->setStyleSheet(styleSheet5);
QDateTime date = QDateTime::currentDateTime();
QString formatteddate = date.toString("dd.MM.yyyy");
ui->lab_date->setText(formatteddate);

QTime time = QTime::currentTime();
QString formatTime = time.toString("hh:mm:ss");
ui->lab_time->setText(formatTime);
//qDebug()<<"the mainwidow is closed";
// Array of all buttons




}

//elapsed time
void MainWindow::updateElapsedTime()
{
    if (timer5.isValid()) {
        qint64 elapsed = timer5.elapsed();
        QString elapsedTimeStr = QString::number(elapsed) + " ms";
        //ui->elapsed_time->setText(elapsedTimeStr);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete footsensor;
    delete elapsedTimer;
    delete updateTimer;
    delete protimer;
    delete sensortimer;
    delete timerfoot;
    db.close();
}



void MainWindow::nReceiveFreq(int count)
{
     nFreqCount=count;
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
          ui->lineEdit_71->setText(QString::number(60));
          return;}
      setRange(ui->lineEdit_71, prevValue, value, 2520);

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
   QString text = ui->lineEdit_71->text().trimmed();
   if (!text.isEmpty()) {
       int input = text.toInt();
       int output = getvalue(input);  // Process the input to get the desired output
       ui->lineEdit_71->setText(QString::number(output));
       lastValidValues[ui->lineEdit_71] = output;  // Store the processed output as the last valid value
   } else {
       // Restore the last valid value if input is empty
       if (lastValidValues.contains(ui->lineEdit_71)) {
           ui->lineEdit_71->setText(QString::number(lastValidValues[ui->lineEdit_71]));
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
void MainWindow::saveCurrentTabValues()
{
    int currentIndex = ui->tabWidget->currentIndex(); // Detect current tab
    int pulse = ui->lineEdit_75->text().toInt();
    int ocupulse= ui->lineEdit_77->text().toInt();
    int ocuburst = ui->lineEdit_76->text().toInt();
    int singleburst = ui->lineEdit_78->text().toInt();
    int multiburst = ui->lineEdit_79->text().toInt();
    int coldphaco = ui->lineEdit_80->text().toInt();
    int coldphaco1 = ui->lineEdit_81->text().toInt();

    if (currentTab == 1) {
        us1pulseline = pulse;
        us1ocupulseline = ocupulse;
        us1ocuburstline = ocuburst;
        us1singleburstline = singleburst;
        us1multiburstline = multiburst;
        us1coldphacoline = coldphaco;
        us1coldphaco1line = coldphaco1;
    }
    else if (currentTab == 2) {
        us2pulseline = pulse;
        us2ocupulseline = ocupulse;
        us2ocuburstline = ocuburst;
        us2singleburstline = singleburst;
        us2multiburstline = multiburst;
        us2coldphacoline = coldphaco;
        us2coldphaco1line = coldphaco1;
    }
    else if (currentTab == 3) {
        us3pulseline = pulse;
        us3ocupulseline = ocupulse;
        us3ocuburstline = ocuburst;
        us3singleburstline = singleburst;
        us3multiburstline = multiburst;
        us3coldphacoline = coldphaco;
        us3coldphaco1line = coldphaco1;
    }
    else if (currentTab == 4) {
        us4pulseline = pulse;
        us4ocupulseline = ocupulse;
        us4ocuburstline = ocuburst;
        us4singleburstline = singleburst;
        us4multiburstline = multiburst;
        us4coldphacoline = coldphaco;
        us4coldphaco1line = coldphaco1;
    }
}

void MainWindow::ULTRASONICBUT1()
{
    saveCurrentTabValues();
    currentTab = 1;
    ui->tabWidget->setCurrentIndex(0);

    QString currentText = ui->CutMode_vitCom->currentText();
   updateTabsBasedOnComboBox(currentText);
    ui->CutMode_vit->show();
    ui->CutMode_vitCom->show();
    ui->tabWidget_2->show();
    ui->CI5_5->show();
    handler->phaco_off();
    handler->freq_count(0);
ui->label_32->show();
    butname=1;
    handler->buzz();
     us1PdmMode = true;
        us2PdmMode = false;
        us3PdmMode = false;
        us4PdmMode = false;
        activategpio();
        activebutton="ULTRASONICBUT1";
        disablelayout="ULTRASONICBUT1";
        ui->lineEdit_75->setText(QString::number(us1pulseline));
        ui->lineEdit_77->setText(QString::number(us1ocupulseline));
        ui->lineEdit_76->setText(QString::number(us1ocuburstline));
        ui->lineEdit_78->setText(QString::number(us1singleburstline));
        ui->lineEdit_79->setText(QString::number(us1multiburstline));
        ui->lineEdit_80->setText(QString::number(us1coldphacoline));
        ui->lineEdit_81->setText(QString::number(us1coldphaco1line));


}

void MainWindow::ULTRASONICBUT2()
{
    saveCurrentTabValues();
    currentTab = 2;
    ui->tabWidget->setCurrentIndex(1);

    QString currentText = ui->CutMode_vitCom_2->currentText();
 updateTabsBasedOnComboBox(currentText);

 ui->label_32->show();
 ui->CI5_5->setEnabled(true);
 ui->CI5_5->show();
 handler->phaco_off();
 handler->freq_count(0);

 ui->CutMode_vit_2->show();
 ui->CutMode_vitCom_2->show();
 ui->tabWidget_2->show();
    butname=2;
  handler->buzz();
   us1PdmMode = false;
      us2PdmMode = true;
      us3PdmMode = false;
      us4PdmMode = false;
      handler->safetyvent_off();
      disablelayout="ULTRASONICBUT2";

activebutton="ULTRASONICBUT2";
ui->lineEdit_75->setText(QString::number(us2pulseline));
ui->lineEdit_77->setText(QString::number(us2ocupulseline));
ui->lineEdit_76->setText(QString::number(us2ocuburstline));
ui->lineEdit_78->setText(QString::number(us2singleburstline));
ui->lineEdit_79->setText(QString::number(us2multiburstline));
ui->lineEdit_80->setText(QString::number(us2coldphacoline));
ui->lineEdit_81->setText(QString::number(us2coldphaco1line));
}
void MainWindow::ULTRASONICBUT3()
{
    saveCurrentTabValues();
    currentTab = 3;
    ui->tabWidget->setCurrentIndex(2);

    QString currentText2 = ui->CutMode_vitCom_3->currentText();
   updateTabsBasedOnComboBox(currentText2);

   ui->CI5_5->setEnabled(true);
   ui->CI5_5->show();
   ui->label_32->show();


   ui->tabWidget_2->show();
    butname=3;
     handler->buzz();
     us1PdmMode = false;
        us2PdmMode = false;
        us3PdmMode = true;
        us4PdmMode = false;
        handler->safetyvent_off();
        handler->phaco_off();
        handler->freq_count(0);
        ui->CutMode_vit_3->show();
        ui->CutMode_vitCom_3->show();
        activebutton ="ULTRASONICBUT3";
        disablelayout="ULTRASONICBUT3";
        ui->lineEdit_75->setText(QString::number(us3pulseline));
        ui->lineEdit_77->setText(QString::number(us3ocupulseline));
        ui->lineEdit_76->setText(QString::number(us3ocuburstline));
        ui->lineEdit_78->setText(QString::number(us3singleburstline));
        ui->lineEdit_79->setText(QString::number(us3multiburstline));
        ui->lineEdit_80->setText(QString::number(us3coldphacoline));
        ui->lineEdit_81->setText(QString::number(us3coldphaco1line));
}

void MainWindow::ULTRASONICBUT4()
{
    saveCurrentTabValues();
    currentTab = 4;
    ui->tabWidget->setCurrentIndex(3);

    QString currentText3 = ui->CutMode_vitCom_4->currentText();
    updateTabsBasedOnComboBox(currentText3);

    ui->CI5_5->setEnabled(true);
    ui->CI5_5->show();
    ui->label_32->show();

    ui->tabWidget_2->show();
     butname=4;
     handler->buzz();
      us1PdmMode = false;
         us2PdmMode = false;
         us3PdmMode = false;
         us4PdmMode = true;
         handler->safetyvent_off();
         handler->phaco_off();
         handler->freq_count(0);
         ui->CutMode_vit_4->show();
         ui->CutMode_vitCom_4->show();
         activebutton="ULTRASONICBUT4";
         disablelayout="ULTRASONICBUT4";
         ui->lineEdit_75->setText(QString::number(us4pulseline));
         ui->lineEdit_77->setText(QString::number(us4ocupulseline));
         ui->lineEdit_76->setText(QString::number(us4ocuburstline));
         ui->lineEdit_78->setText(QString::number(us4singleburstline));
         ui->lineEdit_79->setText(QString::number(us4multiburstline));
         ui->lineEdit_80->setText(QString::number(us4coldphacoline));
         ui->lineEdit_81->setText(QString::number(us4coldphaco1line));
  }

int MainWindow::footswitchrange()
{

}

void MainWindow:: disablegpio()
{
    nHandPiece=1;
    nHandPiece1 = 0;  // Set nHandPiece to 1
    currentButtonIndex=0;

        QPushButton *buttons1[] = { ui->DIABUT, ui->IA1BUT, ui->IA2BUT, ui->VITRECTOMYBUT };

//        // Disconnect any previous connections
//        for (int i = 0; i < sizeof(buttons1) / sizeof(buttons1[0]); i++) {
//            disconnect(buttons1[i], &QPushButton::clicked, this, nullptr);
//        }

        // Connect each button's clicked signal to updateCurrentButtonIndex
        for (int i = 0; i < sizeof(buttons1) / sizeof(buttons1[0]); i++) {
            connect(buttons1[i], &QPushButton::clicked, this, [this, i]() {
                updateCurrentButtonIndex(i);
            });
        }
}

void MainWindow::activategpio() {
    nHandPiece = 0;
    nHandPiece1 = 1;
    currentButtonIndex=0;

    QPushButton *buttons[] = {
        ui->ULTRASONICBUT1, ui->ULTRASONICBUT2,
        ui->ULTRASONICBUT3, ui->ULTRASONICBUT4,
        ui->IA1BUT, ui->IA2BUT, ui->VITRECTOMYBUT, ui->DIABUT
    };

    // Connect each button's clicked signal to updateCurrentButtonIndex
    for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++) {
        connect(buttons[i], &QPushButton::clicked, this, [this, i]() {
            updateCurrentButtonIndex(i);
        });
    }
}


void MainWindow::footpedalvalues(int &value1, int &value2, int &value3, int &value4)
{
//qDebug()<<"values are received from the doctor window is"<<value1<<value2<<value3<<value4;
    nfpzero = static_cast<int>(value1*40.90);
    nfpone =  static_cast<int>(value2*40.90);
    nfptwo =  static_cast<int>(value3*40.90);
    nfpthree =  static_cast<int>(value4*40.90);


    //qDebug()<<"that is the range"<<nfpzero<<nfpone<<nfptwo<<nfpthree;
footpedalcheck();

}

void MainWindow::rx_defaultvalues(int &value1, int &value2, int &value3, int &value4)
{
    //qDebug()<<"values are received from the doctor window is"<<value1<<value2<<value3<<value4;
        nfpzero = static_cast<int>(value1/40.90);
        nfpone =  static_cast<int>(value2/40.90);
        nfptwo =  static_cast<int>(value3/40.90);
        nfpthree =  static_cast<int>(value4/40.90);


        //qDebug()<<"that is the range"<<nfpzero<<nfpone<<nfptwo<<nfpthree;
    footpedalcheck();
}



void MainWindow::disablefunction()
{
//   protimer->start(1);
//   Tacutalsensor->start(1);
    this->show();
        this->raise();
        this->activateWindow();
        // restart timers that were stopped
        if (protimer) protimer->start(1);
        if (Tacutalsensor) Tacutalsensor->start(1);

}


void MainWindow::IRRIGATIONBUT1()
{
    ui->tabWidget->setCurrentIndex(4);
    ui->CI5_5->setEnabled(true);
    ui->CI5_5->show();
    ui->CutMode_vit->hide();
    ui->CutMode_vitCom->hide();
    ui->CutMode_vit_2->hide();
    ui->CutMode_vitCom_2->hide();
    ui->CutMode_vit_3->hide();
    ui->CutMode_vitCom_3->hide();
    ui->CutMode_vit_4->hide();
    ui->CutMode_vitCom_4->hide();
     ui->tabWidget_2->hide();
     butname=5;
     handler->buzz();
     ui->label_32->hide();
     ui->tabWidget_2->hide();
     handler->safetyvent_off();
     disablelayout = "IRRIGATION1";


 }

void MainWindow::IRRIGATIONBUT2()
{
    ui->tabWidget->setCurrentIndex(5);
    ui->CI5_5->setEnabled(true);
    ui->CI5_5->show();

    ui->CutMode_vit->hide();
    ui->CutMode_vitCom->hide();
    ui->CutMode_vit_2->hide();
    ui->CutMode_vitCom_2->hide();
    ui->CutMode_vit_3->hide();
    ui->CutMode_vitCom_3->hide();
    ui->CutMode_vit_4->hide();
    ui->CutMode_vitCom_4->hide();
     ui->tabWidget_2->hide();
     butname=6;
     handler->buzz();
     ui->label_32->hide();
     ui->tabWidget_2->hide();
     handler->safetyvent_off();
     disablelayout = "IRRIGATION2";

   }

void MainWindow::VITRECTOMYBUT()
{
    ui->tabWidget->setCurrentIndex(6);
    ui->CI5_5->setEnabled(true);
ui->CI5_5->show();
    butname=7;
    handler->buzz();
    ui->label_32->hide();
    ui->tabWidget_2->hide();
    handler->safetyvent_off();
    ui->CutMode_vit->hide();
    ui->CutMode_vitCom->hide();
    ui->CutMode_vit_2->hide();
    ui->CutMode_vitCom_2->hide();
    ui->CutMode_vit_3->hide();
    ui->CutMode_vitCom_3->hide();
    ui->CutMode_vit_4->hide();
    ui->CutMode_vitCom_4->hide();
    disablelayout="VITRECTOMY";
}

void MainWindow::DIATHERMYBUT() {
    // Emit buzz sound
    handler->buzz();

    // Disable safety vent and motor
    handler->safetyvent_off();
    motoroff();

    // Switch to Diathermy tab
    ui->tabWidget->setCurrentIndex(7);
    ui->CI5_5->hide();
    overallci=false;

    // Hide unnecessary UI elements for Diathermy mode
    ui->label_32->hide();
    ui->CutMode_vit->hide();
    ui->CutMode_vitCom->hide();
    ui->CutMode_vit_2->hide();
    ui->CutMode_vitCom_2->hide();
    ui->CutMode_vit_3->hide();
    ui->CutMode_vitCom_3->hide();
    ui->CutMode_vit_4->hide();
    ui->CutMode_vitCom_4->hide();
    ui->tabWidget_2->hide();

    // Reset button name
    butname = 0;
   disablelayout="DIATHERMY";
    // Trigger current tab setup
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
     if(value > 2520)
    {
        value=2520;
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
    handler->phaco_power(0);
    //handler->convert_dac(0x30, 0);
    handler->fs_count_limit(nfpzero+nfpone+nfptwo);
    handler->freq_count(0);
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
                         "    image: url(:/images/pushbutton.png);"
                         "    height: 91px;"
                         "    width: 91px;"
                         "    color: white;"
                         "    border-radius: 40px;"
                         "    outline: none;"  // Removes focus outline
                         "}";

    QString styleSheet1 = "QPushButton {"
                          "    font-family: Ubuntu;"
                          "    font-size: 15pt;"
                          "    background-color: transparent;"
                          "    image: url(:/images/glass.png);"
                          "    height: 81px;"
                          "    width: 81px;"
                          "    color: black;"
                          "    border-radius: 40px;"
                          "    outline: none;"  // Removes focus outline
                          "}";

    QString styleSheet3 = "QPushButton {"
                          "    font-family: Ubuntu;"
                          "    font-size: 13pt;"
                          "    background-color: transparent;"
                          "    height: 81px;"
                          "    width: 81px;"
                          "    image: url(:/images/newbut.png);"
                          "    border-radius: 40px;"
                          "    outline: none;"  // Removes focus outline
                          "}";

    QString styleSheet4 = "QPushButton {"
                          "    font-family: Ubuntu;"
                          "    font-size: 13pt;"
                          "    background-color: transparent;"
                          "    height: 81px;"
                          "    width: 81px;"
                          "    image: url(:/images/irr.png);"
                          "    color: black;"
                          "    border-radius: 40px;"
                          "    outline: none;"  // Removes focus outline
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
    //ui->elapsed_time->hide();
    ui->elapsed_time_2->hide();
   // ui->label_28->hide();
   // ui->label_26->hide();
    //ui->label_32->hide();
    switch (tab) {
        case 0:
            ui->ULTRASONICBUT1->setStyleSheet(styleSheet);
            ui->label_3->show();
           ui->label_32->show();
            ui->elapsed_time->show();
            ui->label_26->show();
            ui->label_28->show();
            ui->elapsed_time_2->show();

            break;
        case 1:
            ui->ULTRASONICBUT2->setStyleSheet(styleSheet);
            ui->label_4->show();
            ui->elapsed_time->show();
            ui->label_26->show();
            ui->label_28->show();
            ui->elapsed_time_2->show();
            ui->label_32->show();
            break;
        case 2:
            ui->ULTRASONICBUT3->setStyleSheet(styleSheet);
            ui->label_14->show();
            ui->elapsed_time->show();
            ui->label_26->show();
            ui->label_28->show();
            ui->elapsed_time_2->show();
            ui->label_32->show();
            break;
        case 3:
            ui->ULTRASONICBUT4->setStyleSheet(styleSheet);
            ui->elapsed_time->show();
            ui->label_26->show();
            ui->label_28->show();
            ui->elapsed_time_2->show();
            ui->label_12->show();
            ui->label_32->show();
            break;
        case 4:
            ui->IA1BUT->setStyleSheet(styleSheet);
            ui->label_13->show();
           ui->label_32->hide();
           ui->elapsed_time->show();
                       ui->label_28->show();
                       ui->elapsed_time_2->show();
                       ui->label_26->show();

            break;
        case 5:
            ui->IA2BUT->setStyleSheet(styleSheet);
            ui->label_5->show();
            ui->label_32->hide();
            ui->elapsed_time->show();
                        ui->label_28->show();
                        ui->elapsed_time_2->show();
                        ui->label_26->show();
            break;
        case 6:
            ui->VITRECTOMYBUT->setStyleSheet(styleSheet);
            ui->label_6->show();
           ui->label_32->hide();
           ui->elapsed_time->show();
                       ui->label_28->show();
                       ui->elapsed_time_2->show();
                       ui->label_26->show();
            break;
        case 7:
            ui->DIABUT->setStyleSheet(styleSheet);
            ui->label_11->show();
           ui->label_32->hide();
           ui->elapsed_time->show();
                       ui->label_28->show();
                       ui->elapsed_time_2->show();
                       ui->label_26->show();
            break;
        default:
            break;
    }


}

//if tune is completed then only
void MainWindow::setTuneMode() {
    handler->phaco_off();
    handler->phaco_power(0);
    handler->freq_count(0);
     handler->convert_dac(0x30, 0);                // ADD THIS - reset the frequency count variable
    qDebug() << "[TUNE COMPLETE] nFreqCount reset to =" << nFreqCount;

    QString styleSheet4 = "QLabel {"
               "image: url(:/images/Tuned.png);"

                "background-color:transparent;"

                                     "}";
    QString styleSheet5 = "QLabel {"
               "image: url(:/images/not_Tuned.png);"

                "background-color:transparent;"

                                     "}";
    QString styleSheetOn = "QPushButton {"
                           " font:20pt Ubuntu;"
                           " background-color: green;"
                           " color: black;"
                           " border:5px solid black;"
                           " border-radius:30px;"
                           " font-weight: bold;"
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
    ui->us1onoff->setText("ON");
    ui->us1onoff->setStyleSheet(styleSheetOn);
    ui->us1powup_but->setEnabled(true);
    ui->us1powdown_but->setEnabled(true);
    ui->us2onoff->setText("ON");
    ui->us2onoff->setStyleSheet(styleSheetOn);
    ui->us2powup_but->setEnabled(true);
    ui->us2powdown_but->setEnabled(true);
    ui->us3onoff->setText("ON");
    ui->us3onoff->setStyleSheet(styleSheetOn);
    ui->us3powup_but->setEnabled(true);
    ui->us3powdown_but->setEnabled(true);
    ui->us4onoff->setText("ON");
    ui->us4onoff->setStyleSheet(styleSheetOn);
    ui->us4powup_but->setEnabled(true);
    ui->us4powdown_but->setEnabled(true);
   // ui->label_32->show();
    ui->us1onoff->setEnabled(true);
    ui->us2onoff->setEnabled(true);
    ui->us3onoff->setEnabled(true);
    ui->us4onoff->setEnabled(true);
  //  nHandPiece = 0;  // Set nHandPiece to 0

  resetEptUsd();


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

//        ui->vitonoff->setStyleSheet(styleSheetOff);
//        ui->vitonoff->setText("OFF");
//        ui->vitpowup_but->setEnabled(false);
//        ui->vitpowdown_but->setEnabled(false);
//        //qDebug()<<"the power is off";


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
void MainWindow::beepsound(int position )
{
    if(vibon == "Vibration ON"){
        handler->vibrator_on(1,position,300);
ui->pushButton_2->setStyleSheet("image: url(:/images/vibrationon.png);border-radius:20px;border:none;");
    }else{
handler->vibrator_off();
handler->vibrator_on(0,0,0);
        ui->pushButton_2->setStyleSheet("image: url(:/images/vibrationoff.png);border-radius:20px;border:none;");
    }
   // qDebug()<<"beep";
//    QProcess p;
//    QString path = "/usr/bin/beep";
//    p.setProgram(path);
//    p.start();
    //    p.waitForFinished();
}

void MainWindow::receivesignal(const QString &text)
{


}
void MainWindow::onCutMode_vitComChanged(int index) {

    QString modeText = ui->CutMode_vitCom->currentText();
   updateTabsBasedOnComboBox(modeText);


}

void MainWindow::onCutMode_vitComChanged1(int index) {
    QString currentText = ui->CutMode_vitCom_2->currentText();
    updateTabsBasedOnComboBox(currentText);
  //  qDebug()<<"the mode us2 is"<<currentText;

}

void MainWindow::onCutMode_vitComChanged2(int index) {
    QString currentText = ui->CutMode_vitCom_3->currentText();
    updateTabsBasedOnComboBox(currentText);
    //qDebug()<<"the mode is"<<currentText;

}

void MainWindow::onCutMode_vitComChanged3(int index) {
    QString currentText = ui->CutMode_vitCom_4->currentText();
    updateTabsBasedOnComboBox(currentText);

}

void MainWindow::updateTabsBasedOnComboBox(const QString &selected) {
    bool modeFound = false;
    handler->fs_count_limit(nfpzero+nfpone+nfptwo);
    handler->phaco_off();
    ui->label_32->show();

    if (selected == "Continuous") {
        ui->tabWidget_2->setCurrentIndex(0);
        handler->pdm_mode(CONTINOUS);
        handler->freq_count(nFreqCount);
//       handler->phaco_on(nfpzero+nfpone+nfptwo);
        modeFound = true;
        ui->label_32->hide();
        handler->buzz();
    }
    else if (selected == "Pulse") {
        ui->tabWidget_2->setCurrentIndex(1);
        ui->label_32->show();

        handler->freq_count(nFreqCount);
        handler->pdm_mode(PULSE_MODE);
        nPulseCount = ui->lineEdit_75->text().toInt();
        handler->pulse_count(nPulseCount);
       // qDebug() << "The pulse count is:" << nPulseCount;

        modeFound = true;
        handler->buzz();
    }
    else if (selected == "Ocupulse") {
        ui->tabWidget_2->setCurrentIndex(2);
          ui->label_32->show();
        //handler->phaco_on(nfpzero+nfpone+nfptwo);
        handler->freq_count(nFreqCount);
        handler->pdm_mode(CONTINOUS);



        modeFound = true;

        handler->buzz();

    }
    else if (selected == "Ocuburst") {
        ui->tabWidget_2->setCurrentIndex(3);
        //handler->phaco_on(nfpzero+nfpone+nfptwo);
        handler->freq_count(nFreqCount);
        handler->pdm_mode(CONTINOUS);

        nOcuBurstCount = ui->lineEdit_77->text().toInt();
        handler->pulse_count(nOcuBurstCount);

        modeFound = true;
       ui->label_32->show();
        handler->buzz();

    }
    else if (selected == "Single burst") {
        ui->tabWidget_2->setCurrentIndex(4);
        handler->pdm_mode(SINGLE_BURST);
       // handler->phaco_on(nfpzero+nfpone+nfptwo);
        handler->freq_count(nFreqCount);
        handler->phaco_power(100);
        nSingleBurst=ui->lineEdit_78->text().toInt();
        handler->burst_length(nSingleBurst);

//qDebug()<<"the single burst mode is workinggggg";
modeFound = true;
       ui->label_32->show();
        handler->buzz();

    }
    else if (selected == "Multi burst") {
        ui->tabWidget_2->setCurrentIndex(5);
        handler->pdm_mode(MULTI_BURST);
      //  handler->phaco_on(nfpzero+nfpone+nfptwo);
        handler->freq_count(nFreqCount);
       // qDebug() << "The resonant freq_count is:" << nFreqCount;

        handler->phaco_power(100);

        nMultiBurstCount = ui->lineEdit_79->text().toInt();
        handler->burst_length(nMultiBurstCount);

        modeFound = true;
        ui->label_32->show();
        handler->buzz();

    }
    else if (selected == "Cold phaco") {
        ui->tabWidget_2->setCurrentIndex(6);
        //handler->phaco_on(nfpzero+nfpone+nfptwo);
      //  handler->fs_count(nfpzero + nfpone + nfptwo);
        handler->freq_count(nFreqCount);
        handler->pdm_mode(COLD_PHACO);
        nColdPhacoPer=ui->lineEdit_80->text().toInt();
        nColdPhacoCount=ui->lineEdit_81->text().toInt();
        handler->cold_pulse(nColdPhacoPer,nColdPhacoCount);

        modeFound = true;

        ui->label_32->show();
        handler->buzz();

    }

    if (!modeFound) {
      //  qDebug() << "Unknown mode selected!";
    }

    // qDebug() << "Mode is working: " << selected;
}

void MainWindow::disablesetTunemode()
{

    handler->phaco_off();
        handler->phaco_power(0);
        //handler->convert_dac(0x30, 0);  // ADD THIS - zero DAC before tuning
        handler->freq_count(0);
    QString styleSheet4 = "QLabel {"
               "image: url(:/images/Tuned.png);"

                "background-color:transparent;"

                                     "}";
    QString styleSheet5 = "QLabel {"
               "image: url(:/images/not_Tuned.png);"

                "background-color:transparent;"

                                     "}";
    QString styleSheetOn = "QPushButton {"
                           " font:20pt Ubuntu;"
                           " background-color: green;"
                           " color: black;"
                           " border:5px solid black;"
                           " border-radius:30px;"
                           " font-weight: bold;"
                           "}";
// Set the flag based on the argument
 // Enable or disable the US buttons based on the flag
   //QMessageBox::information(nullptr,"Info","Tune is completed");
 ui->ULTRASONICBUT1->setEnabled(false);    // us1
    ui->label_16->setStyleSheet(styleSheet5);
    ui->label_17->setStyleSheet(styleSheet5);
    ui->label_18->setStyleSheet(styleSheet5);
    ui->label_27->setStyleSheet(styleSheet5);
    ui->ULTRASONICBUT2->setEnabled(false);  // us2
    ui->ULTRASONICBUT3->setEnabled(false);  // us3
    ui->ULTRASONICBUT4->setEnabled(false);  // us4
ui->label_19->setStyleSheet("background-color:transparent;border:none;image: url(:/images/singletick.png);");
    ui->us1onoff->setEnabled(false);
    ui->us2onoff->setEnabled(false);
    ui->us3onoff->setEnabled(false);
    ui->us4onoff->setEnabled(false);
    handler->phaco_off();
    handler->freq_count(0);
    handler->phaco_power(0);

    //  nHandPiece = 0;  // Set nHandPiece to 0
}

void MainWindow::rx_swaponoff(const QString &swap)
{
 swapingonoff=swap;
 //qDebug()<<"swaping onoff"<<swapingonoff;
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
    int val=readsensorvalue();

    QString text=ui->comboBox->currentText();
 us1=ui->us1mode->text();
 us2=ui->us2mode->text();

  QString ia1=ui->ia1mode->text();
  QString ia2=ui->ia2mode->text();
 vus1=ui->us1vacmode->text();
 vus2=ui->us2vacmode->text();
 us3=ui->us3mode->text();
vus3=ui->us3vacmode->text();
us4=ui->us4mode->text();
vus4=ui->us4vacmode->text();
  QString vit=ui->vitmode->text();
  QString vvac=ui->vitvacmode->text();
  //QString us2on=ui->us2onoff->text();

    switch(butname)
    {

    // dia
    case 0:
    {
        int pow = (ui->lineEdit_74->text().toInt() * 255) / 100;
        bool flag = true;
        stopPhacoTracking();
        handler->phaco_off();
            handler->fs_count_limit(nfpzero+nfpone+nfptwo);

        handler->phaco_power(0);
        handler->pinchvalve_off();
       motoroff();

            if (range > 0 && range < nfpzero) {
                ui->pushButton_42->setText("0");
                ui->dial_2->setValue(range);
                handler->dia_off();
               handler->vibrator_off();

                if (!overallci ) {
                    ui->CI5_5->setStyleSheet(styleSheet4);
                    handler->pinchvalve_off();
                    handler->safetyvent_off();
                }


                beepsound(0);
                flag=true;
                handler->speaker_off();
            }
            else if (range >= nfpzero && range < (nfpzero + nfpone + nfptwo + nfpthree)) {
                ui->pushButton_42->setText("1");
                handler->vibrator_on(1,1,300);
                ui->dial_2->setValue(range);
                if(pow>70)
                    pow=70;
                handler->dia_on();
                handler->dia_count(pow);
  beepsound(1);
                if (!overallci) {
                    ui->CI5_5->setStyleSheet(styleSheet4);
                    handler->pinchvalve_off();
                    handler->safetyvent_off();
                }

                if(speakeronoff == "Speaker ON"){
                    handler->speaker_on(0,0,1,0);
                }else{
                      handler->speaker_off();
                }


   }
        break;
    }
//ULTRASONICBUT 1
  //us1
    case 1: {
        //they will not run
        int pow1 = ui->lineEdit_57->text().toInt(); // Get the power value from the line edit
        int vacline=ui->lineEdit_55->text().toInt();
        QString text=ui->us1onoff->text();
        bool flag1 = false;
        QString us1powmode=ui->CutMode_vitCom->currentText();
         int nOfftime=0;
         int us1reachedvac;
    //    qDebug()<<nfpzero<<nfpone<<nfptwo<<nfpthree<<"those are reterved from the footpedal screen";

 //qDebug()<<"the nonlinear vaccum is"<<nonlinear_prevac;
        if (us1 == "Panel"||vus1=="Panel"){
            if (range>0 && range<nfpzero) {
                ui->pushButton_42->setText("0");
                handler->speaker_off();
                ui->dial_2->setValue(range);
                    handler->freq_count(0);
                    stopPhacoTracking();
                    handler->phaco_off();
                    handler->phaco_power(0);
                    handler->convert_dac(0x30, 0);
                    qDebug() << "[US1 POS0] range=" << range
                                     << "nfpzero=" << nfpzero
                                     << "phaco_off and power(0) called";
                    handler->fs_count_limit(nfpzero+nfpone+nfptwo);
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
          beepsound(0);
                flag1 = true; // Reset flag
}
            else if (range >= nfpzero & range < nfpzero+nfpone) {
                     ui->pushButton_42->setText("1");
                     ui->dial_2->setValue(range);
                     handler->pinchvalve_on();
                     if(ventonus1==false){
                     handler->safetyvent_on();
                   QThread::msleep(100);
                   handler->safetyvent_off();
                   ventonus1=true;
                     }
                     if(speakeronoff == "Speaker ON"){
                     handler->speaker_on(0,0,1,0);
                     }else{
                         handler->speaker_off();
                     }
                  int pro=readsensorvalue();
                  ui->label_8->setText(QString::number(pro));
                  ui->CI5_5->setStyleSheet(styleSheet3);
                  //footpedalbeep();
                  motoroff();
                  ui->label_7->setText("0");

                  handler->freq_count(0);
                  stopPhacoTracking();
                  handler->phaco_off();
                //  handler->fs_count(0);
                 us1poweron=false;
  beepsound(1);
  flag1 = true;
            }
            else if (range >=(nfpone+nfpzero) && range < (nfpzero+nfpone+nfptwo)) {
                ui->pushButton_42->setText("2");
                //qDebug()<<"the postion is 2nd";
                ui->label_8->show();
                ventonus1=false;
                ui->dial_2->setValue(range);
                ui->label_7->setText("0");
                ui->CI5_5->setStyleSheet(styleSheet3);
                beepsound(2);
                handler->pinchvalve_on();
                handler->freq_count(0);
                stopPhacoTracking();
                handler->phaco_off();
               // handler->fs_count(0);
                us1poweron= false;
                if(vus1=="Panel" ){
                    int nonlinear_prevac=readsensorvalue();
                    ui->label_8->setText(QString::number(nonlinear_prevac));
                    if(nonlinear_prevac <=  vacline){
                     motoron(ui->lineEdit_56);
                     if(speakeronoff == "Speaker ON"){
                  handler->speaker_on(nonlinear_prevac,1,0,0);
                     }else{
                         handler->speaker_off();
                     }
                    }


                  if (nonlinear_prevac > vacline) {
                                   motoroff(); // Turn off the motor
                                   ui->label_8->setText(QString::number(vacline));
                                   if(speakeronoff == "Speaker ON"){
                                  handler->speaker_on(nonlinear_prevac,0,0,1);
                                   }else{
                                       handler->speaker_off();
                                   }
                                   if(ui->label_8->text()==vacline){
                                   ui->label_8->setText(QString::number(vacline));
                                   }
                               }

                  if(us1reachedvac == vacline){
                      motoroff();
                  }
                }
                flag1 = true; // Reset flag
            }
            else if (range >=(nfpzero+nfpone+nfptwo) && range < (nfpzero+nfpone+nfptwo+nfpthree)) {
                ui->pushButton_42->setText("3");
                 ui->dial_2->setValue(range);
                ui->label_7->show();
                ui->label_8->show();
                //footpedalbeep();
                beepsound(3);
                ventonus1=false;
                handler->pinchvalve_on();
                //handler->safetyvent_off();
                ui->CI5_5->setStyleSheet(styleSheet3);
                if(us1 == "Panel"){
                if((!us1poweron && text == "ON")){

                    startPhacoTracking(pow1);      // pow1 is already a 0–1 fraction in that block
                        handler->phaco_on(range);
                        handler->freq_count(nFreqCount);

                        if(us1powmode == "Multi burst"){
                                    if(range>(nfpzero+nfpone+nfptwo)){
                                     double upper=static_cast<int>(range)-static_cast<int>(nfpzero+nfpone+nfptwo);
                                    double lower=static_cast<int>(nfpzero+nfpone+nfptwo+nfpthree)-static_cast<int>(nfpzero+nfpone+nfptwo);
                                double intialontime=static_cast<double>(upper)/static_cast<double>(lower);
                                double ontime=static_cast<double>(intialontime)*120;
                                int rounded_Value=std::round(ontime);
                                nOfftime=120-static_cast<int>(ontime);
                                handler->burst_off_length(nOfftime);
                                }

                        }
                        ui->label_7->setText(QString::number(pow1));
                        handler->convert_dac(0x30,pow1);
                        // Ensure this is called once, when the timer should start
                        //qDebug()<<"the mode is from us1 is"<<ui->CutMode_vitCom->currentText();
                        if (!elapsedTimer->isValid()) {
                            elapsedTimer->start();
                            qDebug() << "Timer started!";
                        }                      //  cumulativeElapsedTimeSec=0;
                    //    cumulativeEffectiveTimeSec=0;
                        updateTimes(pow1,ui->elapsed_time, ui->elapsed_time_2);

 }else {
                    stopPhacoTracking();
                    handler->phaco_off();
                    handler->freq_count(0);
                    handler->phaco_power(0);
                    handler->fs_count_limit(nfpzero);

                 }
                }

if(vus1 == "Panel"){
     int nonlinear_prevac=readsensorvalue();
                            //  int nonlinear_vac = std::min(nonlinear_prevac, vacline);
     ui->label_8->setText(QString::number(nonlinear_prevac));

     if(nonlinear_prevac < vacline){
                              motoron(ui->lineEdit_56);
                              if(us1powmode == "Ocuburst"){
                                  handler->pdm_mode(CONTINOUS);
                              }else if(us1powmode == "Ocupulse" ){
                                  handler->pdm_mode(CONTINOUS);
                              }
                              if(speakeronoff == "Speaker ON"){
                              handler->speaker_on(nonlinear_prevac,1,0,0);
                              }else{
                                  handler->speaker_off();
                              }
     }

                              if (nonlinear_prevac >= vacline) {
                                  motoroff(); // Turn off the motori
                                  ui->label_8->setText(QString::number(vacline));

                                  if(speakeronoff == "Speaker ON"){
                                  handler->speaker_on(0,0,0,1);
                                  }else{
                                      handler->speaker_off();
                                  }
                                  if (us1powmode == "Ocuburst") {
                                      handler->pdm_mode(SINGLE_BURST);

                                     nOcuBurstCount = ui->lineEdit_76->text().toInt();
                                      handler->burst_length(  nOcuBurstCount ); // Set burst length

                                      // Update the label with the power value
                                      ui->label_7->setText(QString::number(pow1));
                                  }

                                  else if(us1powmode == "Ocupulse" ){
                                      handler->pdm_mode(PULSE_MODE);
                                    nOcuPulseCount = ui->lineEdit_77->text().toInt();
                                      handler->pulse_count(nOcuPulseCount);
                                  }
                                  us1reachedvac=nonlinear_prevac;
                              }

}

 }

  }
        if ((us1 == "Surgeon")||( vus1 == "Surgeon")) {
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

                stopPhacoTracking();
                handler->phaco_off();
                 handler->phaco_power(0);
                 handler->convert_dac(0x30, 0);
                 handler->freq_count(0);
                 handler->fs_count_limit(nfpzero+nfpone+nfptwo);
                ui->label_7->setText("0");
                handler->speaker_off();
                beepsound(0);
                int pro = readsensorvalue();
                 ui->label_8->setText(QString::number(pro));
                us1poweron=false;
                flag1 = true; // Reset flag

            } else if (range >= nfpzero && range < (nfpzero + nfpone)) {
                // Position 1 behavior
                ui->pushButton_42->setText("1");
                //footpedalbeep();
                ui->dial_2->setValue(range);
                ui->label_7->setText("0");

                handler->pinchvalve_on();

                if(ventonus1==false){
                    handler->safetyvent_on();
              QThread::msleep(100);
             handler->safetyvent_off();
              ventonus1=true;
                }
                int pro = readsensorvalue();
                 ui->label_8->setText(QString::number(pro));
                ui->CI5_5->setStyleSheet(styleSheet3);
                motoroff();
                ui->label_7->setText("0");

                stopPhacoTracking();
                handler->phaco_off();
                handler->phaco_power(0);
                handler->freq_count(0);
             //   handler->fs_count(0);
                if(speakeronoff == "Speaker ON"){
                handler->speaker_on(0,0,1,0);
                }else{
                    handler->speaker_off();
                }
                us1poweron=false;

                beepsound(1);

                flag1 = true; // Reset flag

            } else if (range >= (nfpzero + nfpone) && range < (nfpzero + nfpone + nfptwo)) {
                // Position 2 behavior
                ui->pushButton_42->setText("2");
                ui->dial_2->setValue(range);
                ui->label_7->setText("0");

                ventonus1 = false;

                handler->pinchvalve_on();
                ui->CI5_5->setStyleSheet(styleSheet3);


                if (vus1 == "Surgeon") {
                    const int MIN_RANGE = nfpzero + nfpone;
                    const int MAX_RANGE = nfpzero + nfpone + nfptwo;
                    int divi = MAX_RANGE - MIN_RANGE;  // Divider
                    int calibration = range - MIN_RANGE;  // Calibration value

                    if (divi != 0) {
                        double final = static_cast<double>(vacline) / static_cast<double>(divi);
                        int presetvac = static_cast<int>(std::round(calibration * final));
                        int pro = readsensorvalue();
                        ui->label_8->setText(QString::number(pro));
                        if(speakeronoff == "Speaker ON"){
                        handler->speaker_on(pro,1,0,0);
                        }else{
                            handler->speaker_off();
                        }

                        // Motor control based on preset value
                        if (pro < presetvac) {
                            motoron(ui->lineEdit_56);

                            if (!motorus1) {
                                motorus1= true;
                            }
                        } else if (motorus1) {
                            motoroff();
                            motorus1= false;
                        }

                        // If pro exceeds vitpreset, cap it
                        if (pro >= vacline) {
                            pro = static_cast<int>(vacline);
                            ui->label_8->setText(QString::number(pro));
                            motoroff();
                            if(speakeronoff == "Speaker ON"){
                            handler->speaker_on(0,0,0,1);
                            }else{
                                handler->speaker_off();
                            }
                        }
                    }

                }



                stopPhacoTracking();
                handler->phaco_off();
                handler->phaco_power(0);
                handler->freq_count(0);
                handler->fs_count_limit(nfpzero);
                us1poweron=false;
                beepsound(2);

                flag1 = true; // Reset flag

            } else if (range >= (nfpzero + nfpone + nfptwo) && range < (nfpzero + nfpone + nfptwo + nfpthree)) {
                int nOfftime=0;
                ui->pushButton_42->setText("3");
                ui->dial_2->setValue(range);
                //footpedalbeep();
                ventonus1 = false;
                handler->pinchvalve_on();
               // handler->safetyvent_off();
                ui->CI5_5->setStyleSheet(styleSheet3);
                beepsound(3);

                if(ui->us1mode->text() == "Surgeon"){
                 if (!us1poweron && text == "ON") {

                     startPhacoTracking(pow1);      // pow1 is already a 0–1 fraction in that block
                         handler->phaco_on(range);
                         handler->freq_count(nFreqCount);
                     if(ui->CutMode_vitCom->currentText()== "Multi burst"){


                                 double upper=static_cast<int>(range)-static_cast<int>(nfpzero+nfpone+nfptwo);
                                 double lower=static_cast<int>(nfpzero+nfpone+nfptwo+nfpthree)-static_cast<int>(nfpzero+nfpone+nfptwo);
                             double intialontime=static_cast<double>(upper)/static_cast<double>(lower);
                             double ontime=static_cast<double>(intialontime)*120;
                             int rounded_Value=std::round(ontime);
                             nOfftime=120-static_cast<int>(ontime);
                             handler->burst_off_length(nOfftime);
                             nMultiBurstCount = ui->lineEdit_79->text().toInt();
                             handler->burst_length(nMultiBurstCount);




                     }

                     float progress4 = ((range - static_cast<float>(nfpone + nfptwo + nfpzero)) /
                                        (static_cast<float>(nfpzero+nfpone+nfptwo+nfpthree) - static_cast<float>(nfpone + nfptwo + nfpzero)) )* pow1;
                     int rounded_value=std::round(progress4);
                      handler->phaco_power(rounded_value);
                     ui->label_7->setText(QString::number(rounded_value));

 if (!elapsedTimer->isValid()) {
     elapsedTimer->start();
 }
 updateTimes(pow1,ui->elapsed_time,ui->elapsed_time_2);



                 }else {  // If pushButton is OFF
                    stopPhacoTracking();
                     handler->phaco_off();
                    handler->freq_count(0);
                    handler->phaco_power(0);
                 }
                }
                if (vus1 == "Surgeon") {
                    const int MIN_RANGE = nfpzero + nfpone + nfptwo;
                    const int MAX_RANGE = nfpzero + nfpone + nfptwo + nfpthree;
                    int divi = MAX_RANGE - MIN_RANGE;
                    int calibration = range - MIN_RANGE;

                    if (divi != 0) {
                        double final = static_cast<double>(vacline) / static_cast<double>(divi);
                        int presetvac = static_cast<int>(std::round(calibration * final));
                        int pro = readsensorvalue();
                        ui->label_8->setText(QString::number(pro));
                        if(speakeronoff == "Speaker ON"){
                        handler->speaker_on(presetvac,1,0,0);
                        }else{
                            handler->speaker_off();
                        }
                        if(us1powmode == "Ocuburst"){
                            handler->pdm_mode(CONTINOUS);
                        }else if(us1powmode == "Ocupulse" ){
                            handler->pdm_mode(CONTINOUS);
                        }
                        if (pro < presetvac) {
                            motoron(ui->lineEdit_56);
                            if (!motorus1) {
                                motorus1 = true;
                            }
                        } else if (motorus1) {
                            motoroff();
                            motorus1 = false;
                        }

                        if (pro >= vacline) {
                            pro = static_cast<int>(vacline);
                            ui->label_8->setText(QString::number(pro));
                            if(speakeronoff == "Speaker ON"){
                            handler->speaker_on(pro,0,0,1);
                            }else{
                                handler->speaker_off();
                            }
                            if(us1powmode == "Ocuburst"){
                                handler->pdm_mode(SINGLE_BURST);
                            }else if(us1powmode == "Ocupulse" ){
                                handler->pdm_mode(PULSE_MODE);
                            }
                            motoroff();
                        }
                    }
                }

}
        }


        break;
    }
    case 2: {
        //they will not run
        int pow2 = ui->lineEdit_58->text().toInt(); // Get the power value from the line edit
        int us2vacline=ui->lineEdit_60->text().toInt();
        QString text=ui->us2onoff->text();
        bool flag1 = false;
        QString us2powmode=ui->CutMode_vitCom_2->currentText();
         int nOfftime;
           int us2reachedvac;
        // qDebug()<<"the us2 mode is"<<us2<<"the us2 vacmode is"<<vus2;
        if (us2 == "Panel"||vus2=="Panel"){
            if (range>0 && range<nfpzero) {
                ui->pushButton_42->setText("0");
                handler->speaker_off();
                ui->dial_2->setValue(range);
                    handler->freq_count(0);
                    stopPhacoTracking();
                    handler->phaco_off();
                    handler->phaco_power(0);
                    handler->convert_dac(0x30, 0);
                        handler->fs_count_limit(nfpzero+nfpone+nfptwo);

                     us2poweron=false;
                       if(!overallci){
                           ui->CI5_5->setStyleSheet(styleSheet4);
                           handler->pinchvalve_off();
                    }

                       if(ventonus2==false){
                        //   handler->pinchvalve_on();
                 handler->safetyvent_on();
                     QThread::msleep(100);
                       handler->pinchvalve_off();
                     handler->safetyvent_off();
                     ventonus2=true;
                       }
                       int pro=readsensorvalue();
                       ui->label_93->setText(QString::number(pro));
                   motoroff();

                ui->label_92->setText("0");

                beepsound(0);
                flag1 = true; // Reset flag
}
            else if (range >= nfpzero & range < nfpzero+nfpone) {
                     ui->pushButton_42->setText("1");
                     ui->dial_2->setValue(range);
                     handler->pinchvalve_on();
                     if(ventonus2==false){
                     handler->safetyvent_on();
                   QThread::msleep(100);
                   handler->safetyvent_off();
                   ventonus2=true;
                     }

                  int pro=readsensorvalue();
                  ui->label_93->setText(QString::number(pro));
                  ui->CI5_5->setStyleSheet(styleSheet3);
                  //footpedalbeep();
                  motoroff();
                  ui->label_92->setText("0");
                  if(speakeronoff == "Speaker ON"){
                  handler->speaker_on(0,0,1,0);
                  }else{
                      handler->speaker_off();
                  }
                  handler->freq_count(0);
                  stopPhacoTracking();
                  handler->phaco_off();
                //  handler->fs_count(0);
                 us2poweron=false;
                 beepsound(1);
flag1 = true;
            }
            else if (range >=(nfpone+nfpzero) && range < (nfpzero+nfpone+nfptwo)) {
                ui->pushButton_42->setText("2");
              //  qDebug()<<"the postion is 2nd";
                ui->label_93->show();
                ventonus2=false;
                ui->dial_2->setValue(range);
                ui->label_7->setText("0");

                ui->CI5_5->setStyleSheet(styleSheet3);
                handler->pinchvalve_on();
                //handler->safetyvent_off();
                //footpedalbeep();
 if(vus2=="Panel" ){
     int nonlinear_prevac=readsensorvalue();
     if(nonlinear_prevac <=  us2vacline){
       ui->label_93->setText(QString::number(nonlinear_prevac));
      motoron(ui->lineEdit_59);
      if(speakeronoff == "Speaker ON"){
   handler->speaker_on(nonlinear_prevac,1,0,0);
      }else{
          handler->speaker_off();
      }
     }
   if (nonlinear_prevac >  us2vacline) {
                    motoroff(); // Turn off the motor
                    ui->label_93->setText(QString::number(us2vacline));
                    if(speakeronoff == "Speaker ON"){
                   handler->speaker_on(us2vacline,0,0,1);
                    }else{
                        handler->speaker_off();
                    }
                }
   if(us2reachedvac == us2vacline){
       motoroff();
   }
 }
                    handler->freq_count(0);
                    stopPhacoTracking();
                    handler->phaco_off();
                   // handler->fs_count(0);
                    us2poweron= false;
                    beepsound(2);


                flag1 = true; // Reset flag
            }
            else if (range >=(nfpzero+nfpone+nfptwo) && range < (nfpzero+nfpone+nfptwo+nfpthree)) {
                ui->pushButton_42->setText("3");
                 ui->dial_2->setValue(range);
                ui->label_92->show();
                ui->label_93->show();
                ventonus2=false;
                handler->pinchvalve_on();
                ui->CI5_5->setStyleSheet(styleSheet3);
if(vus2=="Panel"){
     int nonlinear_prevac=readsensorvalue();
     if(nonlinear_prevac<=us2vacline){
                              ui->label_93->setText(QString::number(nonlinear_prevac));
                              motoron(ui->lineEdit_59);
                              if(us2powmode == "Ocuburst"){
                                  handler->pdm_mode(CONTINOUS);
                              }else if(us2powmode == "Ocupulse" ){
                                  handler->pdm_mode(CONTINOUS);
                              }
                              if(speakeronoff == "Speaker ON"){
                             handler->speaker_on(nonlinear_prevac,1,0,0);
                              }else{
                                  handler->speaker_off();
                              }
     }
                             if (nonlinear_prevac > us2vacline) {
                                 motoroff(); // Turn off the motor
                                   ui->label_93->setText(QString::number(us2vacline));
                                 if(speakeronoff == "Speaker ON"){
                                 handler->speaker_on(0,0,0,1);
                                 }else{
                                     handler->speaker_off();
                                 }
                                 if(us2powmode == "Ocuburst"){
                                     handler->pdm_mode(SINGLE_BURST);
                                     //handler->phaco_on(range);
                                     startPhacoTracking(pow2);      // pow1 is already a 0–1 fraction in that block
                                         handler->phaco_on(range);
                                         handler->freq_count(nFreqCount);
                                     handler->phaco_power(100);
                                     //handler->freq_count(nFreqCount);
                                     // Retrieve the burst length from the UI
                                    nOcuBurstCount = ui->lineEdit_76->text().toInt();
                                     handler->burst_length(  nOcuBurstCount ); // Set burst length

                                 }else if(us2powmode == "Ocupulse" ){
                                     handler->pdm_mode(PULSE_MODE);
                                   nOcuPulseCount = ui->lineEdit_77->text().toInt();
                                     handler->pulse_count(nOcuPulseCount);
                                 }
                                 us2reachedvac=nonlinear_prevac;
                             }
}
if(us2 == "Panel"){
               if(!us2poweron && text == "ON" ){
                     //  handler->fs_count(range);
                   startPhacoTracking(pow2);      // pow1 is already a 0–1 fraction in that block
                       handler->phaco_on(range);
                       handler->freq_count(nFreqCount);
                       if(us2powmode == "Multi burst"){
                               if(range>(nfpzero+nfpone+nfptwo)){

                                   double upper=static_cast<int>(range)-static_cast<int>(nfpzero+nfpone+nfptwo);
                                   double lower=static_cast<int>(nfpzero+nfpone+nfptwo+nfpthree)-static_cast<int>(nfpzero+nfpone+nfptwo);
                               double intialontime=static_cast<double>(upper)/static_cast<double>(lower);
                               double ontime=static_cast<double>(intialontime)*120;
                               int rounded_Value=std::round(ontime);
                               nOfftime=120-static_cast<int>(ontime);
                               handler->burst_off_length(nOfftime);
                               nMultiBurstCount = ui->lineEdit_79->text().toInt();
                               handler->burst_length(nMultiBurstCount);
                               }




                       }
                       ui->label_92->setText(QString::number(pow2));
                       handler->phaco_power(pow2);
                       // Ensure this is called once, when the timer should start
                       if (!elapsedTimer->isValid()) {
                           elapsedTimer->start();
                          // qDebug() << "Timer started!";
                       }

                       updateTimes(pow2, ui->elapsed_time, ui->elapsed_time_2);

                     }
}
beepsound(3);stopPhacoTracking();


            }

  }
        if ((us2 == "Surgeon")||( vus2 == "Surgeon")) {
            if (range > 0 && range < nfpzero) {
                ui->pushButton_42->setText("0");
                ui->dial_2->setValue(range);
                if (!overallci) {
                    ui->CI5_5->setStyleSheet(styleSheet4);
                    handler->pinchvalve_off();
                }
                if(ventonus2==false){
                   handler->safetyvent_on();
              QThread::msleep(100);
              handler->safetyvent_off();
              ventonus2=true;
                }stopPhacoTracking();
                motoroff();

                stopPhacoTracking();
                handler->phaco_off();
                 handler->phaco_power(0);
                 handler->convert_dac(0x30, 0);
                 handler->freq_count(0);
                     handler->fs_count_limit(nfpzero+nfpone+nfptwo);

                     int pro=readsensorvalue();
                     ui->label_93->setText(QString::number(pro));
                     ui->label_92->setText("0");
                handler->speaker_off();
                beepsound(0);

                us2poweron=false;
                flag1 = true; // Reset flag

            } else if (range >= nfpzero && range < (nfpzero + nfpone)) {
                // Position 1 behavior
                ui->pushButton_42->setText("1");
                //footpedalbeep();
                ui->dial_2->setValue(range);
                ui->label_7->setText("0");

                handler->pinchvalve_on();

                if(ventonus2==false){
                    handler->safetyvent_on();
              QThread::msleep(100);
             handler->safetyvent_off();
              ventonus2=true;
                }

                ui->CI5_5->setStyleSheet(styleSheet3);
                motoroff();
                ui->label_92->setText("0");
                int pro=readsensorvalue();
                ui->label_93->setText(QString::number(pro));
                stopPhacoTracking();
                handler->phaco_off();
                handler->phaco_power(0);
                handler->freq_count(0);
               // handler->fs_count(0);
                if(speakeronoff == "Speaker ON"){
                handler->speaker_on(0,0,1,0);
                }else{
                    handler->speaker_off();
                }
                us2poweron=false;

                beepsound(1);

                flag1 = true; // Reset flag

            } else if (range >= (nfpzero + nfpone) && range < (nfpzero + nfpone + nfptwo)) {
                // Position 2 behavior
                ui->pushButton_42->setText("2");
                //footpedalbeep();
                ui->dial_2->setValue(range);
                ui->label_7->setText("0");

                ventonus2 = false;

                handler->pinchvalve_on();
               // handler->safetyvent_off();
                ui->CI5_5->setStyleSheet(styleSheet3);
                if (vus2 == "Surgeon") {
                    const int MIN_RANGE = nfpzero + nfpone;
                    const int MAX_RANGE = nfpzero + nfpone + nfptwo;
                    int divi = MAX_RANGE - MIN_RANGE;  // Divider
                    int calibration = range - MIN_RANGE;  // Calibration value

                    if (divi != 0) {
                        double final = static_cast<double>(us2vacline) / static_cast<double>(divi);
                        int presetvac = static_cast<int>(std::round(calibration * final));
                        int pro = readsensorvalue();
                        ui->label_93->setText(QString::number(pro));
                        if(speakeronoff == "Speaker ON"){
                        handler->speaker_on(pro,1,0,0);
                        }else{
                            handler->speaker_off();
                        }

                        // Motor control based on preset value
                        if (pro < presetvac) {
                            motoron(ui->lineEdit_59);

                            if (!motorus2) {
                                motorus2= true;
                            }
                        } else if (motorus2) {
                            motoroff();
                            motorus2= false;
                        }

                        // If pro exceeds vitpreset, cap it
                        if (pro >= us2vacline) {
                            pro = static_cast<int>(us2vacline);
                            ui->label_93->setText(QString::number(pro));
                            motoroff();
                            if(speakeronoff == "Speaker ON"){
                            handler->speaker_on(0,0,0,1);
                            }else{
                                handler->speaker_off();
                            }
                        }
                    }

                }


                stopPhacoTracking();
                handler->phaco_off();
                handler->phaco_power(0);
                handler->freq_count(0);
               // handler->fs_count(0);
                us2poweron=false;
                beepsound(2);

                flag1 = true; // Reset flag

            } else if (range >= (nfpzero + nfpone + nfptwo) && range < (nfpzero + nfpone + nfptwo + nfpthree)) {
                ui->pushButton_42->setText("3");
                ui->dial_2->setValue(range);
                ventonus2 = false;
                handler->pinchvalve_on();
                ui->CI5_5->setStyleSheet(styleSheet3);
                if(us2 == "Surgeon"){
                 if (!us2poweron && text == "ON") {
                   //  qDebug()<<"the power is "<<ui->us2onoff->text();
                    // us2poweron=true;
                     startPhacoTracking(pow2);      // pow1 is already a 0–1 fraction in that block
                         handler->phaco_on(range);
                         handler->freq_count(nFreqCount);
                   //  handler->fs_count(range);

                     if(us2powmode == "Multi burst"){
                             if(range>(nfpzero+nfpone+nfptwo)){

                                 double upper=static_cast<int>(range)-static_cast<int>(nfpzero+nfpone+nfptwo);
                                 double lower=static_cast<int>(nfpzero+nfpone+nfptwo+nfpthree)-static_cast<int>(nfpzero+nfpone+nfptwo);
                             double intialontime=static_cast<double>(upper)/static_cast<double>(lower);
                             double ontime=static_cast<double>(intialontime)*120;
                             int rounded_Value=std::round(ontime);
                             nOfftime=120-static_cast<int>(ontime);
                             handler->burst_off_length(nOfftime);
                             nMultiBurstCount = ui->lineEdit_79->text().toInt();
                             handler->burst_length(nMultiBurstCount);

                             }




                     }
                     float progress4 = ((range - static_cast<float>(nfpone + nfptwo + nfpzero)) /
                                        (static_cast<float>(nfpzero+nfpone+nfptwo+nfpthree) - static_cast<float>(nfpone + nfptwo + nfpzero))) * pow2;
                     int rounded_value=std::round(progress4);
                      handler->phaco_power(rounded_value);
                    // qDebug()<<"power id deliverd from the us2 is"<<progress4;
                     ui->label_92->setText(QString::number(rounded_value));

                     if (!elapsedTimer->isValid()) {
                         elapsedTimer->start();  // Start the timer only if it's not already running
                        // qDebug() << "Timer started!";
                     }


                     // Call stopPhacoTracking();the updateTimes function to update the displayed times
                     updateTimes(progress4, ui->elapsed_time, ui->elapsed_time_2);

                     if (progress4 == pow2) {
                        // speedofthelabe(ui->label_7);
                     }

                 }else {  // If pushButton is OFF
                    stopPhacoTracking();
                     handler->phaco_off();
                    handler->freq_count(0);
                   // handler->fs_count(0);
                    handler->phaco_power(0);
                 }
                }
                if (vus1 == "Surgeon") {
                    const int MIN_RANGE = nfpzero + nfpone + nfptwo;
                    const int MAX_RANGE = nfpzero + nfpone + nfptwo + nfpthree;
                    int divi = MAX_RANGE - MIN_RANGE;
                    int calibration = range - MIN_RANGE;

                    if (divi != 0) {
                        double final = static_cast<double>(us2vacline) / static_cast<double>(divi);
                        int presetvac = static_cast<int>(std::round(calibration * final));
                        int pro = readsensorvalue();
                        ui->label_93->setText(QString::number(pro));
                        if(speakeronoff == "Speaker ON"){
                        handler->speaker_on(presetvac,1,0,0);
                        }else{
                            handler->speaker_off();
                        }
                        if(us2powmode == "Ocuburst"){
                            handler->pdm_mode(CONTINOUS);
                        }else if(us2powmode == "Ocupulse" ){
                            handler->pdm_mode(CONTINOUS);
                        }
                        if (pro < presetvac) {
                            motoron(ui->lineEdit_59);
                            if (!motorus2) {
                                motorus2 = true;
                            }
                        } else if (motorus2) {
                            motoroff();
                            motorus2 = false;
                        }

                        if (pro >= us2vacline) {
                            pro = static_cast<int>(us2vacline);
                            ui->label_93->setText(QString::number(pro));
                            if(speakeronoff == "Speaker ON"){
                            handler->speaker_on(pro,0,0,1);
                            }else{
                                handler->speaker_off();
                            }
                            if(us2powmode == "Ocuburst"){
                                handler->pdm_mode(SINGLE_BURST);
                            }else if(us2powmode == "Ocupulse" ){
                                handler->pdm_mode(PULSE_MODE);
                            }
                            motoroff();
                        }
                    }
                }
               beepsound(3);
stopPhacoTracking();

            }

        }


        break;
    }

//ULTRASONICBUT 3
        //us 3

    case 3: {
        //they will not run
        int pow3 = ui->lineEdit_61->text().toInt(); // Get the power value from the line edit
        int us3vacline=ui->lineEdit_63->text().toInt();
        QString text=ui->us3onoff->text();
        bool flag1 = false;
        QString us3powmode=ui->CutMode_vitCom_3->currentText();
         int nOfftime;
      int us3reachedvac;
      // qDebug()<<"the us3 pdm is"<<us3powmode;
        if (us3 == "Panel"||vus3=="Panel"){
            if (range>0 && range<nfpzero) {
                ui->pushButton_42->setText("0");
                handler->speaker_off();
                ui->dial_2->setValue(range);
                    handler->freq_count(0);
                    stopPhacoTracking();
                    handler->phaco_off();
                    handler->phaco_power(0);
                    handler->convert_dac(0x30, 0);
                        handler->fs_count_limit(nfpzero+nfpone+nfptwo);

                     us3poweron=false;
                       if(!overallci){
                           ui->CI5_5->setStyleSheet(styleSheet4);
                           handler->pinchvalve_off();
                    }

                       if(ventonus3==false){
                        //   handler->pinchvalve_on();
                 handler->safetyvent_on();
                     QThread::msleep(100);
                     handler->safetyvent_off();
                     ventonus2=true;
                       }
                       int pro=readsensorvalue();
                       ui->label_99->setText(QString::number(pro));
                   motoroff();

                ui->label_98->setText("0");

                beepsound(0);

                flag1 = true; // Reset flag
}
            else if (range >= nfpzero & range < nfpzero+nfpone) {
                     ui->pushButton_42->setText("1");
                     ui->dial_2->setValue(range);
                     handler->pinchvalve_on();
                     if(ventonus3==false){
                     handler->safetyvent_on();
                   QThread::msleep(100);
                   handler->safetyvent_off();
                   ventonus3=true;
                     }

                  int pro=readsensorvalue();
                  ui->label_99->setText(QString::number(pro));
                  ui->CI5_5->setStyleSheet(styleSheet3);
                  //footpedalbeep();
                  motoroff();
                  ui->label_98->setText("0");
                  if(speakeronoff == "Speaker ON"){
                  handler->speaker_on(0,0,1,0);
                  }else{
                      handler->speaker_off();
                  }
                  handler->freq_count(0);
                  stopPhacoTracking();
                  handler->phaco_off();
                //  handler->fs_count(0);
                 us3poweron=false;
                 beepsound(1);

flag1 = true;
            }
            else if (range >=(nfpone+nfpzero) && range < (nfpzero+nfpone+nfptwo)) {
                ui->pushButton_42->setText("2");
               // qDebug()<<"the postion is 2nd";
                ui->label_99->show();
                ventonus3=false;
                ui->dial_2->setValue(range);
                ui->label_7->setText("0");

                ui->CI5_5->setStyleSheet(styleSheet3);
                handler->pinchvalve_on();

 if(vus3=="Panel" ){
     int nonlinear_prevac=readsensorvalue();
     if(nonlinear_prevac <=  us3vacline){
       ui->label_99->setText(QString::number(nonlinear_prevac));
      motoron(ui->lineEdit_62);
      if(speakeronoff == "Speaker ON"){
   handler->speaker_on(nonlinear_prevac,1,0,0);
      }else{
          handler->speaker_off();
      }
     }
   if (nonlinear_prevac >  us3vacline) {
                    motoroff(); // Turn off the motor
                    ui->label_99->setText(QString::number(us3vacline));
                    if(speakeronoff == "Speaker ON"){
                 handler->speaker_on(nonlinear_prevac,0,0,1);
                    }else{
                        handler->speaker_off();
                    }

                }
   if(us3reachedvac == us3vacline){
       motoroff();
   }
 }
 ui->label_98->setText("0");
                    handler->freq_count(0);
                    stopPhacoTracking();
                    handler->phaco_off();
                   // handler->fs_count(0);
                    us3poweron= false;
                    beepsound(2);


                flag1 = true; // Reset flag
            }
            else if (range >=(nfpzero+nfpone+nfptwo) && range < (nfpzero+nfpone+nfptwo+nfpthree)) {
                ui->pushButton_42->setText("3");
                 ui->dial_2->setValue(range);
                ui->label_98->show();
                ui->label_99->show();
                ventonus2=false;
                handler->pinchvalve_on();
                ui->CI5_5->setStyleSheet(styleSheet3);
if(vus3=="Panel"){
     int nonlinear_prevac=readsensorvalue();
     if(nonlinear_prevac <=us3vacline){
                              ui->label_99->setText(QString::number(nonlinear_prevac));
                              motoron(ui->lineEdit_62);
                              if(us3powmode == "Ocuburst"){
                                  handler->pdm_mode(CONTINOUS);
                              }else if(us3powmode == "Ocupulse" ){
                                  handler->pdm_mode(CONTINOUS);
                              }
                              if(speakeronoff == "Speaker ON"){
                           handler->speaker_on(nonlinear_prevac,1,0,0);
                              }else{
                                  handler->speaker_off();
                              }
     }
                                  if (nonlinear_prevac > us3vacline) {
                                      motoroff(); // Turn off the motor
                                      ui->label_99->setText(QString::number(us3vacline));

                                      if(speakeronoff == "Speaker ON"){
                                      handler->speaker_on(0,0,0,1);
                                      }else{
                                          handler->speaker_off();
                                      }
                                      if(us3powmode == "Ocuburst"){
                                          handler->pdm_mode(SINGLE_BURST);
                                          //handler->phaco_on(range);
                                          startPhacoTracking(pow3);      // pow1 is already a 0–1 fraction in that block
                                              handler->phaco_on(range);
                                              handler->freq_count(nFreqCount);
                                          handler->phaco_power(100);
                                          //handler->freq_count(nFreqCount);
                                          // Retrieve the burst length from the UI
                                         nOcuBurstCount = ui->lineEdit_76->text().toInt();
                                          handler->burst_length(  nOcuBurstCount ); // Set burst length

                                      }else if(us3powmode == "Ocupulse" ){
                                          handler->pdm_mode(PULSE_MODE);
                                        nOcuPulseCount = ui->lineEdit_77->text().toInt();
                                          handler->pulse_count(nOcuPulseCount);
                                      }
                                      us3reachedvac=us3vacline;
                                  }

}
if(us3 == "Panel"){
               if(!us3poweron && text == "ON" ){
                     //  handler->fs_count(range);
                   startPhacoTracking(pow3);      // pow1 is already a 0–1 fraction in that block
                       handler->phaco_on(range);
                       handler->freq_count(nFreqCount);
                       if(us3powmode == "Multi burst"){
                                   if(range>(nfpzero+nfpone+nfptwo)){
                                   double upper=static_cast<int>(range)-static_cast<int>(nfpzero+nfpone+nfptwo);
                                   double lower=static_cast<int>(nfpzero+nfpone+nfptwo+nfpthree)-static_cast<int>(nfpzero+nfpone+nfptwo);
                               double intialontime=static_cast<double>(upper)/static_cast<double>(lower);
                               double ontime=static_cast<double>(intialontime)*120;
                               int rounded_Value=std::round(ontime);
                               nOfftime=120-static_cast<int>(ontime);
                              // qDebug()<<"the off time of burst is"<<nOfftime;
                               handler->burst_off_length(nOfftime);
                               nMultiBurstCount = ui->lineEdit_79->text().toInt();
                               handler->burst_length(nMultiBurstCount);
                               }

                       }
                       ui->label_98->setText(QString::number(pow3));
                       handler->phaco_power(pow3);
                       if (!elapsedTimer->isValid()) {
                           elapsedTimer->start();
                           qDebug() << "Timer started!";
                       }
                       updateTimes(pow3,ui->elapsed_time,ui->elapsed_time_2);
}
}
beepsound(3);


            }

  }
        if ((us3 == "Surgeon")||( vus3 == "Surgeon")) {
            if (range > 0 && range < nfpzero) {
                ui->pushButton_42->setText("0");
                ui->dial_2->setValue(range);
                if (!overallci) {
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

                stopPhacoTracking();
                handler->phaco_off();
                 handler->phaco_power(0);
                 handler->convert_dac(0x30, 0);
                 handler->freq_count(0);
                     handler->fs_count_limit(nfpzero+nfpone+nfptwo);
                     int pro=readsensorvalue();
                     ui->label_99->setText(QString::number(pro));
                ui->label_98->setText("0");
                handler->speaker_off();
                beepsound(0);

                us3poweron=false;
                flag1 = true; // Reset flag

            } else if (range >= nfpzero && range < (nfpzero + nfpone)) {
                // Position 1 behavior
                ui->pushButton_42->setText("1");
                //footpedalbeep();
                ui->dial_2->setValue(range);
                ui->label_7->setText("0");

                handler->pinchvalve_on();

                if(ventonus3==false){
                    handler->safetyvent_on();
              QThread::msleep(100);
             handler->safetyvent_off();
              ventonus3=true;
                }

                ui->CI5_5->setStyleSheet(styleSheet3);
                motoroff();
                  int pro=readsensorvalue();
                  ui->label_99->setText(QString::number(pro));
                  ui->label_98->setText("0");
                  stopPhacoTracking();
                handler->phaco_off();
                handler->phaco_power(0);
                handler->freq_count(0);
               // handler->fs_count(0);
                if(speakeronoff == "Speaker ON"){
                handler->speaker_on(0,0,1,0);
                }else{
                    handler->speaker_off();
                }
                us3poweron=false;

                beepsound(1);

                flag1 = true; // Reset flag

            } else if (range >= (nfpzero + nfpone) && range < (nfpzero + nfpone + nfptwo)) {
                // Position 2 behavior
                ui->pushButton_42->setText("2");
                //footpedalbeep();
                ui->dial_2->setValue(range);
                ui->label_7->setText("0");

                ventonus3 = false;

                handler->pinchvalve_on();
               // handler->safetyvent_off();
                ui->CI5_5->setStyleSheet(styleSheet3);
                if (vus3 == "Surgeon") {
                    const int MIN_RANGE = nfpzero + nfpone;
                    const int MAX_RANGE = nfpzero + nfpone + nfptwo;
                    int divi = MAX_RANGE - MIN_RANGE;  // Divider
                    int calibration = range - MIN_RANGE;  // Calibration value

                    if (divi != 0) {
                        double final = static_cast<double>(us3vacline) / static_cast<double>(divi);
                        int presetvac = static_cast<int>(std::round(calibration * final));
                        int pro = readsensorvalue();
                        ui->label_99->setText(QString::number(pro));
                        if(speakeronoff == "Speaker ON"){
                        handler->speaker_on(pro,1,0,0);
                        }else{
                            handler->speaker_off();
                        }

                        // Motor control based on preset value
                        if (pro < presetvac) {
                            motoron(ui->lineEdit_62);

                            if (!motorus3) {
                                motorus3= true;
                            }
                        } else if (motorus3) {
                            motoroff();
                            motorus3= false;
                        }

                        if (pro >= us3vacline) {
                            pro = static_cast<int>(us3vacline);
                            ui->label_99->setText(QString::number(pro));
                            motoroff();
                            if(speakeronoff == "Speaker ON"){
                            handler->speaker_on(0,0,0,1);
                            }else{
                                handler->speaker_off();
                            }
                        }
                    }

                }


                stopPhacoTracking();
                handler->phaco_off();
                handler->phaco_power(0);
                handler->freq_count(0);
                //handler->fs_count(0);
                us2poweron=false;
                beepsound(2);

                flag1 = true; // Reset flag

            } else if (range >= (nfpzero + nfpone + nfptwo) && range < (nfpzero + nfpone + nfptwo + nfpthree)) {
                ui->pushButton_42->setText("3");
                ui->dial_2->setValue(range);
                ventonus3 = false;
                handler->pinchvalve_on();
                ui->CI5_5->setStyleSheet(styleSheet3);
                if(us3 == "Surgeon"){
                 if (!us3poweron && text == "ON") {
                   //  qDebug()<<"the power is "<<ui->us2onoff->text();
                    // us2poweron=true;
                     startPhacoTracking(pow3);      // pow1 is already a 0–1 fraction in that block
                         handler->phaco_on(range);
                         handler->freq_count(nFreqCount);
                  //   handler->fs_count(range);

                     if(us3powmode == "Multi burst"){
                               if(range>(nfpzero+nfpone+nfptwo)){

                                 double upper=static_cast<int>(range)-static_cast<int>(nfpzero+nfpone+nfptwo);
                                 double lower=static_cast<int>(nfpzero+nfpone+nfptwo+nfpthree)-static_cast<int>(nfpzero+nfpone+nfptwo);
                             double intialontime=static_cast<double>(upper)/static_cast<double>(lower);
                             double ontime=static_cast<double>(intialontime)*120;
                             int rounded_Value=std::round(ontime);
                             nOfftime=120-static_cast<int>(ontime);
                             handler->burst_off_length(nOfftime);
                             nMultiBurstCount = ui->lineEdit_79->text().toInt();
                             handler->burst_length(nMultiBurstCount);
                             }




                     }

                     float progress4 = ((range - static_cast<float>(nfpone + nfptwo + nfpzero)) /
                                        (static_cast<float>(nfpzero+nfpone+nfptwo+nfpthree) - static_cast<float>(nfpone + nfptwo + nfpzero))) * pow3;
                     int rounded_value=std::round(progress4);
                      handler->phaco_power(rounded_value);
                    // qDebug()<<"power id deliverd from the us2 is"<<progress4;
                     ui->label_98->setText(QString::number(rounded_value));
                     if (!elapsedTimer->isValid()) {
                         elapsedTimer->start();
                         qDebug() << "Timer started!";
                     }
                     updateTimes(pow3,ui->elapsed_time,ui->elapsed_time_2);
                     if (progress4 == pow3) {
                        // speedofthelabe(ui->label_7);
                     }

                 }else {  // If pushButton is OFF
                    stopPhacoTracking();
                     handler->phaco_off();
                    handler->freq_count(0);
                   // handler->fs_count(0);
                    handler->phaco_power(0);
                 }
                }
                if (vus3 == "Surgeon") {
                    const int MIN_RANGE = nfpzero + nfpone + nfptwo;
                    const int MAX_RANGE = nfpzero + nfpone + nfptwo + nfpthree;
                    int divi = MAX_RANGE - MIN_RANGE;
                    int calibration = range - MIN_RANGE;

                    if (divi != 0) {
                        double final = static_cast<double>(us3vacline) / static_cast<double>(divi);
                        int presetvac = static_cast<int>(std::round(calibration * final));
                        int pro = readsensorvalue();
                        ui->label_99->setText(QString::number(pro));
                        if(speakeronoff == "Speaker ON"){
                        handler->speaker_on(presetvac,1,0,0);
                        }else{
                            handler->speaker_off();
                        }
                        if(us3powmode == "Ocuburst"){
                            handler->pdm_mode(CONTINOUS);
                        }else if(us3powmode == "Ocupulse" ){
                            handler->pdm_mode(CONTINOUS);
                        }
                        if (pro < presetvac) {
                            motoron(ui->lineEdit_62);
                            if (!motorus3) {
                                motorus3 = true;
                            }
                        } else if (motorus3) {
                            motoroff();
                            motorus3 = false;
                        }

                        if (pro >= us3vacline) {
                            pro = static_cast<int>(us3vacline);
                            ui->label_99->setText(QString::number(pro));
                            if(speakeronoff == "Speaker ON"){
                            handler->speaker_on(pro,0,0,1);
                            }else{
                                handler->speaker_off();
                            }
                            if(us3powmode == "Ocuburst"){
                                handler->pdm_mode(SINGLE_BURST);
                            }else if(us3powmode == "Ocupulse" ){
                                handler->pdm_mode(PULSE_MODE);
                            }
                            motoroff();
                        }
                    }
                }
               beepsound(3);



            }

        }


        break;
    }


        // us4
    case 4: {
        //they will not run
        int pow4 = ui->lineEdit_64->text().toInt(); // Get the power value from the line edit
        int us4vacline=ui->lineEdit_66->text().toInt();
        QString text=ui->us4onoff->text();
        bool flag1 = false;
        QString us4powmode=ui->CutMode_vitCom_4->currentText();
         int nOfftime;
         int us4reachedvac;

       //qDebug()<<"the us4 pdm is"<<us4powmode;
        if (us4 == "Panel"||vus4=="Panel"){
            if (range>0 && range<nfpzero) {
                ui->pushButton_42->setText("0");
                handler->speaker_off();
                ui->dial_2->setValue(range);
                ui->label_105->setText("0");

                    handler->freq_count(0);
                    stopPhacoTracking();
                    handler->phaco_off();
                    handler->phaco_power(0);
                    handler->convert_dac(0x30, 0);
                        handler->fs_count_limit(nfpzero+nfpone+nfptwo);

                     us4poweron=false;
                       if(!overallci){
                           ui->CI5_5->setStyleSheet(styleSheet4);
                           handler->pinchvalve_off();
                    }
//qDebug()<<"the power value at us4 is"<<pow4;
                       if(ventonus4==false){
                        //   handler->pinchvalve_on();
                 handler->safetyvent_on();
                     QThread::msleep(100);
                       handler->pinchvalve_off();
                     handler->safetyvent_off();
                     ventonus4=true;
                       }
                       int pro=readsensorvalue();
                       ui->label_104->setText(QString::number(pro));
                   motoroff();


                   beepsound(0);

                flag1 = true; // Reset flag
}
            else if (range >= nfpzero & range < nfpzero+nfpone) {
                     ui->pushButton_42->setText("1");
                     ui->dial_2->setValue(range);
                     handler->pinchvalve_on();
                     if(ventonus4==false){
                     handler->safetyvent_on();
                   QThread::msleep(100);
                   handler->safetyvent_off();
                   ventonus4=true;
                     }

                  int pro=readsensorvalue();
                  ui->label_104->setText(QString::number(pro));
                  ui->CI5_5->setStyleSheet(styleSheet3);
                  //footpedalbeep();
                  motoroff();
                  ui->label_105->setText("0");
                  if(speakeronoff == "Speaker ON"){
                  handler->speaker_on(0,0,1,0);
                  }else{
                      handler->speaker_off();
                  }                  handler->freq_count(0);
                  stopPhacoTracking();
                  handler->phaco_off();
                 // handler->fs_count(0);
                 us4poweron=false;
                 beepsound(1);

flag1 = true;
            }
            else if (range >=(nfpone+nfpzero) && range < (nfpzero+nfpone+nfptwo)) {
                ui->pushButton_42->setText("2");
               // qDebug()<<"the postion is 2nd";
                ui->label_104->show();
                ventonus4=false;
                ui->dial_2->setValue(range);
                ui->CI5_5->setStyleSheet(styleSheet3);
                handler->pinchvalve_on();

 if(vus4=="Panel" ){
     int nonlinear_prevac=readsensorvalue();
     if(nonlinear_prevac <=  us4vacline){
       ui->label_104->setText(QString::number(nonlinear_prevac));
      motoron(ui->lineEdit_65);
      if(speakeronoff =="Speaker ON"){
   handler->speaker_on(nonlinear_prevac,1,0,0);
      }else{
          handler->speaker_off();
      }
     }
   if (nonlinear_prevac >  us4vacline) {
                    motoroff(); // Turn off the motor
                    ui->label_104->setText(QString::number(us4vacline));
                    if(speakeronoff == "Speaker ON"){
                   handler->speaker_on(us4vacline,0,0,1);
                    }else{
                        handler->speaker_off();
                    }
                }
   if(us4reachedvac == us4vacline){
       motoroff();
   }
 }
 ui->label_105->setText("0");
                    handler->freq_count(0);
                    stopPhacoTracking();
                    handler->phaco_off();
                  //  handler->fs_count(0);
                    us4poweron= false;
                    beepsound(2);


                flag1 = true; // Reset flag
            }
            else if (range >=(nfpzero+nfpone+nfptwo) && range < (nfpzero+nfpone+nfptwo+nfpthree)) {
                ui->pushButton_42->setText("3");
                 ui->dial_2->setValue(range);
                ui->label_105->show();
                ui->label_104->show();
                ventonus4=false;
                handler->pinchvalve_on();
                ui->CI5_5->setStyleSheet(styleSheet3);
if(vus4=="Panel"){
     int nonlinear_prevac=readsensorvalue();
     if(nonlinear_prevac<=us4vacline){
                              ui->label_104->setText(QString::number(nonlinear_prevac));
                              motoron(ui->lineEdit_65);
                              if(us4powmode == "Ocuburst"){
                                  handler->pdm_mode(CONTINOUS);
                              }else if(us4powmode == "Ocupulse" ){
                                  handler->pdm_mode(CONTINOUS);
                              }
                              if(speakeronoff == "Speaker ON"){
                           handler->speaker_on(nonlinear_prevac,1,0,0);
                              }else{
                                  handler->speaker_off();
                              }
     }

                                  if (nonlinear_prevac > us4vacline) {
                                      motoroff(); // Turn off the motor
                                      ui->label_104->setText(QString::number(us4vacline));
                                      if(speakeronoff == "Speaker ON"){
                                      handler->speaker_on(us4vacline,0,0,1);
                                      }else{
                                          handler->speaker_off();
                                      }
                                      if(us4powmode == "Ocuburst"){
                                          handler->pdm_mode(SINGLE_BURST);
                                          startPhacoTracking(pow4);      // pow1 is already a 0–1 fraction in that block
                                              handler->phaco_on(range);
                                              handler->freq_count(nFreqCount);
                                          //handler->phaco_on(range);
                                          handler->phaco_power(100);
                                          //handler->freq_count(nFreqCount);
                                          // Retrieve the burst length from the UI
                                         nOcuBurstCount = ui->lineEdit_76->text().toInt();
                                          handler->burst_length(  nOcuBurstCount ); // Set burst length

                                      }else if(us4powmode == "Ocupulse" ){
                                          handler->pdm_mode(PULSE_MODE);
                                        nOcuPulseCount = ui->lineEdit_77->text().toInt();
                                          handler->pulse_count(nOcuPulseCount);
                                      }
                                  }
                                  us4reachedvac=us4vacline;
                              }
if(us4 == "Panel"){
               if(!us4poweron && text == "ON" ){
                     //  handler->fs_count(range);
                   startPhacoTracking(pow4);      // pow1 is already a 0–1 fraction in that block
                       handler->phaco_on(range);
                       handler->freq_count(nFreqCount);
                       if(us4powmode == "Multi burst"){
                               if(range>(nfpzero+nfpone+nfptwo)){

                                   double upper=static_cast<int>(range)-static_cast<int>(nfpzero+nfpone+nfptwo);
                                   double lower=static_cast<int>(nfpzero+nfpone+nfptwo+nfpthree)-static_cast<int>(nfpzero+nfpone+nfptwo);
                               double intialontime=static_cast<double>(upper)/static_cast<double>(lower);
                               double ontime=static_cast<double>(intialontime)*120;
                               int rounded_Value=std::round(ontime);
                               nOfftime=120-static_cast<int>(ontime);
                               handler->burst_off_length(nOfftime);
                               nMultiBurstCount = ui->lineEdit_79->text().toInt();
                               handler->burst_length(nMultiBurstCount);
                               }




                       }

                       if (!elapsedTimer->isValid()) {
                           elapsedTimer->start();
                           qDebug() << "Timer started!";
                       }
                       updateTimes(pow4,ui->elapsed_time,ui->elapsed_time_2);
                     ui->label_105->setText(QString::number(pow4));
                       handler->phaco_power(pow4);

}
               beepsound(3);


            }

}

  }
        if ((us4== "Surgeon")||( vus4 == "Surgeon")) {
            if (range > 0 && range < nfpzero) {
                ui->pushButton_42->setText("0");
                ui->dial_2->setValue(range);
                if (!overallci) {
                    ui->CI5_5->setStyleSheet(styleSheet4);
                    handler->pinchvalve_off();
                }
                if(ventonus4==false){
                   handler->safetyvent_on();
              QThread::msleep(100);
              handler->safetyvent_off();
              ventonus4=true;
                }
                motoroff();

                stopPhacoTracking();
                handler->phaco_off();
                 handler->phaco_power(0);
                 handler->convert_dac(0x30, 0);
                 handler->freq_count(0);
                     handler->fs_count_limit(nfpzero+nfpone+nfptwo);

                     int pro=readsensorvalue();
                     ui->label_104->setText(QString::number(pro));
                     ui->label_105->setText("0");
                handler->speaker_off();
                beepsound(0);

                us4poweron=false;
                flag1 = true; // Reset flag

            } else if (range >= nfpzero && range < (nfpzero + nfpone)) {
                // Position 1 behavior
                ui->pushButton_42->setText("1");
                //footpedalbeep();
                ui->dial_2->setValue(range);
                ui->label_7->setText("0");

                handler->pinchvalve_on();

                if(ventonus4==false){
                    handler->safetyvent_on();
              QThread::msleep(100);
             handler->safetyvent_off();
              ventonus4=true;
                }

                ui->CI5_5->setStyleSheet(styleSheet3);
                motoroff();
                  int pro=readsensorvalue();
                  ui->label_104->setText(QString::number(pro));
                  ui->label_105->setText("0");
                  stopPhacoTracking();
                handler->phaco_off();
                handler->phaco_power(0);
                handler->freq_count(0);
              //  handler->fs_count(0);
                if(speakeronoff == "Speaker ON"){
                handler->speaker_on(0,0,1,0);
                }else{
                    handler->speaker_off();
                }                us4poweron=false;

                beepsound(1);

                flag1 = true; // Reset flag

            } else if (range >= (nfpzero + nfpone) && range < (nfpzero + nfpone + nfptwo)) {
                // Position 2 behavior
                ui->pushButton_42->setText("2");
                //footpedalbeep();
                ui->dial_2->setValue(range);
                ui->label_7->setText("0");

                ventonus4 = false;

                handler->pinchvalve_on();
               // handler->safetyvent_off();
                ui->CI5_5->setStyleSheet(styleSheet3);
                if (vus4 == "Surgeon") {
                    const int MIN_RANGE = nfpzero + nfpone;
                    const int MAX_RANGE = nfpzero + nfpone + nfptwo;
                    int divi = MAX_RANGE - MIN_RANGE;  // Divider
                    int calibration = range - MIN_RANGE;  // Calibration value

                    if (divi != 0) {
                        double final = static_cast<double>(us4vacline) / static_cast<double>(divi);
                        int presetvac = static_cast<int>(std::round(calibration * final));
                        int pro = readsensorvalue();
                        ui->label_104->setText(QString::number(pro));
                        if(speakeronoff == "Speaker ON"){
                        handler->speaker_on(pro,1,0,0);
                        }else{
                            handler->speaker_off();
                        }

                        // Motor control based on preset value
                        if (pro < presetvac) {
                            motoron(ui->lineEdit_65);

                            if (!motorus4) {
                                motorus4= true;
                            }
                        } else if (motorus4) {
                            motoroff();
                            motorus4= false;
                        }

                        // If pro exceeds vitpreset, cap it
                        if (pro >= us4vacline) {
                            pro = static_cast<int>(us4vacline);
                            ui->label_104->setText(QString::number(pro));
                            motoroff();
                            if(speakeronoff == "Speaker ON"){
                            handler->speaker_on(0,0,0,1);
                            }else{
                                handler->speaker_off();
                            }
                        }
                    }

                }


                stopPhacoTracking();
                handler->phaco_off();
                handler->phaco_power(0);
                handler->freq_count(0);
             //   handler->fs_count(0);
                us4poweron=false;
                beepsound(2);

                flag1 = true; // Reset flag

            } else if (range >= (nfpzero + nfpone + nfptwo) && range < (nfpzero + nfpone + nfptwo + nfpthree)) {
                ui->pushButton_42->setText("3");
                ui->dial_2->setValue(range);
                ventonus3 = false;
                handler->pinchvalve_on();
                ui->CI5_5->setStyleSheet(styleSheet3);
                if(ui->us4mode->text() == "Surgeon"){
                 if (!us4poweron && text == "ON") {
                     startPhacoTracking(pow4);      // pow1 is already a 0–1 fraction in that block
                         handler->phaco_on(range);
                         handler->freq_count(nFreqCount);
                     if(us4powmode == "Multi burst"){
                             if(range>(nfpzero+nfpone+nfptwo)){

                                 double upper=static_cast<int>(range)-static_cast<int>(nfpzero+nfpone+nfptwo);
                                 double lower=static_cast<int>(nfpzero+nfpone+nfptwo+nfpthree)-static_cast<int>(nfpzero+nfpone+nfptwo);
                             double intialontime=static_cast<double>(upper)/static_cast<double>(lower);
                             double ontime=static_cast<double>(intialontime)*120;
                             int rounded_Value=std::round(ontime);
                             nOfftime=120-static_cast<int>(ontime);
                             handler->burst_off_length(nOfftime);
                             nMultiBurstCount = ui->lineEdit_79->text().toInt();
                             handler->burst_length(nMultiBurstCount);
                             }
                     }

                     float progress4 = ((range - static_cast<float>(nfpone + nfptwo + nfpzero)) /
                                        (static_cast<float>(nfpzero+nfpone+nfptwo+nfpthree) - static_cast<float>(nfpone + nfptwo + nfpzero))) * pow4;
                     int rounded_value=std::round(progress4);
                      handler->phaco_power(rounded_value);
                      ui->label_105->setText(QString::number(rounded_value));
                     if (!elapsedTimer->isValid()) {
                         elapsedTimer->start();
                         qDebug() << "Timer started!";
                     }
                     updateTimes(pow4,ui->elapsed_time,ui->elapsed_time_2);
                     if (progress4 == pow4) {
                     }

                 }else {  // If pushButton is OFF
                    stopPhacoTracking();
                     handler->phaco_off();
                    handler->freq_count(0);
                   // handler->fs_count(0);
                    handler->phaco_power(0);
                 }
                }
                if (vus1 == "Surgeon") {
                    const int MIN_RANGE = nfpzero + nfpone + nfptwo;
                    const int MAX_RANGE = nfpzero + nfpone + nfptwo + nfpthree;
                    int divi = MAX_RANGE - MIN_RANGE;
                    int calibration = range - MIN_RANGE;

                    if (divi != 0) {
                        double final = static_cast<double>(us4vacline) / static_cast<double>(divi);
                        int presetvac = static_cast<int>(std::round(calibration * final));
                        int pro = readsensorvalue();
                        ui->label_104->setText(QString::number(pro));
                        if(speakeronoff == "Speaker ON"){
                        handler->speaker_on(presetvac,1,0,0);
                        }else{
                            handler->speaker_off();
                        }
                        if(us4powmode == "Ocuburst"){
                            handler->pdm_mode(CONTINOUS);
                        }else if(us4powmode == "Ocupulse" ){
                            handler->pdm_mode(CONTINOUS);
                        }
                        if (pro < presetvac) {
                            motoron(ui->lineEdit_65);
                            if (!motorus4) {
                                motorus4 = true;
                            }
                        } else if (motorus4) {
                            motoroff();
                            motorus4 = false;
                        }

                        if (pro >= us4vacline) {
                            pro = static_cast<int>(us4vacline);
                            ui->label_104->setText(QString::number(pro));
                            if(speakeronoff == "Speaker ON"){
                            handler->speaker_on(pro,0,0,1);
                            }else{
                                handler->speaker_off();
                            }
                            if(us4powmode == "Ocuburst"){
                                handler->pdm_mode(SINGLE_BURST);
                            }else if(us4powmode == "Ocupulse" ){
                                handler->pdm_mode(PULSE_MODE);
                            }
                            motoroff();
                        }
                    }
                }


               beepsound(3);


            }

        }


        break;
    }        // ia1
        case 5:
    {
bool flag6=false;
double ia1preset=ui->lineEdit_70->text().toDouble();
QString ia1=ui->ia2mode->text();
stopPhacoTracking();
handler->phaco_off();
 handler->phaco_power(0);
        if (ia1 == "Panel") { // surgeon
            if (range >= 0 && range < nfpzero) {
                ui->pushButton_42->setText("0");
                ui->dial_2->setValue(range);
                handler->vibrator_off();

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
                ui->label_113->setText(QString::number(0));
                flag6=true;
            }

            else if (range >= nfpzero && range < (nfpone + nfpzero)) {
                ui->pushButton_42->setText("1");
                if(speakeronoff == "Speaker ON"){
                handler->speaker_on(0,0,1,0);
                }else{
                    handler->speaker_off();
                }

                ui->dial_2->setValue(range);

                handler->pinchvalve_on();
                ui->CI5_5->setStyleSheet(styleSheet3);
                if(ventonia1 == false) {
                    handler->safetyvent_on();
                    QThread::msleep(200);
                    handler->safetyvent_off();
                    ventonia1 = true;
                }
                motoroff();
                int pro = readsensorvalue();
                ui->label_113->setText(QString::number(pro));
                beepsound(1);
                 flag6=true;
            }

            else if (range >= (nfpzero + nfpone) && range < (nfpzero + nfpone + nfptwo + nfpthree)) {
                ui->pushButton_42->setText("2");

                ventonia1 = false;
                handler->safetyvent_off();
                ui->dial_2->setValue(range);
                ui->CI5_5->setStyleSheet(styleSheet3);
                handler->pinchvalve_on();
               int nonlinear_prevac = readsensorvalue(); // Assuming this function reads the current sensor value
               int nonlinear_vac = std::min(nonlinear_prevac, static_cast<int>(ia1preset));
               ui->label_113->setText(QString::number(nonlinear_vac));
               motoron(ui->lineEdit_69);
               if(speakeronoff == "Speaker ON"){
               handler->speaker_on(nonlinear_prevac,1,0,0);
               }else{
                   handler->speaker_off();
               }
               if (nonlinear_prevac >= ia1preset) {
                         motoroff(); // Turn off the motor
                       //  speedofthelabe(ui->label_113);
                         if(speakeronoff == "Speaker ON"){
                         handler->speaker_on(0,0,0,1);
                         }else{
                             handler->speaker_off();
                         }

                    }
               beepsound(2);
           }
        }

        else if(ia1 == "Surgeon"){
            if(range>=0 && range<nfpzero){
                ui->pushButton_42->setText("0");
                  ui->dial_2->setValue(range);
                  handler->vibrator_off();
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
                    //int pro = readsensorvalue();
                 ui->label_113->setText(QString::number(0));
                 handler->speaker_off();
                  flag6=true;


            }
            else if(range>=(nfpzero) && range<(nfpone+nfpzero)){
                ui->pushButton_42->setText("1");

                if(speakeronoff == "Speaker ON"){
                handler->speaker_on(0,0,1,0);
                }else{
                    handler->speaker_off();
                }
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
               beepsound(1);
             flag6=true;
}
            if (range >= (nfpzero + nfpone) && range < (nfpzero + nfpone + nfptwo + nfpthree)) {
                ui->pushButton_42->setText("2");
                ventonia1=false;
                beepsound(2);
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
                    ui->label_113->setText(QString::number(pro));
                  //  const int tolerance = 5; // Example tolerance

                    if (pro < (presetvac)) {
                        motoron(ui->lineEdit_69);
                        if(speakeronoff == "Speaker ON"){
                        handler->speaker_on(pro,1,0,0);
                        }else{
                            handler->speaker_off();
                        }
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
                      //  speedofthelabe(ui->label_113);
                        motoroff();

                        if(speakeronoff == "Speaker ON"){
                        handler->speaker_on(0,0,0,1);
                        }else{
                            handler->speaker_off();
                        }
                    }

            }
               // ia1currentcount=2;

}
        }

    break;
    }
        // ia2
        case 6:
        {
  bool flag6=true;
  ia2=ui->ia1mode->text();
  double ia2vacline=ui->lineEdit_68->text().toInt();

  stopPhacoTracking();
  handler->phaco_off();
  handler->phaco_power(0);
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
             //   int ia2pro=readsensorvalue();
                ui->label_109->setText(QString::number(0));
 motoroff();
  handler->speaker_off();
  beepsound(0);
 flag6=true;
            }
            if(range>=nfpzero && range<(nfpzero+nfpone)){
                ui->pushButton_42->setText("1");
                if(speakeronoff == "Speaker ON"){
                handler->speaker_on(0,0,1,0);
                }else{
                    handler->speaker_off();
                }
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
            beepsound(1);
}
            else if(range>=(nfpzero+nfpone) && range <(nfpzero+nfpone+nfptwo+nfpthree)){
                ui->pushButton_42->setText("2");
                ventonia2=false;
                //footpedalbeep();
                ui->dial_2->setValue(range);
                ui->CI5_5->setStyleSheet(styleSheet3);
                handler->safetyvent_off();
                handler->pinchvalve_on();
            int nonlinear_prevac = readsensorvalue(); // Assuming this function reads the current sensor value
                              int nonlinear_vac = std::min(nonlinear_prevac, static_cast<int>(ia2vacline));
                              ui->label_109->setText(QString::number(nonlinear_vac));
                              motoron(ui->lineEdit_67);
                              if(speakeronoff == "Speaker ON"){
                              handler->speaker_on(nonlinear_prevac,1,0,0);
                              }else{
                                  handler->speaker_off();
                              }
                              if (nonlinear_prevac >= ia2vacline) {
                                  motoroff(); // Turn off the motor
                                  if(speakeronoff == "Speaker ON"){
                                  handler->speaker_on(0,0,0,1);
                                  }else{
                                      handler->speaker_off();
                                  }
                              }
                              beepsound(2);

            }

            }
            else if(ia2 == "Surgeon"){
                if(range>=0 && range<nfpzero){
                    ui->pushButton_42->setText("0");
                      ui->dial_2->setValue(range);
                    if(!overallci){
                 ui->CI5_5->setStyleSheet(styleSheet4);
                        handler->pinchvalve_off();
                    }
                    handler->speaker_off();
                      motoroff();
                      if(ventonia2==false){
                        handler->safetyvent_on();
                        QThread::msleep(100);
                        handler->safetyvent_off();
                        ventonia2=true;
                      }
                      //int ia2pro=readsensorvalue();
                      ui->label_109->setText(QString::number(0));
                      beepsound(0);
                }
                if(range>=nfpzero && range<(nfpone+nfpzero)){
                    ui->pushButton_42->setText("1");
                    if(speakeronoff == "Speaker ON"){
                    handler->speaker_on(0,0,1,0);
                    }else{
                        handler->speaker_off();
                    }
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
                      beepsound(1);
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
                        double final =ia2vacline/ divi;
                        int presetvac = static_cast<int>(std::round(calibration * final));
                        int pro =readsensorvalue();
                        ui->label_109->setText(QString::number(pro));

                        if (pro < (presetvac)) {
                            motoron(ui->lineEdit_67);
                            if(speakeronoff == "Speaker ON"){
                            handler->speaker_on(pro,1,0,0);
                            }else{
                                handler->speaker_off();
                            }
                            if (!motoria2) {
                                motoron(ui->lineEdit_67);
                                motoria2 = true;
                            }
                        } else if (pro >= presetvac || motoria2) {
                            motoroff();
                            motoria2 = false;
                        } // If pro exceeds vitpreset, cap it
                        if (pro >= ia2vacline) {
                            pro = static_cast<int>(ia2vacline);
                            ui->label_109->setText(QString::number(pro));
                          //  speedofthelabe(ui->label_113);
                            motoroff();

                            if(speakeronoff == "Speaker ON"){
                            handler->speaker_on(0,0,0,1);
                            }else{
                                handler->speaker_off();
                            }
                        }

                }
                        beepsound(2);
                      }




       }

            break;
        }
        // vit
    case 7: {
        int pow7 = ui->lineEdit_71->text().toInt();
       QString vvac = ui->vitvacmode->text();  // "Surgeon" or "Panel"
        double vitpreset = ui->lineEdit_73->text().toDouble();  // vitpreset should be double for better precision
        stopPhacoTracking();
        handler->phaco_off();
   handler->phaco_power(0);
        if (range > 0 && range < nfpzero) {
            // State 0
            ui->pushButton_42->setText("0");
            ui->dial_2->setValue(range);
  handler->vibrator_off();
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
            ui->label_118->setText(QString::number(0));
 handler->speaker_off();
 beepsound(0);
            handler->vit_off();
        } else if (range >= nfpzero && range < (nfpzero + nfpone)) {
            // State 1
            ui->pushButton_42->setText("1");
            if(speakeronoff == "Speaker ON"){
            handler->speaker_on(0,0,1,0);
            }else{
                handler->speaker_off();
            }            //footpedalbeep();
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
            beepsound(1);

        } else if (range >= (nfpzero + nfpone) && range < (nfpzero + nfpone + nfptwo)) {
            // State 2
            ui->pushButton_42->setText("2");
            ventonvit = false;
            beepsound(2);
            ui->dial_2->setValue(range);
            ui->CI5_5->setStyleSheet(styleSheet3);
            handler->safetyvent_off();
            handler->pinchvalve_on();
if(swapingonoff == "Swap OFF"){
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
                    if(speakeronoff == "Speaker ON"){
                    handler->speaker_on(pro,1,0,0);
                    }else{
                        handler->speaker_off();
                    }

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
                        if(speakeronoff == "Speaker ON"){
                        handler->speaker_on(0,0,0,1);
                        }else{
                            handler->speaker_off();
                        }
                    }
                }

            } else if (vvac == "Panel") {
                int nonlinear_prevac = readsensorvalue();  // Current sensor value
                int nonlinear_vac = std::min(nonlinear_prevac, static_cast<int>(vitpreset));
                ui->label_118->setText(QString::number(nonlinear_vac));
                if(speakeronoff=="Speaker ON"){
                handler->speaker_on(nonlinear_prevac,1,0,0);
}else{
                    handler->speaker_off();
                }
                // Motor control for Panel
                motoron(ui->lineEdit_72);
                if (nonlinear_prevac >= vitpreset) {
                    motoroff();
                    if(speakeronoff == "Speaker ON"){
                    handler->speaker_on(0,0,0,1);
                    }else{
                        handler->speaker_off();
                    }

                }
            }

            ui->label_119->setText("0");
            handler->vit_off();
}if(swapingonoff == "Swap ON"){


    if (!vitonoff) {  // vitonoff check for vit actions

        ui->label_119->setText(QString::number(static_cast<int>(pow7)));
     // qDebug()<<"the value is from the vit"<<pow7;
        handler->vit_on((pow7));
      //  handler->vit_ontime(30);
    }else{
              handler->vit_off();
    }

//    if (vvac == "Surgeon") {
//        const int MIN_RANGE = nfpzero + nfpone ;
//        const int MAX_RANGE = nfpzero + nfpone + nfptwo ;
//        int divi = MAX_RANGE - MIN_RANGE;
//        int calibration = range - MIN_RANGE;

//        if (divi != 0) {
//            double final = static_cast<double>(vitpreset) / static_cast<double>(divi);
//            int presetvac = static_cast<int>(std::round(calibration * final));
//            int pro = readsensorvalue();
//            ui->label_118->setText(QString::number(pro));
//            if(speakeronoff == "Speaker ON"){
//            handler->speaker_on(presetvac,1,0,0);
//            }else{
//                handler->speaker_off();
//            }
//            if (pro < presetvac) {
//                motoron(ui->lineEdit_72);
//                if (!motorvit) {
//                    motorvit = true;
//                }
//            } else if (motorvit) {
//                motoroff();
//                motorvit = false;
//            }

//            if (pro >= vitpreset) {
//                pro = static_cast<int>(vitpreset);
//                ui->label_118->setText(QString::number(pro));
//                if(speakeronoff == "Speaker ON"){
//                handler->speaker_on(pro,0,0,1);
//                }else{
//                    handler->speaker_off();
//                }
//                motoroff();
//            }
//        }
//    } else if (vvac == "Panel") {
//        int nonlinear_prevac = readsensorvalue();
//        int nonlinear_vac = std::min(nonlinear_prevac, static_cast<int>(vitpreset));
//        ui->label_118->setText(QString::number(nonlinear_vac));
//        if(speakeronoff == "Speaker ON"){
//        handler->speaker_on(nonlinear_prevac,1,0,0);
//        }else{
//            handler->speaker_off();
//        }
//        motoron(ui->lineEdit_72);
//        if (nonlinear_prevac >= vitpreset) {
//            motoroff();
//            if(speakeronoff == "Speaker ON"){
//            handler->speaker_on(nonlinear_prevac,0,0,1);
//            }else{
//                handler->speaker_off();
//            }
//        }
//    }



}
        } else if (range >= (nfpzero + nfpone + nfptwo) && range < (nfpzero + nfpone + nfptwo + nfpthree)) {
            // State 3
            ui->pushButton_42->setText("3");
            ventonvit = false;
            beepsound(3);
            ui->dial_2->setValue(range);
            ui->CI5_5->setStyleSheet(styleSheet3);
            handler->safetyvent_off();
            handler->pinchvalve_on();
if(swapingonoff == "Swap OFF"){
            if (!vitonoff) {  // vitonoff check for vit actions

                ui->label_119->setText(QString::number(static_cast<int>(pow7)));
             // qDebug()<<"the value is from the vit"<<pow7;
                handler->vit_on((pow7));
              //  handler->vit_ontime(30);
            }else{
                      handler->vit_off();
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
                    if(speakeronoff == "Speaker ON"){
                    handler->speaker_on(presetvac,1,0,0);
                    }else{
                        handler->speaker_off();
                    }
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
                        if(speakeronoff == "Speaker ON"){
                        handler->speaker_on(pro,0,0,1);
                        }else{
                            handler->speaker_off();
                        }
                        motoroff();
                    }
                }
            } else if (vvac == "Panel") {
                int nonlinear_prevac = readsensorvalue();
                int nonlinear_vac = std::min(nonlinear_prevac, static_cast<int>(vitpreset));
                ui->label_118->setText(QString::number(nonlinear_vac));
                if(speakeronoff == "Speaker ON"){
                handler->speaker_on(nonlinear_prevac,1,0,0);
                }else{
                    handler->speaker_off();
                }
                motoron(ui->lineEdit_72);
                if (nonlinear_prevac >= vitpreset) {
                    motoroff();
                    if(speakeronoff == "Speaker ON"){
                    handler->speaker_on(nonlinear_prevac,0,0,1);
                    }else{
                        handler->speaker_off();
                    }
                }
            }

        }
else if(swapingonoff == "Swap ON"){
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
            if(speakeronoff == "Speaker ON"){
            handler->speaker_on(presetvac,1,0,0);
            }else{
                handler->speaker_off();
            }
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
                if(speakeronoff == "Speaker ON"){
                handler->speaker_on(pro,0,0,1);
                }else{
                    handler->speaker_off();
                }
                motoroff();
            }
        }
    } else if (vvac == "Panel") {
        int nonlinear_prevac = readsensorvalue();
        int nonlinear_vac = std::min(nonlinear_prevac, static_cast<int>(vitpreset));
        ui->label_118->setText(QString::number(nonlinear_vac));
        if(speakeronoff == "Speaker ON"){
        handler->speaker_on(nonlinear_prevac,1,0,0);
        }else{
            handler->speaker_off();
        }
        motoron(ui->lineEdit_72);
        if (nonlinear_prevac >= vitpreset) {
            motoroff();
            if(speakeronoff == "Speaker ON"){
            handler->speaker_on(nonlinear_prevac,0,0,1);
            }else{
                handler->speaker_off();
            }
        }
    }
        }
        }

        break;
    }


    }

}
void MainWindow::updateLabel()
{

    if (!elapsedTimer->isValid()) {
    }
}


//void MainWindow::updateTimes(QElapsedTimer* timer, float progress4, int& cumulativeElapsedTimeSec, int& cumulativeEffectiveTimeSec, QPushButton* elapsedTimeLabel, QPushButton* effectiveTimeLabel) {
//    // Ensure the timer is not null
//    if (!timer) {
//        qDebug() << "Timer is null!";
//        return;
//    }

//    // If the timer is not started, start it
//    if (!timer->isValid()) {
//        timer->start();
//        qDebug() << "Timer started!";
//    }

//    // Get elapsed time in milliseconds
//    int elapsedTimeMs = timer->elapsed();
//    qDebug() << "Elapsed Time (ms):" << elapsedTimeMs;  // Debugging elapsed time in ms

//    // If elapsed time is 0, print a message to help debug
//    if (elapsedTimeMs == 0) {
//        qDebug() << "Elapsed time is still 0!";
//    }

//    // Calculate minutes, seconds, and milliseconds for elapsed time
//    int elapsedTimeSec = elapsedTimeMs / 1000;  // Get the full seconds part
//    int milliseconds = elapsedTimeMs % 1000;  // Get the milliseconds part
//    int minutes = elapsedTimeSec / 60;  // Calculate minutes
//    elapsedTimeSec = elapsedTimeSec % 60;  // Get remaining seconds

//    // Add current elapsed time to cumulative elapsed time
//    cumulativeElapsedTimeSec += elapsedTimeSec;

//    // Format the elapsed time string as min:sec::ms
//    QString elapsedTimeStr = QString::number(minutes) + ":"
//                             + QString::number(elapsedTimeSec).rightJustified(2, '0') + ":"
//                             + QString::number(milliseconds).rightJustified(3, '0');

//    qDebug() << "Elapsed Time (min:sec::ms):" << elapsedTimeStr;

//    // Update the UI with formatted elapsed time
//    elapsedTimeLabel->setText(elapsedTimeStr);

//    // Calculate effective time: This formula might need adjustment depending on your requirement
//    int effectiveTimeSec = elapsedTimeSec * progress4;  // Calculate effective time in seconds
//    cumulativeEffectiveTimeSec += effectiveTimeSec;

//    qDebug() << "Effective Time (seconds):" << effectiveTimeSec;

//    // Calculate minutes, seconds, and milliseconds for effective time
//    int effectiveMinutes = effectiveTimeSec / 60;  // Calculate minutes
//    int effectiveSeconds = effectiveTimeSec % 60;  // Get remaining seconds
//    int effectiveMilliseconds = static_cast<int>((progress4 - int(progress4)) * 1000);  // Calculate milliseconds

//    // Format the effective time string as min:sec::ms
//    QString effectiveTimeStr = QString::number(effectiveMinutes) + ":"
//                               + QString::number(effectiveSeconds).rightJustified(2, '0') + ":"
//                               + QString::number(effectiveMilliseconds).rightJustified(3, '0');

//    // Update the UI with effective time
//    effectiveTimeLabel->setText(effectiveTimeStr);
//}
// Function to update the elapsed time and effective time
//void MainWindow::updateTimes(float progress4, QPushButton* elapsedTimeLabel, QPushButton* effectiveTimeLabel) {
//    // Ensure the timer is not null
//    if (!elapsedTimer) {
//        qDebug() << "Timer is null!";
//        return;
//    }

//    // Get elapsed time in milliseconds
//    int elapsedTimeMs = elapsedTimer->elapsed();
//    qDebug() << "Elapsed Time (ms):" << elapsedTimeMs;

//    // Calculate minutes, seconds, and milliseconds for elapsed time
//    int elapsedTimeSec = elapsedTimeMs / 1000;  // Get the full seconds part
//    int milliseconds = elapsedTimeMs % 1000;  // Get the milliseconds part
//    int minutes = elapsedTimeSec / 60;  // Calculate minutes
//    elapsedTimeSec = elapsedTimeSec % 60;  // Get remaining seconds

//    // Add current elapsed time to cumulative elapsed time
//    cumulativeElapsedTimeSec += elapsedTimeSec;

//    // Format the elapsed time string as min:sec::ms
//    QString elapsedTimeStr = QString::number(minutes) + ":"
//                             + QString::number(elapsedTimeSec).rightJustified(2, '0') + ":"
//                             + QString::number(milliseconds).rightJustified(3, '0');
//    qDebug() << "Elapsed Time (min:sec::ms):" << elapsedTimeStr;

//    // Update the UI with formatted elapsed time
//    elapsedTimeLabel->setText(elapsedTimeStr);


//    // Check that progress4 is within expected range [0.0, 1.0]
//    if (progress4 < 0.0f) {
//               progress4 = 0.0f;
//           } else if (progress4 > 1.0f) {
//               progress4 = 1.0f;
//           }

//          int effectiveTime = elapsedTimeSec * progress4;  // Effective time based on progress
//          cumulativeEffectiveTimeSec += effectiveTime;

//          qDebug() << "Effective Time (seconds):" << effectiveTime;

//          // Format the effective time as mm:ss:ms
//          int effectiveMinutes = cumulativeEffectiveTimeSec / 60;
//          int effectiveSeconds = cumulativeEffectiveTimeSec % 60;
//          int effectiveMilliseconds = cumulativeEffectiveTimeSec % 1000;

//          QString effectiveTimeStr = QString::number(effectiveMinutes) + ":"
//                                     + QString::number(effectiveSeconds).rightJustified(2, '0') + ":"
//                                     + QString::number(effectiveMilliseconds).rightJustified(3, '0');

//          qDebug() << "Effective Time (min:sec:ms):" << effectiveTimeStr;

//          // Update the UI with formatted effective time
//          effectiveTimeLabel->setText(effectiveTimeStr);

//}

void MainWindow::on_elapsed_time_clicked()
{
    // Reset the elapsed time and cumulative elapsed time
    if (elapsedTimer) {
        elapsedTimer->restart();  // Restart the timer to reset elapsed time
    }
    cumulativeElapsedTimeSec = 0;  // Reset the cumulative elapsed time

    // Reset the UI labels to "00:00"
    ui->elapsed_time->setText("00:00");
    ui->elapsed_time_2->setText("00:00");
}

void MainWindow::mainus1button()
{
    if(nHandPiece ==0 && nHandPiece1 ==1){
    ULTRASONICBUT1();
    activategpio();
    ui->label_32->show();
    }
    else{
        DIATHERMYBUT();
        disablegpio();
    }
}

void MainWindow::on_elapsed_time_2_clicked()
{
    // Reset the elapsed time and cumulative elapsed time
    if (elapsedTimer) {
        elapsedTimer->restart();  // Restart the timer to reset elapsed time
    }
    cumulativeEffectiveTimeSec = 0;  // Reset the cumulative elapsed time

    // Reset the UI labels to "00:00"
    ui->elapsed_time->setText("00:00");
    ui->elapsed_time_2->setText("00:00");
}

void MainWindow::updateTimes(float progress4, QPushButton* elapsedTimeLabel, QPushButton* effectiveTimeLabel) {
    // Ensure the timer is not null
    if (!elapsedTimer) {
        qDebug() << "Timer is null!";
        return;
    }

    // Get elapsed time in milliseconds
    int elapsedTimeMs = elapsedTimer->elapsed();

    // Calculate minutes and seconds for elapsed time
    int elapsedTimeSec = elapsedTimeMs / 1000;  // Get the full seconds part
    int minutes = elapsedTimeSec / 60;         // Calculate minutes
    elapsedTimeSec = elapsedTimeSec % 60;      // Get remaining seconds

    // Add current elapsed time to cumulative elapsed time
    cumulativeElapsedTimeSec += elapsedTimeSec;

    // Format the elapsed time string as min:sec
    QString elapsedTimeStr = QString::number(minutes) + ":"
                             + QString::number(elapsedTimeSec).rightJustified(2, '0');

    // Update the UI with formatted elapsed time
    elapsedTimeLabel->setText(elapsedTimeStr);

    // Calculate effective time using progress4
    // Ensure progress4 is within bounds (clamp between 0.0 and 1.0)
    if (progress4 < 0.0f) {
        progress4 = 0.0f;
    } else if (progress4 > 1.0f) {
        progress4 = 1.0f;
    }

    // Calculate effective time based on cumulative elapsed time and progress4
    float effectiveTime = static_cast<float>(cumulativeElapsedTimeSec) * progress4;
    int effectiveTimeSec = static_cast<int>(effectiveTime); // Cast effectiveTime to int before using

    // Format the effective time as min:sec
    int effectiveMinutes = effectiveTimeSec / 60; // Convert to minutes
    int effectiveSeconds = effectiveTimeSec % 60; // Remaining seconds

    // Format the effective time as min:sec
    QString effectiveTimeStr = QString::number(effectiveMinutes) + ":"
                               + QString::number(effectiveSeconds).rightJustified(2, '0');

    // Update the UI with formatted effective time
    effectiveTimeLabel->setText(effectiveTimeStr);
}





//us1 non linear and linear
void MainWindow::us1_linear_nonlinear()
{
     handler->buzz();
     // ui->us1mode->setText("SURGEON");
     us1=ui->us1mode->text();

       if(us1 == "Surgeon" )
       {
           ui->us1mode->setStyleSheet(
               "font: 15pt;"
               "image: url(:/images/panelbutton.png);"
               "background-color: transparent;"
               "border: none;"
               "outline: none;"
           );
            ui->us1mode->setText("Panel");

             footpedalcheck();


       }
       else if(us1 == "Panel" )
       {
           ui->us1mode->setStyleSheet(
               "font: 15pt;"
               "image: url(:/images/panelbutton.png);"
               "background-color: transparent;"
               "border: none;"
               "outline: none;"
           );
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

           ui->us1vacmode->setStyleSheet(
               "font: 15pt;"
               "image: url(:/images/panelbutton.png);"
               "background-color: transparent;"
               "border: none;"
               "outline: none;"
           );
            ui->us1vacmode->setText("Panel");

             footpedalcheck();
       }
       else if(vus1=="Panel")
       {

           ui->us1vacmode->setStyleSheet(
               "font: 15pt;"
               "image: url(:/images/panelbutton.png);"
               "background-color: transparent;"
               "border: none;"
               "outline: none;"
           );
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

           ui->us2mode->setStyleSheet(
               "font: 15pt;"
               "image: url(:/images/panelbutton.png);"
               "background-color: transparent;"
               "border: none;"
               "outline: none;"
           );
             ui->us2mode->setText("Panel");


              footpedalcheck();
               }
       else if(us2 == "Panel")
       {
           ui->us2mode->setStyleSheet(
               "font: 15pt;"
               "image: url(:/images/panelbutton.png);"
               "background-color: transparent;"
               "border: none;"
               "outline: none;"
           );
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

        ui->us2vacmode->setStyleSheet(
            "font: 15pt;"
            "image: url(:/images/panelbutton.png);"
            "background-color: transparent;"
            "border: none;"
            "outline: none;"
        );
          ui->us2vacmode->setText("Panel");

           footpedalcheck();
            }
    else if(vus2 == "Panel")
    {

        ui->us2vacmode->setText("Surgeon");
        ui->us2vacmode->setStyleSheet(
            "font: 15pt;"
            "image: url(:/images/panelbutton.png);"
            "background-color: transparent;"
            "border: none;"
            "outline: none;"
        );
      footpedalcheck();
    }

}

void MainWindow::us3_linear_nonlinear()
{
     handler->buzz();
    us3 = ui->us3mode->text();


       if(us3 == "Surgeon")
       {
           ui->us3mode->setStyleSheet(
               "font: 15pt;"
               "image: url(:/images/panelbutton.png);"
               "background-color: transparent;"
               "border: none;"
               "outline: none;"
           );
             ui->us3mode->setText("Panel");

              footpedalcheck();
       }
       else if(us3 == "Panel")
       {
           ui->us3mode->setStyleSheet(
               "font: 15pt;"
               "image: url(:/images/panelbutton.png);"
               "background-color: transparent;"
               "border: none;"
               "outline: none;"
           );
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
           ui->us3vacmode->setStyleSheet(
               "font: 15pt;"
               "image: url(:/images/panelbutton.png);"
               "background-color: transparent;"
               "border: none;"
               "outline: none;"
           );
            footpedalcheck();
     }
     else if(vus3 == "Panel")
     {

         ui->us3vacmode->setText("Surgeon");
         ui->us3vacmode->setStyleSheet(
             "font: 15pt;"
             "image: url(:/images/panelbutton.png);"
             "background-color: transparent;"
             "border: none;"
             "outline: none;"
         );
         footpedalcheck();
     }

}

void MainWindow::us4_linear_nonlinear()
{
     handler->buzz();
    us4 = ui->us4mode->text();

       if(us4 == "Surgeon")
       {

           ui->us4mode->setStyleSheet(
               "font: 15pt;"
               "image: url(:/images/panelbutton.png);"
               "background-color: transparent;"
               "border: none;"
               "outline: none;"
           );
             ui->us4mode->setText("Panel");

             footpedalcheck();
                }
       else if(us4 == "Panel")
       {
           ui->us4mode->setText("Surgeon");
           ui->us4mode->setStyleSheet(
               "font: 15pt;"
               "image: url(:/images/panelbutton.png);"
               "background-color: transparent;"
               "border: none;"
               "outline: none;"
           );


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
          ui->us4vacmode->setStyleSheet(
              "font: 15pt;"
              "image: url(:/images/panelbutton.png);"
              "background-color: transparent;"
              "border: none;"
              "outline: none;"
          );
          footpedalcheck();
             }
    else if(vus4 == "Panel")
    {

           ui->us4vacmode->setText("Surgeon");
           ui->us4vacmode->setStyleSheet(
               "font: 15pt;"
               "image: url(:/images/panelbutton.png);"
               "background-color: transparent;"
               "border: none;"
               "outline: none;"
           );
            footpedalcheck();
    }

}



void MainWindow::ia1_linear_nonlinear()
{
     handler->buzz();
    ia1= ui->ia1mode->text();

       if(ia1== "Surgeon")
       {

           ui->ia1mode->setStyleSheet(
               "font: 15pt;"
               "image: url(:/images/panelbutton.png);"
               "background-color: transparent;"
               "border: none;"
               "outline: none;"
           );
           ui->ia1mode->setText("Panel");
            footpedalcheck();
               }
       else if(ia1 == "Panel")
       {
           ui->ia1mode->setStyleSheet(
               "font: 15pt;"
               "image: url(:/images/panelbutton.png);"
               "background-color: transparent;"
               "border: none;"
               "outline: none;"
           );
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
           ui->ia2mode->setStyleSheet(
               "font: 15pt;"
               "image: url(:/images/panelbutton.png);"
               "background-color: transparent;"
               "border: none;"
               "outline: none;"
           );             footpedalcheck();
                }
       else if(ia2 == "Panel")
       {
           ui->ia2mode->setText("Surgeon");
           ui->ia2mode->setStyleSheet(
               "font: 15pt;"
               "image: url(:/images/panelbutton.png);"
               "background-color: transparent;"
               "border: none;"
               "outline: none;"
           ); footpedalcheck();
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
// handler->buzz();
    nPulseCount = ui->lineEdit_75->text().toInt();
     nPulseCount++;
    if ( nPulseCount > 15) {
         nPulseCount = 15;
    }
    ui->lineEdit_75->setText(QString::number(nPulseCount));
   // qDebug()<<"the increment pulse count is"<<nPulseCount;
 //   handler->phaco_on(nfpzero+nfpone+nfptwo);
  //   handler->fs_count(nfpzero+nfpone+nfptwo);
    handler->freq_count(nFreqCount);
handler->pdm_mode(PULSE_MODE);
    handler->pulse_count(nPulseCount);


  //qDebug()<<pulse;
}

void MainWindow::pulsedown_mode()
{
 //handler->buzz();

     nPulseCount = ui->lineEdit_75->text().toInt();
    nPulseCount--;
    if ( nPulseCount < 1) {
         nPulseCount = 1;
    }
    ui->lineEdit_75->setText(QString::number(nPulseCount));
     //qDebug()<<"the decrement pulse count is"<<nPulseCount;
    // handler->phaco_on(nfpzero+nfpone+nfptwo);
    //  handler->fs_count(nfpzero+nfpone+nfptwo);
    handler->freq_count(nFreqCount);
    handler->pdm_mode(PULSE_MODE);
    handler->pulse_count( nPulseCount);
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
// handler->buzz();
    nSingleBurst = ui->lineEdit_78->text().toInt();
    nSingleBurst += 10;
    if (nSingleBurst > 400) {
        nSingleBurst = 400;
    }
    ui->lineEdit_78->setText(QString::number(nSingleBurst));
//    handler->phaco_on(nfpzero+nfpone+nfptwo);
  //   handler->fs_count(nfpzero+nfpone+nfptwo);
   handler->freq_count(nFreqCount);
    handler->freq_count(nFreqCount);
     handler->pdm_mode(SINGLE_BURST);
     handler->burst_length(nSingleBurst);
      //qDebug()<<singleburst;
}

void MainWindow::singleburstdown_mode()
{
// handler->buzz();
    nSingleBurst = ui->lineEdit_78->text().toInt();
   nSingleBurst -= 10;
    if (nSingleBurst < 10) {
        nSingleBurst = 10;
    }
    ui->lineEdit_78->setText(QString::number(nSingleBurst));
  //  handler->phaco_on(nfpzero+nfpone+nfptwo);
   //  handler->fs_count(nfpzero+nfpone+nfptwo);
   handler->freq_count(nFreqCount);
     handler->pdm_mode(SINGLE_BURST);
   handler->burst_length(nSingleBurst);
    //qDebug()<<singleburst;
}
//multiburst
void MainWindow::multiburstup_mode()
{
// handler->buzz();
    //int footrange=footswitchrange();
    nMultiBurstCount = ui->lineEdit_79->text().toInt();
    nMultiBurstCount += 10;
    if (nMultiBurstCount > 400) {
        nMultiBurstCount = 400;
    }
    ui->lineEdit_79->setText(QString::number(nMultiBurstCount));
  //  handler->fs_count(nfpzero+nfpone+nfptwo);
    handler->freq_count(nFreqCount);
    handler->pdm_mode(MULTI_BURST);
    handler->burst_length(nMultiBurstCount);

}

void MainWindow::multiburstdown_mode()
{
// handler->buzz();
     //int footrange=footswitchrange();
    nMultiBurstCount = ui->lineEdit_79->text().toInt();
    nMultiBurstCount -= 10;
    if (nMultiBurstCount < 10) {
        nMultiBurstCount = 10;
    }
    ui->lineEdit_79->setText(QString::number(nMultiBurstCount));
  //  handler->fs_count(nfpzero+nfpone+nfptwo);
    handler->freq_count(nFreqCount);
    handler->pdm_mode(MULTI_BURST);
    handler->burst_length(nMultiBurstCount);

}
//ocupulse
void MainWindow::ocupulseup_mode()
{



}

void MainWindow::ocupulsedown_mode()
{

}
//coldphaco
void MainWindow::on_ButColdPhacoPer_down_clicked()
{
    // handler->buzz();
    nColdPhacoPer = ui->lineEdit_80->text().toInt();
    nColdPhacoPer -= 5;
    if (nColdPhacoPer < 5) {
        nColdPhacoPer = 5;
    }
    ui->lineEdit_80->setText(QString::number(nColdPhacoPer));
  //  handler->fs_count(nfpzero+nfpone+nfptwo);
    handler->freq_count(nFreqCount);
    handler->pdm_mode(COLD_PHACO);

    handler->cold_pulse(nColdPhacoPer,nColdPhacoCount);
}


void MainWindow::on_ButColdPulse_up_clicked()
{
    // handler->buzz();
     nColdPhacoCount = ui->lineEdit_81->text().toInt();
     nColdPhacoCount += 1;
     if (nColdPhacoCount > 15) {
         nColdPhacoCount = 15;
     }
     ui->lineEdit_81->setText(QString::number(nColdPhacoCount));
   //  handler->fs_count(nfpzero+nfpone+nfptwo);
     handler->freq_count(nFreqCount);
     handler->pdm_mode(COLD_PHACO);

     handler->cold_pulse(nColdPhacoPer,nColdPhacoCount);

}


void MainWindow::on_ButColdPulse_down_clicked()
{
    //  handler->buzz();
      nColdPhacoCount = ui->lineEdit_81->text().toInt();
      nColdPhacoCount -= 1;
      if (nColdPhacoCount < 1) {
          nColdPhacoCount = 1;
      }
      ui->lineEdit_81->setText(QString::number(nColdPhacoCount));
     // handler->fs_count(nfpzero+nfpone+nfptwo);
      handler->freq_count(nFreqCount);
      handler->pdm_mode(COLD_PHACO);

      handler->cold_pulse(nColdPhacoPer,nColdPhacoCount);

}


void MainWindow::on_ButColdPhacoPer_up_clicked()
{
    //handler->buzz();
       nColdPhacoPer = ui->lineEdit_80->text().toInt();
       nColdPhacoPer += 5;
       if (nColdPhacoPer > 40) {
           nColdPhacoPer = 40;
       }
       ui->lineEdit_80->setText(QString::number(nColdPhacoPer));
      // handler->fs_count(nfpzero+nfpone+nfptwo);
       handler->freq_count(nFreqCount);
       handler->pdm_mode(COLD_PHACO);

     handler->cold_pulse(nColdPhacoPer,nColdPhacoCount);
}

void MainWindow::coldphacoup_mode()
{

}

void MainWindow::coldphacodown_mode()
{

}

void MainWindow::coldphaco1up_mode()
{

}

void MainWindow::coldphaco1down_mode()
{

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
  int hand = readGPIOValue(960);


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
   if(hand==0)
   {
   ui->pushButton->setStyleSheet(styleSheet4);
   ui->label_19->show();


   }
   else if(hand==1)
   {
     ui->pushButton->setStyleSheet(styleSheet5);
     ui->label_19->hide();

   }
}
void MainWindow::onUpdateStatusTimeout(){
    updatehandpieceStatus();
}

void MainWindow::linearvaccum()
{



}
void MainWindow::updateProgressBar() {

}

void MainWindow::motoron(QLineEdit *lineEdit)
{
    int linevalue=lineEdit->text().toInt();
    handler->write_motor(0x01,0x03,linevalue);
}

void MainWindow::motoroff()
{
    handler->write_motor(0x00,0x00,0);
}

void MainWindow::motorccwon()
{
    qDebug()<<"the reflux motor is working";
handler->write_motor(0x01,0x07,2);
   //handler->write_motor(0x00,0x07,2);
   //handler->write_motor(0x01,0x03,40);


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





void MainWindow::updateTimers()
{
    // ── 1. Always update the live clock display ──────────────────────────
    QTime time = QTime::currentTime();
    ui->lab_time->setText(time.toString("hh:mm:ss"));

    // ── 2. If phaco is NOT currently firing, nothing more to do ──────────
    if (!m_phacoFiring)
        return;

    // ── 3. Compute how many ms have elapsed in this burst ────────────────
    qint64 burstMs = m_phacoSegTimer.elapsed();   // ms since last phaco_on()

    // Total EPT = all previous complete bursts  +  current burst so far
    qint64 totalEptMs = m_eptTotalMs + burstMs;

    // ── 4. Format EPT as  mm:ss  and push to UI ──────────────────────────
    int totalSec = static_cast<int>(totalEptMs / 1000);
    int eptMin   = totalSec / 60;
    int eptSec   = totalSec % 60;
    QString eptStr = QString("%1:%2")
                         .arg(eptMin, 2, 10, QChar('0'))
                         .arg(eptSec, 2, 10, QChar('0'));
    ui->elapsed_time->setText(eptStr);          // EPT display label

    // ── 5. USD = cumulative (power% × elapsed_seconds) ───────────────────
    //       Add the contribution of the current burst to the running total
    double burstUsd   = static_cast<double>(m_currentPowerPct)
                        * (static_cast<double>(burstMs) / 1000.0);
    double displayUsd = m_usdTotal + burstUsd;

    // Display USD rounded to one decimal  (e.g. "47.3 %·s")
    ui->elapsed_time_2->setText(
        QString::number(displayUsd, 'f', 1));    // USD display label
}

void MainWindow::changesvaluesql()
{
    // Set up the database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(PATH);

    if (!db.open()) {
        qDebug() << "SQL database failed to open:" << db.lastError().text();
        return;
    }

    QSqlQuery query(db);
    query.prepare(R"(
        UPDATE phacohigh SET
            Quadvacmode = :Quadvacmode,
            Quadpowermethod = :Quadpowermethod,
            Quadpowmode = :Quadpowmode,
            Chopvacmode = :Chopvacmode,
            Choppowermethod = :Choppowermethod,
            Choppowmode = :Choppowmode,
            Sculptvacmode = :Sculptvacmode,
            Sculptpowermethod = :Sculptpowermethod,
            Sculptpowmode = :Sculptpowmode,
            cortexvacmode = :cortexvacmode,
            polishvacmode = :polishvacmode,
            vitcutmode = :vitcutmode,
            vitvacmode = :vitvacmode,
            Epinpowermethod = :Epinpowermethod,
            Epinpowmode = :Epinpowmode,
            Epinvacmode = :Epinvacmode
        WHERE surgeon = :surgeon
    )");

    // Bind values from UI to the query
    query.bindValue(":Quadvacmode", ui->us2vacmode->text());
    query.bindValue(":Quadpowermethod", ui->CutMode_vitCom_2->currentText());
    query.bindValue(":Quadpowmode", ui->us2mode->text());

    query.bindValue(":Chopvacmode", ui->us3vacmode->text());
    query.bindValue(":Choppowermethod", ui->CutMode_vitCom_3->currentText());
    query.bindValue(":Choppowmode", ui->us3mode->text());

    query.bindValue(":Sculptvacmode", ui->us4vacmode->text());
    query.bindValue(":Sculptpowermethod", ui->CutMode_vitCom_4->currentText());
    query.bindValue(":Sculptpowmode", ui->us4mode->text());

    // Assuming appropriate UI elements for cortex, polish, and vit modes
    query.bindValue(":cortexvacmode", ui->ia1mode->text());
    query.bindValue(":polishvacmode", ui->ia2mode->text());
    query.bindValue(":vitcutmode", ui->vitmode->text());
    query.bindValue(":vitvacmode", ui->vitvacmode->text());

    query.bindValue(":Epinpowermethod", ui->CutMode_vitCom->currentText());
    query.bindValue(":Epinpowmode", ui->us1mode->text());
    query.bindValue(":Epinvacmode", ui->us1vacmode->text());

    query.bindValue(":surgeon", ui->comboBox_4->currentText());

    // Execute the query and check for errors
    if (!query.exec()) {
      //  qDebug() << "Update failed:" << query.lastError().text();
    } else {
     //  qDebug() << "Update successful";
    }

    db.close(); // Close the database connection
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection); // Cleanup
}

void MainWindow::changebuttonstyle()
{
    ui->label_19->show();
}
void MainWindow::updateCurrentButtonIndex(int index) {
    currentButtonIndex = index;
}

void MainWindow::moved(int gpio) {
    QPushButton *buttons[] = {
         ui->ULTRASONICBUT1, ui->ULTRASONICBUT2,
         ui->ULTRASONICBUT3, ui->ULTRASONICBUT4, ui->IA1BUT,
         ui->IA2BUT, ui->VITRECTOMYBUT, ui->DIABUT
      };
    QPushButton *buttons1[] = {
         ui->DIABUT, ui->IA1BUT,
         ui->IA2BUT, ui->VITRECTOMYBUT
    };
    int totalButtons = sizeof(buttons) / sizeof(buttons[0]);
    int totalButtons1 = sizeof(buttons1) / sizeof(buttons1[0]);

    // Handling for nHandPiece == 0
    if (gpio == 0 && nHandPiece == 0 && nHandPiece1 == 1) {
        if (currentButtonIndex == -1) {
            currentButtonIndex = 0; // Start from the first button (IA1)
        } else {
            currentButtonIndex = (currentButtonIndex + 1) % totalButtons;
        }

      //  qDebug() << "Handpiece 0, GPIO: " << gpio << ", Button Index: " << currentButtonIndex;
      //  qDebug() << "Selected Button: " << buttons[currentButtonIndex]->text();

        // Handle each button's specific action
        if (buttons[currentButtonIndex] == ui->ULTRASONICBUT1) {
            ULTRASONICBUT1();
        } else if (buttons[currentButtonIndex] == ui->ULTRASONICBUT2) {
            ULTRASONICBUT2();
        } else if (buttons[currentButtonIndex] == ui->ULTRASONICBUT3) {
            ULTRASONICBUT3();
        } else if (buttons[currentButtonIndex] == ui->ULTRASONICBUT4) {
            ULTRASONICBUT4();
        } else if (buttons[currentButtonIndex] == ui->IA1BUT) {
            IRRIGATIONBUT1();
        } else if (buttons[currentButtonIndex] == ui->IA2BUT) {
            IRRIGATIONBUT2();
        } else if (buttons[currentButtonIndex] == ui->VITRECTOMYBUT) {
            VITRECTOMYBUT();
        } else if (buttons[currentButtonIndex] == ui->DIABUT) {
            DIATHERMYBUT();
        }

        buttons[currentButtonIndex]->click();
        buttons[currentButtonIndex]->setFocus();
    }

    // Handling for nHandPiece == 1
    if (gpio == 0 && nHandPiece == 1 && nHandPiece1 == 0) {
        if (currentButtonIndex == -1) {
            currentButtonIndex = 0; // Start from DIABUT
        } else {
            currentButtonIndex = (currentButtonIndex + 1) % totalButtons1;
        }

//        // Debugging output
       // qDebug() << "Handpiece 1, GPIO: " << gpio << ", Button Index: " << currentButtonIndex;
        //qDebug() << "Selected Button: " << buttons1[currentButtonIndex]->text();

        // Handle each button's specific action
        if (buttons1[currentButtonIndex] == ui->DIABUT) {
            DIATHERMYBUT();
        } else if (buttons1[currentButtonIndex] == ui->IA1BUT) {
            IRRIGATIONBUT1();
        } else if (buttons1[currentButtonIndex] == ui->IA2BUT) {
            IRRIGATIONBUT2();
        } else if (buttons1[currentButtonIndex] == ui->VITRECTOMYBUT) {
            VITRECTOMYBUT();
        }

        buttons1[currentButtonIndex]->click();
        buttons1[currentButtonIndex]->setFocus();
    }
}



void MainWindow::movePushButtonBottomToTop(int gpio) {
    // Define button arrays for each set of buttons
    QPushButton *buttons[] = {
        ui->ULTRASONICBUT1, ui->ULTRASONICBUT2,
        ui->ULTRASONICBUT3, ui->ULTRASONICBUT4, ui->IA1BUT,
        ui->IA2BUT, ui->VITRECTOMYBUT, ui->DIABUT
    };

    QPushButton *buttons1[] = {
        ui->DIABUT, ui->IA1BUT,
        ui->IA2BUT, ui->VITRECTOMYBUT
    };

    int totalButtons = sizeof(buttons) / sizeof(buttons[0]);
    int totalButtons1 = sizeof(buttons1) / sizeof(buttons1[0]);

    // Ensure gpio is 0 for processing
    if (gpio == 0) {
        if (nHandPiece == 0 && nHandPiece1 == 1) {
            // Handle button index for the first set of buttons (buttons)
            if (currentButtonIndex == -1) {
                currentButtonIndex = totalButtons - 1; // Start at the last button
            } else {
                // Move to the previous button (bottom to top)
                currentButtonIndex = (currentButtonIndex - 1 + totalButtons) % totalButtons;
            }
           // qDebug() << "Decrementing button, currentButtonIndex: " << currentButtonIndex;

            // Handle corresponding actions based on the selected button
            if (buttons[currentButtonIndex] == ui->ULTRASONICBUT1) {
                // Handle ULTRASONICBUT1 as required (could be DIABUT depending on logic)
                DIATHERMYBUT();
            } else if (buttons[currentButtonIndex] == ui->ULTRASONICBUT2) {
                ULTRASONICBUT2();
            } else if (buttons[currentButtonIndex] == ui->ULTRASONICBUT3) {
                ULTRASONICBUT3();
            } else if (buttons[currentButtonIndex] == ui->ULTRASONICBUT4) {
                ULTRASONICBUT4();
            } else if (buttons[currentButtonIndex] == ui->IA1BUT) {
                IRRIGATIONBUT1();
            } else if (buttons[currentButtonIndex] == ui->IA2BUT) {
                IRRIGATIONBUT2();
            } else if (buttons[currentButtonIndex] == ui->VITRECTOMYBUT) {
                VITRECTOMYBUT();
            } else if (buttons[currentButtonIndex] == ui->DIABUT) {
                DIATHERMYBUT();
            }

            buttons[currentButtonIndex]->click();
            buttons[currentButtonIndex]->setFocus();
        }

        // Handle second set of buttons (buttons1) if nHandPiece == 1 and nHandPiece1 == 0
        if (nHandPiece == 1 && nHandPiece1 == 0) {
            if (currentButtonIndex == -1) {
                currentButtonIndex = totalButtons1 - 1; // Start at the last button
            } else {
                // Handle DIABUT separately for GPIO check
                if (buttons1[currentButtonIndex] == ui->DIABUT && gpio == 0) {
                    // If GPIO is 0, decrement to the previous button
                    currentButtonIndex = (currentButtonIndex - 1 + totalButtons1) % totalButtons1;
                } else {
                    // Move to the previous button (bottom to top)
                    currentButtonIndex = (currentButtonIndex - 1 + totalButtons1) % totalButtons1;
                }
            }

            //qDebug() << "Handpiece 1, GPIO: " << gpio << ", Button Index: " << currentButtonIndex;

            // Handle actions for buttons in buttons1
            if (buttons1[currentButtonIndex] == ui->DIABUT) {
                DIATHERMYBUT();
                qDebug() << "Diathermy mode triggered";
            } else if (buttons1[currentButtonIndex] == ui->IA1BUT) {
                IRRIGATIONBUT1();
            } else if (buttons1[currentButtonIndex] == ui->IA2BUT) {
                IRRIGATIONBUT2();
            } else if (buttons1[currentButtonIndex] == ui->VITRECTOMYBUT) {
                VITRECTOMYBUT();
            }

            buttons1[currentButtonIndex]->click();
            buttons1[currentButtonIndex]->setFocus();
        }
    }
}



void MainWindow::onPdmModeSelected(int gpio) {
    if (gpio == 0 && us1PdmMode) {
        moveTab(1);  // Only move tabs if us1 is in PDM mode
        int index=ui->CutMode_vitCom->currentIndex();
        onCutMode_vitComChanged(index);
        if(disablelayout == "DIATHERMY" || disablelayout == "IRRIGATION1" || disablelayout == "IRRIGATION2" || disablelayout == "VITRECTOMY"){
            ui->label_32->hide();
        }if(disablelayout == "ULTRASONICBUT1" ||disablelayout == "ULTRASONICBUT2"||disablelayout == "ULTRASONICBUT3"||disablelayout == "ULTRASONICBUT4"){
            ui->label_32->show();
        }
    }

}

void MainWindow::onPdmModeSelected1(int gpio) {
    if (gpio == 0 && us2PdmMode) {
        moveTab1(2);  // Only move tabs if us2 is in PDM mode
        int index1=ui->CutMode_vitCom_2->currentIndex();
        onCutMode_vitComChanged1(index1);
        if(disablelayout == "DIATHERMY" || disablelayout == "IRRIGATION1" || disablelayout == "IRRIGATION2" || disablelayout == "VITRECTOMY"){
            ui->label_32->hide();
        }if(disablelayout == "ULTRASONICBUT1" ||disablelayout == "ULTRASONICBUT2"||disablelayout == "ULTRASONICBUT3"||disablelayout == "ULTRASONICBUT4"){
            ui->label_32->show();
        }
    }
}

void MainWindow::onPdmModeSelected2(int gpio) {
    if (gpio == 0 && us3PdmMode) {
        moveTab2(3);  // Only move tabs if us3 is in PDM mode
        int index=ui->CutMode_vitCom_3->currentIndex();
        onCutMode_vitComChanged2(index);
        if(disablelayout == "DIATHERMY" || disablelayout == "IRRIGATION1" || disablelayout == "IRRIGATION2" || disablelayout == "VITRECTOMY"){
            ui->label_32->hide();
        }if(disablelayout == "ULTRASONICBUT1" ||disablelayout == "ULTRASONICBUT2"||disablelayout == "ULTRASONICBUT3"||disablelayout == "ULTRASONICBUT4"){
            ui->label_32->show();
        }
    }
}

void MainWindow::onPdmModeSelected3(int gpio) {
    if (gpio == 0 && us4PdmMode) {
        moveTab3(4);  // Only move tabs if us4 is in PDM mode
        int index=ui->CutMode_vitCom_4->currentIndex();
        onCutMode_vitComChanged3(index);
        if(disablelayout == "DIATHERMY" || disablelayout == "IRRIGATION1" || disablelayout == "IRRIGATION2" || disablelayout == "VITRECTOMY"){
            ui->label_32->hide();
        }if(disablelayout == "ULTRASONICBUT1" ||disablelayout == "ULTRASONICBUT2"||disablelayout == "ULTRASONICBUT3"||disablelayout == "ULTRASONICBUT4"){
            ui->label_32->show();
        }
    }
}

void MainWindow::moveTab(int usIndex) {
    if (!ui->tabWidget_2) {
     //   qDebug() << "tabWidget_2 is null!";
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
      //  qDebug() << "Invalid usIndex value:" << usIndex;
        return;
    }

    // Update combo boxes based on usIndex
    if (usIndex == 1) {
        if (ui->CutMode_vitCom) {
          //  qDebug() << "Updating CutMode_vitCom with index:" << currentIndex;
            ui->CutMode_vitCom->setCurrentIndex(currentIndex);
            onCutMode_vitComChanged(ui->CutMode_vitCom->currentIndex());
        } else {
           // qDebug() << "CutMode_vitCom is null!";
        }
    }

    // Update tab widget
    ui->tabWidget_2->setCurrentIndex(currentIndex);
   // qDebug() << "Moved to tab index:" << currentIndex << " for usIndex:" << usIndex;
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
     if (ui->tabWidget->currentIndex() == 7) {
           motoroff();
           handler->write_motor(0,0,0);
        }



    if (gpio == 0) {
             motorccwon();
             if (ui->tabWidget->currentIndex() == 7) {
                   motoroff();
                   handler->write_motor(0,0,0);
                }

    }
}

void MainWindow::on_CI4_2_clicked()
{
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

    // Toggle the overallci state
    overallci = !overallci;

    if (overallci) {
        // Turn ON Continuous Irrigation
        ui->CI5_5->setStyleSheet(styleSheetOn);
        handler->pinchvalve_on();       // Activate pinch valve
        handler->speaker_on(0, 0, 1, 0); // Activate speaker
    } else {
        // Turn OFF Continuous Irrigation
        ui->CI5_5->setStyleSheet(styleSheetOff);
        handler->pinchvalve_off();     // Deactivate pinch valve
        handler->speaker_off();        // Deactivate speaker
    }
    ui->CI5_5->update(); // Ensure UI updates immediately
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

    // If Diathermy mode is active, always keep pinch valve off
    if (ui->tabWidget->currentIndex() == 7) { // Check if in Diathermy tab
        if (overallci) {
            overallci = false;
            ui->CI5_5->setStyleSheet(styleSheetOff);
            handler->pinchvalve_off(); // Ensure pinch valve is off
        }
        ui->CI5_5->update();
        return; // Exit early to prevent any further action
    }

    // Adjust Continuous Irrigation based on GPIO input
    if (value == 0 && !overallci) {
        // GPIO indicates to turn ON Continuous Irrigation
        overallci = true;
        ui->CI5_5->setStyleSheet(styleSheetOn);
        handler->pinchvalve_on();
        handler->buzz(); // Indicate change
    } else if (value == 0 && overallci) {
        // GPIO indicates to turn OFF Continuous Irrigation
        overallci = false;
        ui->CI5_5->setStyleSheet(styleSheetOff);
        handler->pinchvalve_off();
        handler->buzz(); // Indicate change
    }
    ui->CI5_5->update(); // Ensure UI updates immediately
}
void MainWindow::poweronoff(int gpio)
{
    static bool isPowerOn = false; // Track the current power state

    if (gpio == 0)
    {
        if (isPowerOn)
        {
            // Turn off power and disable buttons
            enableButtons(true);
            isPowerOn = false; // Update power state
            qDebug() << "Power turned off, buttons disabled.";
        }
    }
    else if (gpio == 1)
    {
        if (!isPowerOn)
        {
            // Turn on power and enable buttons
            enableButtons(false);
            isPowerOn = true; // Update power state
            qDebug() << "Power turned on, buttons enabled.";
        }
    }
}



void MainWindow::on_us1onoff_clicked()
{
    QString styleSheetOn = "QPushButton {"
                           " font:20pt Ubuntu;"
                           " background-color: green;"
                           " color: black;"
                           " border:5px solid black;"
                           " border-radius:30px;"
                           " font-weight: bold;"
                           "}";

    QString styleSheetOff = "QPushButton {"
                            " font:20pt Ubuntu;"
                            " background-color: rgb(224, 27, 36);"
                            " color: black;"
                            " border:5px solid black;"
                            " border-radius:30px;"
                            " font-weight: bold;"
                            "}";

    // Toggle us1poweron status
    us1poweron = !us1poweron;
    QString text=ui->us1onoff->text();

    if (us1poweron && text=="OFF") {
        ui->us1onoff->setText("ON");  // Turn ON
        ui->us1onoff->setStyleSheet(styleSheetOn);
        ui->us2onoff->setText("ON");  // Turn ON
        ui->us2onoff->setStyleSheet(styleSheetOn);
        us2poweron = true;
        ui->us3onoff->setText("ON");  // Turn ON
        ui->us3onoff->setStyleSheet(styleSheetOn);
        us3poweron = true;
        ui->us4onoff->setText("ON");  // Turn ON
        ui->us4onoff->setStyleSheet(styleSheetOn);
        us4poweron = true;
        ui->us1powup_but->setEnabled(true);
        ui->us1powdown_but->setEnabled(true);
        ui->us2powup_but->setEnabled(true);
        ui->us2powdown_but->setEnabled(true);
        ui->us3powup_but->setEnabled(true);
        ui->us3powdown_but->setEnabled(true);
        ui->us4powup_but->setEnabled(true);
        ui->us4powdown_but->setEnabled(true);
        ui->vitpowup_but->setEnabled(true);
        ui->vitpowdown_but->setEnabled(true);

    } else {
        ui->us1onoff->setText("OFF");  // Turn OFF
        ui->us1onoff->setStyleSheet(styleSheetOff);
        ui->us2onoff->setText("OFF");  // Turn OFF
        ui->us2onoff->setStyleSheet(styleSheetOff);
        us2poweron = false;
        ui->us3onoff->setText("OFF");  // Turn OFF
        ui->us3onoff->setStyleSheet(styleSheetOff);
        us3poweron = false;
        ui->us4onoff->setText("OFF");  // Turn OFF
        ui->us4onoff->setStyleSheet(styleSheetOff);
        us4poweron = false;
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

    }
}





void MainWindow::on_us2onoff_clicked()
{

    QString styleSheetOn = "QPushButton {"
                           " font:20pt Ubuntu;"
                           " background-color: green;"
                           " color: black;"
                           " border:5px solid black;"
                           " border-radius:30px;"
                           " font-weight: bold;"
                           "}";

    QString styleSheetOff = "QPushButton {"
                            " font:20pt Ubuntu;"
                            " background-color: rgb(224, 27, 36);"
                            " color: black;"
                            " border:5px solid black;"
                            " border-radius:30px;"
                            " font-weight: bold;"
                            "}";

    // Toggle us1poweron status
    us2poweron = !us2poweron;
    QString text=ui->us2onoff->text();

    if (us2poweron && text=="OFF") {
        ui->us2onoff->setText("ON");  // Turn ON
        ui->us2onoff->setStyleSheet(styleSheetOn);
        ui->us1onoff->setText("ON");  // Turn ON
        ui->us1onoff->setStyleSheet(styleSheetOn);
        us1poweron = true;
        ui->us3onoff->setText("ON");  // Turn ON
        ui->us3onoff->setStyleSheet(styleSheetOn);
        us3poweron = true;
        ui->us4onoff->setText("ON");  // Turn ON
        ui->us4onoff->setStyleSheet(styleSheetOn);
        us4poweron = true;
        ui->us1powup_but->setEnabled(true);
        ui->us1powdown_but->setEnabled(true);
        ui->us2powup_but->setEnabled(true);
        ui->us2powdown_but->setEnabled(true);
        ui->us3powup_but->setEnabled(true);
        ui->us3powdown_but->setEnabled(true);
        ui->us4powup_but->setEnabled(true);
        ui->us4powdown_but->setEnabled(true);
        ui->vitpowup_but->setEnabled(true);
        ui->vitpowdown_but->setEnabled(true);

    } else {
        ui->us2onoff->setText("OFF");  // Turn OFF
        ui->us2onoff->setStyleSheet(styleSheetOff);
        ui->us1onoff->setText("OFF");  // Turn OFF
        ui->us1onoff->setStyleSheet(styleSheetOff);
        us1poweron = false;
        ui->us3onoff->setText("OFF");  // Turn OFF
        ui->us3onoff->setStyleSheet(styleSheetOff);
        us3poweron = false;
        ui->us4onoff->setText("OFF");  // Turn OFF
        ui->us4onoff->setStyleSheet(styleSheetOff);
        us4poweron = false;
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

    }
}


void MainWindow::on_us3onoff_clicked()
{

    QString styleSheetOn = "QPushButton {"
                           " font:20pt Ubuntu;"
                           " background-color: green;"
                           " color: black;"
                           " border:5px solid black;"
                           " border-radius:30px;"
                           " font-weight: bold;"
                           "}";

    QString styleSheetOff = "QPushButton {"
                            " font:20pt Ubuntu;"
                            " background-color: rgb(224, 27, 36);"
                            " color: black;"
                            " border:5px solid black;"
                            " border-radius:30px;"
                            " font-weight: bold;"
                            "}";

    // Toggle us1poweron status
    us3poweron = !us3poweron;
    QString text=ui->us3onoff->text();

    if (us3poweron && text=="OFF") {
        ui->us3onoff->setText("ON");  // Turn ON
        ui->us3onoff->setStyleSheet(styleSheetOn);
        ui->us1onoff->setText("ON");  // Turn ON
        ui->us1onoff->setStyleSheet(styleSheetOn);
        us1poweron = true;
        ui->us2onoff->setText("ON");  // Turn ON
        ui->us2onoff->setStyleSheet(styleSheetOn);
        us2poweron = true;
        ui->us4onoff->setText("ON");  // Turn ON
        ui->us4onoff->setStyleSheet(styleSheetOn);
        us4poweron = true;
        ui->us1powup_but->setEnabled(true);
        ui->us1powdown_but->setEnabled(true);
        ui->us2powup_but->setEnabled(true);
        ui->us2powdown_but->setEnabled(true);
        ui->us3powup_but->setEnabled(true);
        ui->us3powdown_but->setEnabled(true);
        ui->us4powup_but->setEnabled(true);
        ui->us4powdown_but->setEnabled(true);
        ui->vitpowup_but->setEnabled(true);
        ui->vitpowdown_but->setEnabled(true);

    } else {
        ui->us3onoff->setText("OFF");  // Turn OFF
        ui->us3onoff->setStyleSheet(styleSheetOff);
        ui->us1onoff->setText("OFF");  // Turn OFF
        ui->us1onoff->setStyleSheet(styleSheetOff);
        us1poweron = false;
        ui->us2onoff->setText("OFF");  // Turn OFF
        ui->us2onoff->setStyleSheet(styleSheetOff);
        us2poweron = false;
        ui->us4onoff->setText("OFF");  // Turn OFF
        ui->us4onoff->setStyleSheet(styleSheetOff);
        us4poweron = false;
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

    }
}

void MainWindow::on_us4onoff_clicked()
{



    QString styleSheetOn = "QPushButton {"
                           " font:20pt Ubuntu;"
                           " background-color: green;"
                           " color: black;"
                           " border:5px solid black;"
                           " border-radius:30px;"
                           " font-weight: bold;"
                           "}";

    QString styleSheetOff = "QPushButton {"
                            " font:20pt Ubuntu;"
                            " background-color: rgb(224, 27, 36);"
                            " color: black;"
                            " border:5px solid black;"
                            " border-radius:30px;"
                            " font-weight: bold;"
                            "}";

    // Toggle us1poweron status
    us4poweron = !us4poweron;
    QString text=ui->us4onoff->text();

    if (us4poweron && text=="OFF") {
        ui->us4onoff->setText("ON");  // Turn ON
        ui->us4onoff->setStyleSheet(styleSheetOn);
        ui->us1onoff->setText("ON");  // Turn ON
        ui->us1onoff->setStyleSheet(styleSheetOn);
        us1poweron = true;
        ui->us3onoff->setText("ON");  // Turn ON
        ui->us3onoff->setStyleSheet(styleSheetOn);
        us3poweron = true;
        ui->us2onoff->setText("ON");  // Turn ON
        ui->us2onoff->setStyleSheet(styleSheetOn);
        us2poweron = true;
        ui->us1powup_but->setEnabled(true);
        ui->us1powdown_but->setEnabled(true);
        ui->us2powup_but->setEnabled(true);
        ui->us2powdown_but->setEnabled(true);
        ui->us3powup_but->setEnabled(true);
        ui->us3powdown_but->setEnabled(true);
        ui->us4powup_but->setEnabled(true);
        ui->us4powdown_but->setEnabled(true);
        ui->vitpowup_but->setEnabled(true);
        ui->vitpowdown_but->setEnabled(true);

    } else {
        ui->us4onoff->setText("OFF");  // Turn OFF
        ui->us4onoff->setStyleSheet(styleSheetOff);
        ui->us1onoff->setText("OFF");  // Turn OFF
        ui->us1onoff->setStyleSheet(styleSheetOff);
        us1poweron = false;
        ui->us3onoff->setText("OFF");  // Turn OFF
        ui->us3onoff->setStyleSheet(styleSheetOff);
        us3poweron = false;
        ui->us2onoff->setText("OFF");  // Turn OFF
        ui->us2onoff->setStyleSheet(styleSheetOff);
        //us2spoweron = false;
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
        ui->vitonoff->setText("OFF");
        ui->vitonoff->setStyleSheet(styleSheet2);
        ui->vitpowup_but->setEnabled(false);
        ui->vitpowdown_but->setEnabled(false);
        handler->vit_off();


    } else {
        ui->vitonoff->setText("ON");
        ui->vitonoff->setStyleSheet(styleSheet1);
        ui->vitpowup_but->setEnabled(true);
        ui->vitpowdown_but->setEnabled(true);
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



int MainWindow::getvalue(int input)
{
    if(input<=60){
        return 60;
    }else{
        return ((input-1)/60+1)*60;
    }
}


//void MainWindow::on_CI4_2_clicked()
//{
//    QString styleSheet3 = "QPushButton {"
//           " font:20pt Ubuntu;"
//          " background-color: green;"
//            "color: black;"
//      "image: url(:/images/ci.png);"
//           " border:5px solid black;"
//           " border-radius:30px;"
//            "font-weight: bold;"
//                                 "}";
//    QString styleSheet4 = "QPushButton {"
//           " font:20pt Ubuntu;"
//          " background-color: red;"

//            "image: url(:/images/ci.png);"
//           " border:5px solid black;"
//           " border-radius:30px;"
//            "font-weight: bold;"
//                                 "}";
//    if(!overallci){
//        ui->CI5_5->setStyleSheet(styleSheet3);

//            handler->pinchvalve_on();
//        handler->speaker_on(0,0,1,0);
//    }
//    else{
//    ui->CI5_5->setStyleSheet(styleSheet4);
//        handler->pinchvalve_off();
//        handler->speaker_off();
//    }
//    overallci=!overallci;
//}

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

        ui->lblus1flow->show();

        //us2
        ui->label_91->show();
        ui->lineEdit_59->show();
        ui->us2flowup_but->show();
        ui->us2flowdown_but->show();

        ui->lblus2flow->show();
        //us3
         ui->label_96->show();
        ui->lineEdit_62->show();
        ui->us3flowup_but->show();
        ui->us3flowdown_but->show();

        ui->lblus3flow->show();

        //us4
        ui->label_103->show();
        ui->lineEdit_65->show();
        ui->us4flowup_but->show();
        ui->us4flowdown_but->show();

        ui->lblus4flow->show();

        //ia 1
        ui->label_112->show();
        ui->lineEdit_69->show();
        ui->ia1flowup_but->show();
        ui->ia1flowdown_but->show();

        ui->lblia1flow->show();

        //ia2
        ui->label_108->show();
        ui->lineEdit_67->show();
        ui->ia2flowup_but->show();
        ui->ia2flowdown_but->show();

        ui->lblia2flow->show();

        //vit
        ui->label_122->show();
        ui->lineEdit_72->show();
        ui->vitflowup_but->show();
        ui->vitflowdown_but->show();

        ui->lblvitflow->show();


    }
    else if(value == "Venturi"){
       // ui->pushButton_2->setStyleSheet("background-color:green;");
        //us1
        ui->label_87->hide();
        ui->lineEdit_56->hide();
        ui->us1flowup_but->hide();
        ui->us1flowdown_but->hide();

        ui->lblus1flow->hide();
        //us2
        ui->label_91->hide();
        ui->lineEdit_59->hide();
        ui->us2flowup_but->hide();
        ui->us2flowdown_but->hide();

        ui->lblus2flow->hide();
        //us3
         ui->label_96->hide();
        ui->lineEdit_62->hide();
        ui->us3flowup_but->hide();
        ui->us3flowdown_but->hide();

        ui->lblus3flow->hide();
        //us4
        ui->label_103->hide();
        ui->lineEdit_65->hide();
        ui->us4flowup_but->hide();
        ui->us4flowdown_but->hide();

        ui->lblus4flow->hide();
        //ia 1
        ui->label_112->hide();
        ui->lineEdit_69->hide();
        ui->ia1flowup_but->hide();
        ui->ia1flowdown_but->hide();

        ui->lblia1flow->hide();
        //ia2
        ui->label_108->hide();
        ui->lineEdit_67->hide();
        ui->ia2flowup_but->hide();
        ui->ia2flowdown_but->hide();

        ui->lblia1flow->hide();
        //vit
        ui->label_122->hide();
        ui->lineEdit_72->hide();
        ui->vitflowup_but->hide();
        ui->vitflowdown_but->hide();

        ui->lblvitflow->hide();
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
      ui->vitvacmode->setText(surgicalData.vitvacmode);
      ui->ia2mode->setText(surgicalData.ia1mode);
      ui->ia1mode->setText(surgicalData.ia2mode);
      //qDebug()<<surgicalData.ia1asp<<surgicalData.ia2asp<<surgicalData.ia1vac<<surgicalData.ia2vac<<"these are sended from the doctor windiow";
      if(disablelayout == "DIATHERMY" || disablelayout == "IRRIGATION1" || disablelayout == "IRRIGATION2" || disablelayout == "VITRECTOMY"){
          ui->label_32->hide();

      }if(disablelayout == "ULTRASONICBUT1" ||disablelayout == "ULTRASONICBUT2"||disablelayout == "ULTRASONICBUT3"||disablelayout == "ULTRASONICBUT4"){
          ui->label_32->show();
      }

}

void MainWindow::on_SETTINGS_BUT_2_clicked()
{
    //qDebug() << "Disabling functions.";
    handler->buzz();
    motoroff();
    stopPhacoTracking();
    handler->phaco_off();
        handler->fs_count_limit(nfpzero+nfpone+nfptwo);

    handler->freq_count(0);
    handler->phaco_power(0);
    //handler->convert_dac(0x30, 0);
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
    if (ui->tabWidget->currentIndex() == 7) { // Check if in Diathermy tab
        ui->label_32->hide();
    }
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

void MainWindow::rx_speakeronoff(const QString &text)
{
    speakeronoff =text;
}

int MainWindow::
readsensorvalue()
{
    int sensorvalue=vac->convert(0XD7);
    int actualsensor=sensorvalue*0.17;
    return actualsensor;
    //qDebug()<<actualsensor;

}

void MainWindow::push(const QString &surgeonName) {
    qDebug() << "Attempting to fetch data for surgeon:" << surgeonName;
    emit sendsurgeon(surgeonName);

    QString connName = "push_conn_" + QString::number(QDateTime::currentMSecsSinceEpoch());

    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
        db.setDatabaseName(PATH);

        if (!db.open()) {
            qDebug() << "Failed to open database. Error:" << db.lastError().text();
            QSqlDatabase::removeDatabase(connName);
            return;
        }

        QSqlQuery query(db);
        query.prepare(
            "SELECT diapowmax, pump, Swaponoff, viberationoff, speakeronoff, Epinaspmax, Epinvacmax, Epinpowmax, "
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

        if (!query.exec()) {
            qDebug() << "Failed to execute query. Error:" << query.lastError().text();
            db.close();
            QSqlDatabase::removeDatabase(connName);
            return;
        }

        if (query.next()) {
            // Your existing code to update UI with query results
            int phacoPowerMax = query.value("diapowmax").toInt();
            QString pump = query.value("pump").toString();
            QString vibeonoff = query.value("viberationoff").toString();
            QString speaker_onoff = query.value("speakeronoff").toString();
            QString swap = query.value("Swaponoff").toString();

            // Update all your UI elements here (keep your existing UI update code)
            ui->lineEdit_74->setText(QString::number(phacoPowerMax));
            ui->comboBox->setCurrentText(pump);

            // ... rest of your UI update code ...

            int nfpo = query.value("fzero").toInt();
            int nfp1 = query.value("fone").toInt();
            int nfp2 = query.value("ftwo").toInt();
            int nfp3 = query.value("fthree").toInt();

            nfpzero = static_cast<int>((nfpo / 100.0) * 4090);
            nfpone = static_cast<int>((nfp1 / 100.0) * 4090);
            nfptwo = static_cast<int>((nfp2 / 100.0) * 4090);
            nfpthree = static_cast<int>((nfp3 / 100.0) * 4090);

            vibon = vibeonoff;
            speakeronoff = speaker_onoff;

            // Emit signals for footpedal
            emit left_foot(query.value("footleft").toString());
            emit right_foot(query.value("footright").toString());
            emit bottom_left(query.value("footbottomleft").toString());
            emit bottom_right(query.value("footbottomright").toString());

            performpump(pump);
        }

        db.close();
    }

    QSqlDatabase::removeDatabase(connName);
}

void MainWindow::on_pushButton_42_clicked()
{
   // qDebug() << "Disabling functions.";
    motoroff();
    stopPhacoTracking();
    handler->phaco_off();
        handler->fs_count_limit(nfpzero+nfpone+nfptwo);

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
    qDebug() << "Handpiece button clicked - going to tune page";

    // Stop all hardware
    motoroff();
    stopPhacoTracking();
    if (handler) {
        handler->phaco_off();
        handler->freq_count(0);
        handler->phaco_power(0);
        //handler->convert_dac(0x30, 0);
        handler->pinchvalve_off();
        handler->safetyvent_off();
        handler->vit_off();
    }

    // Stop timers
    if (protimer)      protimer->stop();
    if (Tacutalsensor) Tacutalsensor->stop();

    // Hide MainWindow, show prime (which shows the tune tab)
    this->hide();

    // Signal prime to switch to tune tab
    emit emittuning();
}


void MainWindow::on_ocuburstdown_but_clicked()
{
    nOcuPulseCount = ui->lineEdit_77->text().toInt();
     nOcuPulseCount -= 1;
    if ( nOcuPulseCount <1) {
         nOcuPulseCount = 1;
    }
    ui->lineEdit_77->setText(QString::number( nOcuPulseCount));
    handler->freq_count(nFreqCount);
   // handler->fs_count(nfpzero+nfpone+nfptwo);
   // handler->burst_length(nOcuBurstCount);
     handler->pdm_mode(CONTINOUS);
    // qDebug()<<nOcuBurstCount;
}

void MainWindow::on_ocuburstup_but_clicked()
{
    nOcuPulseCount = ui->lineEdit_77->text().toInt();
     nOcuPulseCount += 1;
    if ( nOcuPulseCount > 15) {
         nOcuPulseCount = 15;
    }
    ui->lineEdit_77->setText(QString::number( nOcuPulseCount));
    handler->freq_count(nFreqCount);
    //handler->fs_count(nfpzero+nfpone+nfptwo);
   // handler->burst_length(nOcuBurstCount);
    handler->pdm_mode(CONTINOUS);
   // qDebug()<<nOcuBurstCount;
}

void MainWindow::on_ocupulsedown_but_clicked()
{
    nOcuBurstCount = ui->lineEdit_76->text().toInt();
    nOcuBurstCount -= 40;
    if (nOcuBurstCount < 40) {
        nOcuBurstCount = 40;
    }
    ui->lineEdit_76->setText(QString::number(nOcuBurstCount));
    handler->freq_count(nFreqCount);
   // handler->fs_count(nfpzero+nfpone+nfptwo);
    handler->pdm_mode(CONTINOUS);
}

void MainWindow::on_ocupulseup_but_clicked()
{
    nOcuBurstCount = ui->lineEdit_76->text().toInt();
    nOcuBurstCount += 40;
    if (nOcuBurstCount > 400) {
        nOcuBurstCount = 400;
    }
    ui->lineEdit_76->setText(QString::number(nOcuBurstCount));

    handler->freq_count(nFreqCount);
   // handler->fs_count(nfpzero+nfpone+nfptwo);
    handler->pdm_mode(CONTINOUS);

}

void MainWindow::on_clickedclear()
{
    QList<QLineEdit*> allEdits = {
        ui->lineEdit_57, ui->lineEdit_55, ui->lineEdit_56,
        ui->lineEdit_58, ui->lineEdit_60, ui->lineEdit_59,
        ui->lineEdit_61, ui->lineEdit_63, ui->lineEdit_62,
        ui->lineEdit_64, ui->lineEdit_66, ui->lineEdit_65,
        ui->lineEdit_70, ui->lineEdit_69, ui->lineEdit_68,
        ui->lineEdit_67, ui->lineEdit_71, ui->lineEdit_73,
        ui->lineEdit_72, ui->lineEdit_74
    };

    for (QLineEdit *le : allEdits) {
        if (le->hasFocus()) {
            le->clear();
            break;
        }
    }
}

void MainWindow::startPhacoTracking(float powerPct)
{
    if (m_phacoFiring)
        return;                        // already tracking – avoid double-start

    m_currentPowerPct = powerPct;
    m_phacoSegTimer.start();           // restart segment timer
    m_phacoFiring    = true;
}

// Call this AFTER every handler->phaco_off()
void MainWindow::stopPhacoTracking()
{
    if (!m_phacoFiring)
        return;                        // not firing – nothing to accumulate

    qint64 burstMs = m_phacoSegTimer.elapsed();

    // Commit this burst's EPT
    m_eptTotalMs += burstMs;

    // Commit this burst's USD
    m_usdTotal += static_cast<double>(m_currentPowerPct)
                  * (static_cast<double>(burstMs) / 1000.0);

    m_phacoFiring = false;
}

// Call this at the start of each new surgery case
void MainWindow::resetEptUsd()
{
    m_phacoFiring     = false;
    m_eptTotalMs      = 0;
    m_usdTotal        = 0.0;
    m_currentPowerPct = 0.0f;

    ui->elapsed_time->setText("00:00");
    ui->elapsed_time_2->setText("0.0");
}
