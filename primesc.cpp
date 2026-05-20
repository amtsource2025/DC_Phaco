#include "primesc.h"
#include "ui_primesc.h"
#include <QMessageBox>
#include<QTimer>
#include<QFile>
#include<QImage>
#include<QDebug>
#include<QIODevice>
primesc::primesc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::primesc)
{
    ui->setupUi(this);
    move(400,400);
    m=new MainWindow;
    sur=new doctor;
    set=new settings;
    ui->progressBar_2->setRange(0,100);
    QString styleSheet = "QPushButton {"
                                 "    font-family: Ubuntu;"
                                 "    font-size: 20pt;"
                                 "    background-color: white;"
                                 "color:black;"
                                 "    border-radius: 20px;" // Adjust the radius as needed
                                 "}";

    QString tabStyle = "QTabBar::tab:selected { background-color: black; color: #ffffff; border-radius:50px}";
    QString styleSheet3 =
                "QTabBar::tab {"
                "    font-size: 16px; /* Set the font size */"
                "    border-radius: 10px; /* Set the border radius */"
                "}";
ui->tabWidget->setStyleSheet(styleSheet3);

     ui-> tabWidget->setStyleSheet(tabStyle);
    ui->tabWidget->setTabText(0 ,"     primesc        ");
    ui->tabWidget->setTabText(1 ,"     TUNE         ");
    ui->tabWidget->setTabText(2 ,"     CLEAN        ");



   connect(ui->prime1_but,&QPushButton::clicked,this,&primesc::prime);

   connect(ui->Tune_but,&QPushButton::clicked,this,&primesc::Tune);

   connect(ui->clean_but,&QPushButton::clicked,this,&primesc::Clean);
   connect(ui->Start_tune_2,&QPushButton::clicked,this,&primesc::Start_Tune);
   connect(ui->prime1_but, &QPushButton::clicked, [=](){
       ui->prime1_but->setStyleSheet(styleSheet);

   });
   connect(ui->Tune_but, &QPushButton::clicked, [=](){
       ui->Tune_but->setStyleSheet(styleSheet);

   });
  connect(ui->clean_but, &QPushButton::clicked, [=](){
      ui->clean_but->setStyleSheet(styleSheet);

  });

//  connect(ui->primesc1_but,&QPushButton::clicked,this,&MainWindow::current);


   connect(ui->start_check_2,&QCheckBox::toggled,this,&primesc::update);
   connect(ui->wait_Check_2,&QCheckBox::toggled,this,&primesc::update);
   connect(ui->motor_Check_2,&QCheckBox::toggled,this,&primesc::update);
   connect(ui->done_Check_2,&QCheckBox::toggled,this,&primesc::update);
  // connect(ui->pushButton_4,&QPushButton::clicked,this,&primesc::click);


   //ui->progressBar_2->->setMinimum(0);
   //ui->progressBar_2->->setMaximum(100);
    timer1=new QTimer;
    connect(timer1,&QTimer::timeout,this,&primesc::timer);
    timer1->start(50);
    value=0;
connect(ui->tabWidget, &QTabWidget::currentChanged, this, &primesc::current);
     current(ui->tabWidget->currentIndex());
     ui->tick_lab_2->close();

}

primesc::~primesc()
{
    delete ui;
}
void primesc::click()
{
    QString styleSheet = "QPushButton {"
                                 "    font-family: Ubuntu;"
                                 "    font-size: 20pt;"
                                 "    background-color: black;"
            "color:white;"
                                 "    border-radius: 20px;" // Adjust the radius as needed
                                 "}";

   ui->prime1_but->setStyleSheet(styleSheet);
   ui->Tune_but->setStyleSheet(styleSheet);
   ui->clean_but->setStyleSheet(styleSheet);


}
void primesc::update()
{
    if (ui->start_check_2->isChecked()) {
        ui-> progressBar_2->setValue(val1+25); // Example value, adjust as needed
    }
    else{
        ui->progressBar_2->setValue(val1);
    }

  if (ui->wait_Check_2->isChecked()) {
         ui-> progressBar_2->setValue(val1+50);  // Example value, adjust as needed
    }
  else{
      ui->progressBar_2->setValue(val1-25);
  }
  if (ui->motor_Check_2->isChecked()) {
         ui-> progressBar_2->setValue(val1+75);  // Example value, adjust as needed
    }
  else{
      ui->progressBar_2->setValue(val1-25);
  }
  if (ui->done_Check_2->isChecked()) {
         ui-> progressBar_2->setValue(val1+100);  // Example value, adjust as needed
    }
  else{
      ui->progressBar_2->setValue(val1-25);
  }
}

void primesc::hp(int connected)
{
    if(!connected){
       // ui->label_2->show();

    }else{
         //  ui->label_2->setStyleSheet("image: url(:/connected.png);");
    }
}



void primesc::timer(){

    QString histogramText;
    for (int i = 0; i < value; ++i) {
        histogramText += "|";
//        histogramText +="<font color='white'>|</font>";
//        histogramText +="<b>|<b>;";
       // ui->label_4->setStyleSheet("color:white");
    }
    ui->progress_lab_2->setText(histogramText);


    ui->tick_lab_2->setStyleSheet("font-size:20px;");

    //ui->label_4->setStyleSheet("background-color:white;");
    ui->tick_lab_2->show();

    // Increment value and reset if it exceeds a certain limit
    value++;
    if (value > 100){
        ui->tick_lab_2->show();
         ui->Start_tune_2->setText("Tuning....");
        timer1->stop();
         ui->tick_lab_2->setStyleSheet("image: url(:/tick2.png);");

        value = 0;
//    QPixmap pixmap("image: url(:/tick2.png);");
//    ui->label_6->setPixmap(pixmap);


    }

}

void primesc::current(int tab)
{
    QString styleSheet = "QPushButton {"
                                 "    font-family: Ubuntu;"
                                 "    font-size: 20pt;"
                                 "    background-color: white;"
                                 "color:black;"

                                 "    border-radius: 20px;" // Adjust the radius as needed
                                 "}";
    QString styleSheet1 = "QPushButton {"
                                 "    font-family: Ubuntu;"
                                 "    font-size: 20pt;"
                                 "    background-color: black;"
                                "color:white;"
                                 "    border-radius: 20px;"
            "font:bold;"// Adjust the radius as needed
                                 "}";
    if(tab!=0){
        ui->prime1_but->setStyleSheet(styleSheet1);}
   if(tab!=1){
       ui->Tune_but->setStyleSheet(styleSheet1);}
   if(tab!=2){
       ui->clean_but->setStyleSheet(styleSheet1);}
    if (tab == 0) {
        ui->prime1_but->setStyleSheet(styleSheet);

    } else if (tab == 1) {
        ui->Tune_but->setStyleSheet(styleSheet);

    } else if (tab == 2) {
        ui->clean_but->setStyleSheet(styleSheet);
    }


}
void primesc::prime()
{

//QString styleSheet1 = "QPushButton {"
//    "    font-family: Ubuntu;"
//    "    font-size: 20pt;"
//    "    background-color: black;"
//    "color:white;"
//        "font:bold";
//   "    border-radius: 20px;" // Adjust the radius as needed
//    "}";
    ui->tabWidget->setCurrentIndex(0);


    click();
    timer1->stop();
}

void primesc::Tune()
{

    ui->tabWidget->setCurrentIndex(1);

    click();
    timer1->start(50);
    timer();
}

void primesc::Clean()
{

    ui->tabWidget->setCurrentIndex(2);
   // ui->pushButton_9->setText("CLEAN");
    //ui->pushButton_9->setStyleSheet(styleSheet1);
    click();
    timer1->stop();
}


void primesc::Start_Tune()
{



   // m->show();


}

void primesc::on_pushButton_clicked()
{

 //   m->show();
}


void primesc::on_pushButton_2_clicked()
{
    set->show();
}

void primesc::on_pushButton_3_clicked()
{
    QString styleSheet = "QPushButton {"
                                 "    font-family: Ubuntu;"
                                 "    font-size: 20pt;"
                                 "    background-color: black;"
                                 "color:white;"

                                 "    border-radius: 20px;" // Adjust the radius as needed
                                 "}";


    Tune();
    ui->Tune_but->setStyleSheet(styleSheet);
}




void primesc::on_pushButton_4_clicked()
{
    QString styleSheet = "QPushButton {"
                                 "    font-family: Ubuntu;"
                                 "    font-size: 20pt;"
                                 "    background-color: black;"
                                 "color:white;"

                                 "    border-radius: 20px;" // Adjust the radius as needed
                                 "}";


    primesc();
    ui->prime1_but->setStyleSheet(styleSheet);

}
