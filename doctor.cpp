#include "doctor.h"
#include "ui_doctor.h"
#include<QMessageBox>
#include<QTimer>
#include<QSqlError>
#include<QMap>
doctor::doctor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::doctor),
    spealer_onoff(false)
{
    ui->setupUi(this);
    QFile hpFile("/home/tune/handpiece_no.txt");
        if (hpFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&hpFile);
            QString savedHp = in.readLine().trimmed();
            hpFile.close();
            if (!savedHp.isEmpty()) {
                ui->lineEdit_hp->setText(savedHp);
            }
        }
    hand=new hwhandler;
    hand->speaker_off();

    //last surgeon update
   setLastSelectedValue();
   move(0,0);
  cFoot=new footswitch;
    QString tabStyle = "QTabBar::tab:selected { background-color: black; color: #ffffff; }";
    ui->tabWidget_2->setStyleSheet(tabStyle);
         connect(ui->tabWidget_2, &QTabWidget::currentChanged, this, &doctor::clickedtab);
          clickedtab(ui->tabWidget_2->currentIndex());
//index updated based on the surgeon
connect(ui->SelectSurgeon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &doctor::onComboBoxIndexChanged);
//select surgeon on combo box changed
connect(ui->SelectSurgeon,&QComboBox::currentTextChanged,this,&doctor::onSurgeonSelectionChanged);

       //current surgeon

    QString tabStyle1 = "QTabBar::tab:selected { background-color: black; color: #ffffff; }";
     ui-> tabWidget->setStyleSheet(tabStyle1);

    key=new keypad;
    connect(key,&keypad::textsignal,this,&doctor::on_clicked);
    connect(key,&keypad::entersignal,this,&doctor::on_clickedenter);
    connect(key, &keypad::clearsignal, this, &doctor::on_clickedclear);  // ← ADD
    //pump selection
    connect(ui->comboBox,&QComboBox::currentTextChanged, this, &doctor::pumpvalue);
    //lineEdit preset values
     ui->lineEdit_6->setText("100");
  ui->lineEdit_2->setText("40");
  ui->lineEdit_3->setText("500");
  ui->lineEdit_4->setText("100");
  ui->lineEdit_7->setText("500");
  ui->lineEdit_5->setText("40");
  ui->lineEdit_11->setText("500");
  ui->lineEdit_12->setText("40");
  ui->lineEdit->setText("100");
  ui->lineEdit_19->setText("500");

  // Set default values to UI elements
  ui->lineEdit_8->setText("22");
  ui->lineEdit_9->setText("200");
  ui->lineEdit_10->setText("50");

  ui->lineEdit_13->setText("26");
    ui->lineEdit_14->setText("200");
    ui->lineEdit_15->setText("60");

  ui->lineEdit_16->setText("16");
     ui->lineEdit_17->setText("150");
     ui->lineEdit_18->setText("70");

    ui->lineEdit_6->installEventFilter(this);
    //phaco
    ui->lineEdit_2->installEventFilter(this);
    ui->lineEdit_3->installEventFilter(this);
    ui->lineEdit_4->installEventFilter(this);
    //quad

    ui->lineEdit_10->installEventFilter(this);
    ui->lineEdit_9->installEventFilter(this);
    ui->lineEdit_8->installEventFilter(this);

    //chop
    ui->lineEdit_13->installEventFilter(this);
    ui->lineEdit_14->installEventFilter(this);
    ui->lineEdit_15->installEventFilter(this);

    //sculpt

    ui->lineEdit_16->installEventFilter(this);
    ui->lineEdit_17->installEventFilter(this);
    ui->lineEdit_18->installEventFilter(this);



    //ia
    ui->lineEdit_5->installEventFilter(this);
    ui->lineEdit_7->installEventFilter(this);
    ui->lineEdit_11->installEventFilter(this);
    ui->lineEdit_12->installEventFilter(this);
    //vit
    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_19->installEventFilter(this);
    ui->lineEdit_20->installEventFilter(this);
    ui->lineEdit_hp->installEventFilter(this);
//PROGRESSBAR SETTINGS
  ui->progressBar->setRange(5,100);
    ui->progressBar_21->setMinimum(60);
  ui->progressBar_21->setMaximum(2520);
  ui->progressBar_5->setRange(2,40);
  ui->progressBar_6->setRange(5,500);
  ui->progressBar_7->setRange(5,100);
  ui->progressBar_9->setRange(5,500);
  ui->progressBar_10->setRange(5,100);
  ui->progressBar_8->setRange(2,40);
  ui->progressBar_11->setRange(2,40);
  ui->progressBar_12->setRange(5,500);
  ui->progressBar_13->setRange(5,100);
  ui->progressBar_2->setRange(2,40);
  ui->progressBar_3->setRange(5,500);
  ui->progressBar_4->setRange(5,100);
  ui->progressBar_14->setRange(5,500);
  ui->progressBar_15->setRange(2,40);
  ui->progressBar_16->setRange(5,500);
  ui->progressBar_17->setRange(2,40);
  ui->progressBar_19->setRange(5,500);
  ui->progressBar_20->setRange(2,40);
  ui->progressBar_21->setRange(60,2520);

  connect(ui->DiaBut,&QPushButton::clicked,this,&doctor::DiathermyBut);
  connect(ui->PhacoBut,&QPushButton::clicked,this,&doctor::PhacoBut);
  connect(ui->IABut,&QPushButton::clicked,this,&doctor::IrrigationAspirationBut);
  connect(ui->VitBut,&QPushButton::clicked,this,&doctor::VitrectomyBut);


      connect(ui->BackBut,&QPushButton::clicked,this,&doctor::BackBut);

      ui->tabWidget->setTabText(0,"    DIA");
    ui->tabWidget->setTabText(1,"    PHACO");
    ui->tabWidget->setTabText(2,"    IA");
    ui->tabWidget->setTabText(3,"    VIT    ");

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &doctor::click);
     click(ui->tabWidget->currentIndex());
     onSurgeonSelectionChanged(ui->SelectSurgeon->currentText());
     QString text=ui->SelectSurgeon->currentText();
     emit surgeonNamefoot(text);
     connect(this,&doctor::surgeonNamefoot,cFoot,&footswitch::SurgeonFoot);
     connect(cFoot,&footswitch::sendvaltomain,this,&doctor::receivingvalueffs);
     //connecting combobox from footswitch to doctor window
     connect(cFoot,&footswitch::topleft,this,&doctor::receivetopleft);
     connect(cFoot,&footswitch::topright,this,&doctor::receivetopright);
     connect(cFoot,&footswitch::bottomleft,this,&doctor::receivebottomleft);
     connect(cFoot,&footswitch::bottomright,this,&doctor::receivebottomright);
     connect(this,&doctor::txfootpedalvalues,cFoot,&footswitch::rxfootpedalmodes);
     //qDebug()<<"the doctor window is started";
}

doctor::~doctor()
{
    delete ui;
    db.close();
}

bool doctor::eventFilter(QObject *object, QEvent *event)
{
    //diathermy
    if(object == ui->lineEdit_6 && event->type() == QEvent::MouseButtonPress) {
      QMouseEvent *k = static_cast<QMouseEvent *> (event);
      if( k->button() == Qt::LeftButton ) {
          key->close();
          key->resize(401,291);
          key->move(280,320);
          key->show();

          ui->lineEdit_6->setFocus();
          ui->lineEdit_6->setText("");

      } else if ( k->button() == Qt::RightButton ) {
      }
      //phaco
}
    if(object == ui->lineEdit_2 && event->type() == QEvent::MouseButtonPress) {
      QMouseEvent *k = static_cast<QMouseEvent *> (event);
      if( k->button() == Qt::LeftButton ) {
          key->close();
          key->resize(401,291);
          key->move(280,320);
          key->show();
          ui->lineEdit_2->setFocus();
          ui->lineEdit_3->clearFocus();
          ui->lineEdit_4->clearFocus();
          ui->lineEdit_2->setText("");

      } else if ( k->button() == Qt::RightButton ) {
      }
}
    if(object == ui->lineEdit_3 && event->type() == QEvent::MouseButtonPress) {
      QMouseEvent *k = static_cast<QMouseEvent *> (event);
      if( k->button() == Qt::LeftButton ) {
          key->close();
          key->resize(401,291);
          key->move(280,320);
          key->show();
          ui->lineEdit_3->setFocus();
          ui->lineEdit_2->clearFocus();
          ui->lineEdit_4->clearFocus();
          ui->lineEdit_3->setText("");

      } else if ( k->button() == Qt::RightButton ) {
      }
}
    if(object == ui->lineEdit_4 && event->type() == QEvent::MouseButtonPress) {
      QMouseEvent *k = static_cast<QMouseEvent *> (event);
      if( k->button() == Qt::LeftButton ) {
          key->close();
          key->resize(401,291);
          key->move(280,320);
          key->show();
          ui->lineEdit_4->setFocus();
          ui->lineEdit_3->clearFocus();
          ui->lineEdit_2->clearFocus();
          ui->lineEdit_4->setText("");

      } else if ( k->button() == Qt::RightButton ) {
      }
    }
      //quad
      if(object == ui->lineEdit_10 && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->close();
            key->resize(401,291);
            key->move(280,320);
            key->show();
            ui->lineEdit_10->setFocus();
            ui->lineEdit_8->clearFocus();
            ui->lineEdit_9->clearFocus();
            ui->lineEdit_10->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }
  }
      if(object == ui->lineEdit_8 && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->close();
            key->resize(401,291);
            key->move(280,320);
            key->show();
            ui->lineEdit_8->setFocus();
            ui->lineEdit_10->clearFocus();
            ui->lineEdit_9->clearFocus();
            ui->lineEdit_8->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }
  }
      if(object == ui->lineEdit_9 && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->close();
            key->resize(401,291);
            key->move(280,320);
            key->show();
            ui->lineEdit_9->setFocus();
            ui->lineEdit_10->clearFocus();
            ui->lineEdit_8->clearFocus();
            ui->lineEdit_9->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }

}
      //chop
      if(object == ui->lineEdit_13 && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->close();
            key->resize(401,291);
            key->move(280,320);
            key->show();
            ui->lineEdit_13->setFocus();
            ui->lineEdit_14->clearFocus();
            ui->lineEdit_15->clearFocus();
            ui->lineEdit_13->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }
  }
      if(object == ui->lineEdit_14 && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->close();
            key->resize(401,291);
            key->move(280,320);
            key->show();
            ui->lineEdit_14->setFocus();
            ui->lineEdit_13->clearFocus();
            ui->lineEdit_15->clearFocus();
            ui->lineEdit_14->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }
  }
      if(object == ui->lineEdit_15 && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->close();
            key->resize(401,291);
            key->move(280,320);
            key->show();
            ui->lineEdit_15->setFocus();
            ui->lineEdit_14->clearFocus();
            ui->lineEdit_13->clearFocus();
            ui->lineEdit_15->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }

}
      //sculpt
      if(object == ui->lineEdit_16 && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->close();
            key->resize(401,291);
            key->move(280,320);
            key->show();
            ui->lineEdit_16->setFocus();
            ui->lineEdit_17->clearFocus();
            ui->lineEdit_18->clearFocus();
            ui->lineEdit_16->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }
  }
      if(object == ui->lineEdit_17 && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->close();
            key->resize(401,291);
            key->move(280,320);
            key->show();
            ui->lineEdit_17->setFocus();
            ui->lineEdit_18->clearFocus();
            ui->lineEdit_16->clearFocus();
            ui->lineEdit_17->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }
  }
      if(object == ui->lineEdit_18 && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->close();
            key->resize(401,291);
            key->move(280,320);
            key->show();
            ui->lineEdit_18->setFocus();
            ui->lineEdit_17->clearFocus();
            ui->lineEdit_16->clearFocus();
            ui->lineEdit_18->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }

}

//irrigation
    if(object == ui->lineEdit_5 && event->type() == QEvent::MouseButtonPress) {
      QMouseEvent *k = static_cast<QMouseEvent *> (event);
      if( k->button() == Qt::LeftButton ) {
          key->close();
          key->resize(401,291);
          key->move(280,320);
          key->show();
          ui->lineEdit_5->setFocus();
          ui->lineEdit_7->clearFocus();
          ui->lineEdit_11->clearFocus();
          ui->lineEdit_12->clearFocus();
          ui->lineEdit_5->setText("");

      } else if ( k->button() == Qt::RightButton ) {
      }
}
    if(object == ui->lineEdit_7 && event->type() == QEvent::MouseButtonPress) {
      QMouseEvent *k = static_cast<QMouseEvent *> (event);
      if( k->button() == Qt::LeftButton ) {
          key->close();
          key->resize(401,291);
          key->move(280,320);
          key->show();
          ui->lineEdit_7->setFocus();
          ui->lineEdit_11->clearFocus();
          ui->lineEdit_5->clearFocus();
          ui->lineEdit_12->clearFocus();
          ui->lineEdit_7->setText("");

      } else if ( k->button() == Qt::RightButton ) {
      }
    }
      if(object == ui->lineEdit_12 && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->close();
            key->resize(401,291);
            key->move(280,320);
            key->show();
            ui->lineEdit_12->setFocus();
            ui->lineEdit_5->clearFocus();
            ui->lineEdit_11->clearFocus();
            ui->lineEdit_7->clearFocus();
            ui->lineEdit_12->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }
  }
      if(object == ui->lineEdit_11 && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->close();
            key->resize(401,291);
            key->move(280,320);
            key->show();
            ui->lineEdit_11->setFocus();
            ui->lineEdit_5->clearFocus();
            ui->lineEdit_12->clearFocus();
            ui->lineEdit_7->clearFocus();
            ui->lineEdit_11->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }
  }
//vitrectomy
      if(object == ui->lineEdit && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *k = static_cast<QMouseEvent *> (event);
        if( k->button() == Qt::LeftButton ) {
            key->close();
            key->resize(401,291);
            key->move(280,320);
            key->show();
            ui->lineEdit->setFocus();
            ui->lineEdit_19->clearFocus();
            ui->lineEdit_20->clearFocus();
            ui->lineEdit->setText("");

        } else if ( k->button() == Qt::RightButton ) {
        }
      }
        if(object == ui->lineEdit_19 && event->type() == QEvent::MouseButtonPress) {
          QMouseEvent *k = static_cast<QMouseEvent *> (event);
          if( k->button() == Qt::LeftButton ) {
              key->close();
              key->resize(401,291);
              key->move(280,320);
              key->show();
              ui->lineEdit_19->setFocus();
              ui->lineEdit_20->clearFocus();
              ui->lineEdit->clearFocus();
              ui->lineEdit_19->setText("");

          } else if ( k->button() == Qt::RightButton ) {
          }
    }

        if(object == ui->lineEdit_20 && event->type() == QEvent::MouseButtonPress) {
          QMouseEvent *k = static_cast<QMouseEvent *> (event);
          if( k->button() == Qt::LeftButton ) {
              key->close();
              key->resize(401,291);
              key->move(280,320);
              key->show();
              ui->lineEdit_20->setFocus();
              ui->lineEdit->clearFocus();
              ui->lineEdit_19->clearFocus();
              ui->lineEdit_20->setText("");


          } else if ( k->button() == Qt::RightButton ) {
              ui->lineEdit_20->setText("40");
          }
}
        if(object == ui->lineEdit_hp && event->type() == QEvent::MouseButtonPress) {
                  QMouseEvent *k = static_cast<QMouseEvent *>(event);
                  if(k->button() == Qt::LeftButton) {
                      key->close();
                      key->resize(401, 291);
                      key->move(280, 320);
                      key->show();
                      ui->lineEdit_hp->setFocus();
                      ui->lineEdit_hp->setText("");
                  }
                }

    return false;


}

void doctor::setRange(QLineEdit *lineEdit, int prevValue, int value, int maxValue)
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

void doctor::on_clicked(const QString& digit)
{
    //diathermy
    if(ui->lineEdit_6->focusWidget()) {
        ui->lineEdit_6->setFocus();

        int prevValue = ui->lineEdit_6->text().toInt();
        int value = (ui->lineEdit_6->text()+digit).toInt();
if(value ==0){
    ui->lineEdit_6->setText(QString::number(5));

    return;
}
        setRange(ui->lineEdit_6, prevValue, value, 100);
        ui->progressBar->setValue(value);
    }
    //phaco
     if(ui->lineEdit_2->focusWidget()) {
        ui->lineEdit_3->clearFocus();
        ui->lineEdit_4->clearFocus();
        ui->lineEdit_2->setFocus();
        int prevValue = ui->lineEdit_2->text().toInt();
        int value1 = (ui->lineEdit_2->text()+digit).toInt();
        if(value1 == 0){
            ui->lineEdit_2->setText(QString::number(2));
            return;
        }
        setRange(ui->lineEdit_2, prevValue, value1, 40);
        ui->progressBar_5->setValue(value1);

    }
    if(ui->lineEdit_3->focusWidget()) {
        ui->lineEdit_2->clearFocus();
        ui->lineEdit_4->clearFocus();
        ui->lineEdit_3->setFocus();
        int prevValue = ui->lineEdit_3->text().toInt();
        int value2 = (ui->lineEdit_3->text()+digit).toInt();
        if(value2 == 0){
            ui->lineEdit_3->setText(QString::number(5));
            return;
        }
        setRange(ui->lineEdit_3, prevValue, value2, 500);
        ui->progressBar_6->setValue(value2);


    }
     if(ui->lineEdit_4->focusWidget()) {
        ui->lineEdit_2->clearFocus();
        ui->lineEdit_3->clearFocus();
        ui->lineEdit_4->setFocus();
        int prevValue = ui->lineEdit_4->text().toInt();
        int value3 = (ui->lineEdit_4->text()+digit).toInt();
        if(value3 == 0){
            ui->lineEdit_4->setText(QString::number(5));
            return;
        }
        setRange(ui->lineEdit_4, prevValue, value3, 100);
        ui->progressBar_7->setValue(value3);


    }
     //quad
     if(ui->lineEdit_8->focusWidget()) {
        ui->lineEdit_9->clearFocus();
        ui->lineEdit_10->clearFocus();
        ui->lineEdit_8->setFocus();
        int prevValue = ui->lineEdit_8->text().toInt();
        int value1 = (ui->lineEdit_8->text()+digit).toInt();
        if(value1 == 0){
            ui->lineEdit_8->setText(QString::number(2));
            return;

        }
        setRange(ui->lineEdit_8, prevValue, value1, 40);
        ui->progressBar_8->setValue(value1);

    }
    if(ui->lineEdit_10->focusWidget()) {
        ui->lineEdit_9->clearFocus();
        ui->lineEdit_8->clearFocus();
        ui->lineEdit_10->setFocus();
        int prevValue = ui->lineEdit_10->text().toInt();
        int value2 = (ui->lineEdit_10->text()+digit).toInt();
        if(value2 ==0){
            ui->lineEdit_10->setText(QString::number(5));
            return;
        }
        setRange(ui->lineEdit_10, prevValue, value2, 100);
        ui->progressBar_10->setValue(value2);


    }
     if(ui->lineEdit_9->focusWidget()) {
        ui->lineEdit_10->clearFocus();
        ui->lineEdit_8->clearFocus();
        ui->lineEdit_9->setFocus();
        int prevValue = ui->lineEdit_9->text().toInt();
        int value3 = (ui->lineEdit_9->text()+digit).toInt();
        if(value3 == 0){
            ui->lineEdit_9->setText(QString::number(5));
            return;
        }
        setRange(ui->lineEdit_9, prevValue, value3, 500);
          ui->progressBar_9->setValue(value3);


    }
     //chop

     if(ui->lineEdit_13->focusWidget()) {
        ui->lineEdit_15->clearFocus();
        ui->lineEdit_14->clearFocus();
        ui->lineEdit_13->setFocus();
        int prevValue = ui->lineEdit_13->text().toInt();
        int value1 = (ui->lineEdit_13->text()+digit).toInt();
        if(value1 == 0){
            ui->lineEdit_13->setText(QString::number(2));
            return;
        }
        setRange(ui->lineEdit_13, prevValue, value1, 40);
          ui->progressBar_11->setValue(value1);

    }
    if(ui->lineEdit_14->focusWidget()) {
        ui->lineEdit_13->clearFocus();
        ui->lineEdit_15->clearFocus();
        ui->lineEdit_14->setFocus();
        int prevValue = ui->lineEdit_14->text().toInt();
        int value2 = (ui->lineEdit_14->text()+digit).toInt();
        if(value2 == 0){
            ui->lineEdit_14->setText(QString::number(5));
            return;
        }
        setRange(ui->lineEdit_14, prevValue, value2, 500);
          ui->progressBar_12->setValue(value2);

    }
     if(ui->lineEdit_15->focusWidget()) {
        ui->lineEdit_13->clearFocus();
        ui->lineEdit_14->clearFocus();
        ui->lineEdit_15->setFocus();
        int prevValue = ui->lineEdit_15->text().toInt();
        int value3 = (ui->lineEdit_15->text()+digit).toInt();
        if(value3 == 0){
            ui->lineEdit_15->setText(QString::number(5));
            return;
        }
        setRange(ui->lineEdit_15, prevValue, value3, 100);
          ui->progressBar_13->setValue(value3);

    }
     //sculpt
     if(ui->lineEdit_16->focusWidget()) {
        ui->lineEdit_17->clearFocus();
        ui->lineEdit_18->clearFocus();
        ui->lineEdit_16->setFocus();
        int prevValue = ui->lineEdit_16->text().toInt();
        int value1 = (ui->lineEdit_16->text()+digit).toInt();
        if(value1 == 0){
            ui->lineEdit_16->setText(QString::number(2));
            return;
        }
        setRange(ui->lineEdit_16, prevValue, value1, 40);
          ui->progressBar_2->setValue(value1);

    }
    if(ui->lineEdit_18->focusWidget()) {
        ui->lineEdit_16->clearFocus();
        ui->lineEdit_17->clearFocus();
        ui->lineEdit_18->setFocus();
        int prevValue = ui->lineEdit_18->text().toInt();
        int value2 = (ui->lineEdit_18->text()+digit).toInt();
        if(value2 == 0){
            ui->lineEdit_2->setText(QString::number(5));
            return;
        }
        setRange(ui->lineEdit_18, prevValue, value2, 100);
  ui->progressBar_4->setValue(value2);
    }
     if(ui->lineEdit_17->focusWidget()) {
        ui->lineEdit_16->clearFocus();
        ui->lineEdit_18->clearFocus();
        ui->lineEdit_17->setFocus();
        int prevValue = ui->lineEdit_17->text().toInt();
        int value3 = (ui->lineEdit_17->text()+digit).toInt();
        if(value3 == 0){
            ui->lineEdit_17->setText(QString::number(5));
            return;
        }
        setRange(ui->lineEdit_17, prevValue, value3, 500);
  ui->progressBar_3->setValue(value3);

      }



    //irrigation
   if(ui->lineEdit_5->focusWidget()) {
        ui->lineEdit_7->clearFocus();
        ui->lineEdit_11->clearFocus();
        ui->lineEdit_12->clearFocus();
        ui->lineEdit_5->setFocus();
        int prevValue = ui->lineEdit_5->text().toInt();
        int value4 = (ui->lineEdit_5->text()+digit).toInt();
        if(value4 == 0){
            ui->lineEdit_5->setText(QString::number(2));
            return;
        }
        setRange(ui->lineEdit_5, prevValue, value4, 40);
        ui->progressBar_17->setValue(value4);



    }
   if(ui->lineEdit_7->focusWidget()) {
        ui->lineEdit_5->clearFocus();
        ui->lineEdit_11->clearFocus();
        ui->lineEdit_12->clearFocus();
        ui->lineEdit_7->setFocus();
        int prevValue = ui->lineEdit_7->text().toInt();
        int value5 = (ui->lineEdit_7->text()+digit).toInt();
        if(value5 == 0){
            ui->lineEdit_7->setText(QString::number(5));
            return;
        }
        setRange(ui->lineEdit_7, prevValue, value5, 500);
        ui->progressBar_16->setValue(value5);


    }
     if(ui->lineEdit_12->focusWidget()) {
        ui->lineEdit_5->clearFocus();
        ui->lineEdit_11->clearFocus();
        ui->lineEdit_7->clearFocus();
        ui->lineEdit_12->setFocus();
        int prevValue = ui->lineEdit_12->text().toInt();
        int value6 = (ui->lineEdit_12->text()+digit).toInt();
        if(value6 == 0){
            ui->lineEdit_12->setText(QString::number(2));
            return;
        }
        setRange(ui->lineEdit_12, prevValue, value6, 500);
        ui->progressBar_14->setValue(value6);

    }
    if(ui->lineEdit_11->focusWidget()) {
        ui->lineEdit_7->clearFocus();
        ui->lineEdit_12->clearFocus();
        ui->lineEdit_5->clearFocus();
        ui->lineEdit_11->setFocus();
        int prevValue = ui->lineEdit_11->text().toInt();
        int value7 = (ui->lineEdit_11->text()+digit).toInt();
        if(value7 == 0){
            ui->lineEdit_11->setText(QString::number(5));
            return;
        }
        setRange(ui->lineEdit_11, prevValue, value7,40);
        ui->progressBar_15->setValue(value7);

    }
    //vitrectomy
    if(ui->lineEdit->focusWidget()) {
        ui->lineEdit_19->clearFocus();
        ui->lineEdit_20->clearFocus();

        ui->lineEdit->setFocus();
        int prevValue = ui->lineEdit->text().toInt();
        int value8 = (ui->lineEdit->text()+digit).toInt();
        if(value8 == 0){
            ui->lineEdit->setText(QString::number(60));
            return;
        }
        setRange(ui->lineEdit, prevValue, value8, 2520);
        ui->progressBar_21->setValue(value8);

    }
     if(ui->lineEdit_19->focusWidget()) {
        ui->lineEdit->clearFocus();
        ui->lineEdit_20->clearFocus();
        ui->lineEdit_19->setFocus();
        int prevValue = ui->lineEdit_19->text().toInt();
        int value9 = (ui->lineEdit_19->text()+digit).toInt();
        if(value9 == 0){
            ui->lineEdit_19->setText(QString::number(5));
            return;
        }
        setRange(ui->lineEdit_19, prevValue, value9, 500);
        ui->progressBar_19->setValue(value9);

    }
    if(ui->lineEdit_20->focusWidget()) {
        ui->lineEdit->clearFocus();
        ui->lineEdit_19->clearFocus();
      ui->lineEdit_20->setFocus();
        int prevValue = ui->lineEdit_20->text().toInt();
        int value = (ui->lineEdit_20->text()+digit).toInt();
        if(value == 0){
            ui->lineEdit_20->setText(QString::number(2));
            return;
        }

        setRange(ui->lineEdit_20, prevValue, value, 40);
        ui->progressBar_20->setValue(value);


    }
    if(ui->lineEdit_hp->focusWidget()) {
            ui->lineEdit_hp->setFocus();
            ui->lineEdit_hp->setText(ui->lineEdit_hp->text() + digit);
        }
}

void doctor::on_clickedenter()
{
    key->hide();

    if(ui->lineEdit_hp->hasFocus()) {
        emit handpieceNoChanged(ui->lineEdit_hp->text());  // ← emit signal
        return;
    }


    // Maintain previous value for lineEdit
    QString text = ui->lineEdit->text().trimmed();
    if (!text.isEmpty()) {
        int input = text.toInt();
        int output = getvalue(input);  // Process the input to get the desired output
        ui->lineEdit->setText(QString::number(output));
        lastValidValues[ui->lineEdit] = output;  // Store the processed output as the last valid value
    } else {
        // Restore the last valid value if input is empty
        if (lastValidValues.contains(ui->lineEdit)) {
            ui->lineEdit->setText(QString::number(lastValidValues[ui->lineEdit]));
        }
    }
    // Process other QLineEdits for power and vacuum
    QList<QLineEdit*> lineEdits = {
        ui->lineEdit_3, ui->lineEdit_4, ui->lineEdit_10, ui->lineEdit_9,
        ui->lineEdit_14, ui->lineEdit_15, ui->lineEdit_17, ui->lineEdit_18,
        ui->lineEdit_12, ui->lineEdit_7, ui->lineEdit_19,ui->lineEdit_6
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
        ui->lineEdit_2, ui->lineEdit_8, ui->lineEdit_13,
        ui->lineEdit_16, ui->lineEdit_5, ui->lineEdit_11, ui->lineEdit_20
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


int doctor::increasebutton(int input)
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


int doctor::decreasebutton(int input)
{
    if (input % 2 != 0) {
          input++;  // Increment if odd to make it even
      }
      return input;
}



void doctor::DiathermyBut()
{

    hand->buzz();
    ui->tabWidget->setCurrentIndex(0);
}

void doctor::PhacoBut()
{
    hand->buzz();
    ui->tabWidget->setCurrentIndex(1);

}

void doctor::IrrigationAspirationBut()
{
    hand->buzz();
    ui->tabWidget->setCurrentIndex(2);

}

void doctor::VitrectomyBut()
{
    hand->buzz();
    ui->tabWidget->setCurrentIndex(3);


}

void doctor::BackBut()
{
    hand->buzz();
    emit activatemainwindow();
    key->hide();
    this->close();
}


void doctor::click(int tab)
{
    QString styleSheet = "QPushButton {"
            "border-radius: 40px;"
            "width:10px;"
            "height:10px;"
            "background-color:black;"
            "color:white;"
            "border:none;"
            "font:bold;"// Adjust the radius as needed
                                 "}";
    QString styleSheet1 = "QPushButton {"
            "border-radius: 40px;"
            "width:10px;"
            "height:10px;"
            "background-color:transparent;"
            "border:none;"
            "font:bold;"// Adjust the radius as needed
                                 "}";
    if (tab!=0) {
        ui->DiaBut->setStyleSheet("");
   ui->DiaBut->setStyleSheet(styleSheet1);
    }
    if(tab!=1)
    {
        ui->PhacoBut->setStyleSheet("");
          ui->PhacoBut->setStyleSheet(styleSheet1);
    }
    if(tab!=2){
        ui->IABut->setStyleSheet("");
          ui->IABut->setStyleSheet(styleSheet1);
    }
    if(tab!=3){
    ui->VitBut->setStyleSheet("");
     ui->VitBut->setStyleSheet(styleSheet1);
    }
    if (tab == 0) {
    ui->DiaBut->setStyleSheet(styleSheet);
    } else if (tab == 1) {
     ui->PhacoBut->setStyleSheet(styleSheet);
    }                                                                      //set color
    else if(tab==2){
         ui->IABut->setStyleSheet(styleSheet);
    }
    else if(tab==3){
         ui->VitBut->setStyleSheet(styleSheet);
    }

}
void doctor::clickedtab(int tab1)
{
    QString styleSheet = "QPushButton {"
             "font:20pt;"
            "background-color:black;"
            "color:white;"
            "border:2px solid black;"
            "font:bold;"// Adjust the radius as needed
                                 "}";
    QString styleSheet1 = "QPushButton {"
  "font:20pt;"
            "background-color:white;"
            "color:black;"
            "border:2px solid black;"
            "font:bold;"// Adjust the radius as needed
                                 "}";
    if (tab1!=0) {
        ui->EpinBut_phaco->setStyleSheet("");
   ui->EpinBut_phaco->setStyleSheet(styleSheet1);
    }
    if(tab1!=1)
    {
        ui->QuadBut_phaco->setStyleSheet("");
          ui->QuadBut_phaco->setStyleSheet(styleSheet1);
    }
    if(tab1!=2){
        ui->ChopBut_phaco->setStyleSheet("");
          ui->ChopBut_phaco->setStyleSheet(styleSheet1);
    }
    if(tab1!=3){
    ui->ScupltBut_phaco->setStyleSheet("");
     ui->ScupltBut_phaco->setStyleSheet(styleSheet1);
    }

    if (tab1 == 0) {
    ui->EpinBut_phaco->setStyleSheet(styleSheet);
    } else if (tab1 == 1) {
     ui->QuadBut_phaco->setStyleSheet(styleSheet);
    }                                                                      //set color
    else if(tab1==2){
         ui->ChopBut_phaco->setStyleSheet(styleSheet);
    }
    else if(tab1==3){
         ui->ScupltBut_phaco->setStyleSheet(styleSheet);
    }

}

void doctor::on_EpinBut_phaco_clicked()
{
    hand->buzz();
     ui->tabWidget_2->setCurrentIndex(0);
}

void doctor::on_QuadBut_phaco_clicked()
{
    hand->buzz();
    ui->tabWidget_2->setCurrentIndex(1);
}

void doctor::on_ChopBut_phaco_clicked()
{
    hand->buzz();
    ui->tabWidget_2->setCurrentIndex(2);
}

void doctor::on_ScupltBut_phaco_clicked()
{
    hand->buzz();
    ui->tabWidget_2->setCurrentIndex(3);
}

int doctor::getvalue(int input)
{
    if (input <= 60) {
        return 60;
    } else {
        int remainder = input % 60; // Calculate the remainder when dividing by 60

        if (remainder < 30) {
            // Round down to the nearest multiple of 60
            return (input / 60) * 60;
        } else {
            // Round up to the next multiple of 60
            return ((input / 60) + 1) * 60;
        }
    }
}

void doctor::on_pushButton_clicked()
{
   QString value=ui->comboBox->currentText();
   QString value1=ui->SelectSurgeon->currentText();
   int dia=ui->lineEdit_6->text().toInt();
   int us1pow=ui->lineEdit_4->text().toInt();
   int us1vac=ui->lineEdit_3->text().toInt();
int us1asp=ui->lineEdit_2->text().toInt();
int us2pow=ui->lineEdit_10->text().toInt();
int us2vac=ui->lineEdit_9->text().toInt();
int us2asp=ui->lineEdit_8->text().toInt();
int us3pow=ui->lineEdit_15->text().toInt();
int us3vac=ui->lineEdit_14->text().toInt();
int us3asp=ui->lineEdit_13->text().toInt();
int us4pow=ui->lineEdit_18->text().toInt();
int us4vac=ui->lineEdit_17->text().toInt();
int us4asp=ui->lineEdit_16->text().toInt();
int ia1vac=ui->lineEdit_7->text().toInt();
int ia1asp=ui->lineEdit_5->text().toInt();
int ia2vac=ui->lineEdit_12->text().toInt();
int ia2asp=ui->lineEdit_11->text().toInt();
int vitcut=ui->lineEdit->text().toInt();
int vitvac=ui->lineEdit_19->text().toInt();
int vitasp=ui->lineEdit_20->text().toInt();
 QString powmode=ui->PowModeCom_phaco->currentText();
 QString vacmode=ui->VacModeCom_phaco->currentText();
 QString powmethod=ui->PowMethodCom_phaco->currentText();
 QString us2powmode=ui->PowModeCom_phaco_2->currentText();
 QString us2vacmode=ui->VacModeCom_phaco_2->currentText();
 QString us2powermethod=ui->PowMethodCom_phaco_2->currentText();
 QString us3powmode=ui->PowModeCom_phaco_3->currentText();
 QString us3vacmode=ui->VacModeCom_phaco_3->currentText();
 QString us3powermethod=ui->PowMethodCom_phaco_3->currentText();
 QString us4powmode=ui->PowModeCom_phaco_4->currentText();
 QString us4vacmode=ui->VacModeCom_phaco_4->currentText();
 QString us4powermethod=ui->PowMethodCom_phaco_4->currentText();
 QString ia1mode=ui->VacMode1_iacom->currentText();
QString ia2mode=ui->VacMode2_iacom->currentText();
QString vitmode=ui->CutMode_vitCom->currentText();
QString vitvacmode=ui->VacMode_VitCom->currentText();
QString footleft=ui->LeftFoot->currentText();
QString footright=ui->RightFoot->currentText();
QString b_left=ui->BottomLFoot->currentText();
QString b_right=ui->BottomRFoot->currentText();
QString vibrationtext=ui->Vibration_onoff->text();
QString speakeronoff=ui->ButSpeakerOnOff->text();
QString swaponoff=ui->Swap_onoff->text();

    // Open database connection with a unique name
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE", "unique_connection_name");
    mydb.setDatabaseName(PATH);

    // Check if the QSQLITE driver is available
    QStringList drivers = QSqlDatabase::drivers();
//    //qDebug() << "Available drivers:" << drivers;

    if (!drivers.contains("QSQLITE")) {
//        //qDebug() << "Error: QSQLITE driver not available!";
        return;
    }

    if (!mydb.open()) {
//        //qDebug() << "Error: Unable to open database with path:" << PATH;
        return;
    }

    // Prepare SQL query with placeholders
    QSqlQuery query(mydb);  // Use the specific connection
    query.prepare("SELECT * FROM phacohigh WHERE surgeon = :surgeon");

    // Bind values to the placeholders
    QString surgeon = ui->SelectSurgeon->currentText();
    query.bindValue(":surgeon", surgeon);

    // Debug: Log the query and bound values
    //qDebug() << "Executing query: SELECT * FROM phacohigh WHERE surgeon =" << surgeon;

    // Execute the query
    if (!query.exec()) {
//        //qDebug() << "Error executing SELECT query: " << query.lastError().text();
        return;
    }

    // Check if any rows are fetched
    if (!query.next()) {
//        //qDebug() << "No rows fetched for surgeon:" << surgeon;
        return;
    }

    query.prepare("UPDATE phacohigh SET "
                  "viberationoff = :viberationoff, "
                  "speakeronoff = :speakeronoff, "
                  "Swaponoff = :swaponoff, "
                  "diapowmax = :diapowmax, pump = :pump, Epinaspmax = :Epinaspmax, "
                  "Epinvacmax = :Epinvacmax, Epinpowmax = :Epinpowmax, quadpowmax = :quadpowmax, quadvacmax = :quadvacmax, "
                  "quadaspmax = :quadaspmax, Quadvacmode = :Quadvacmode, Quadpowermethod = :Quadpowermethod, "
                  "Quadpowmode = :Quadpowmode, caspmax = :caspmax, cvacmax = :cvacmax, cpowmax = :cpowmax, "
                  "Chopvacmode = :Chopvacmode, Choppowermethod = :Choppowermethod, Choppowmode = :Choppowmode, "
                  "saspmax = :saspmax, svacmax = :svacmax, spowmax = :spowmax, Sculptvacmode = :Sculptvacmode, "
                  "Sculptpowermethod = :Sculptpowermethod, Sculptpowmode = :Sculptpowmode, "
                  "cortexaspmode = :cortexaspmode, cortexvacmode = :cortexvacmode, polishaspmode = :polishaspmode, "
                  "polishvacmode = :polishvacmode, ia1aspmax = :ia1aspmax, ia1vacmax = :ia1vacmax, "
                  "ia2aspmax = :ia2aspmax, ia2vacmax = :ia2vacmax, vitcutmax = :vitcutmax, vitvacmax = :vitvacmax, "
                  "vitaspmax = :vitaspmax, vitcutmode = :vitcutmode, vitvacmode = :vitvacmode, "
                  "footleft = :footleft, footright = :footright, footbottomleft = :footbottomleft, footbottomright = :footbottomright, "
                  "Epinpowermethod = :Epinpowermethod, Epinpowmode = :Epinpowmode, Epinvacmode = :Epinvacmode "
                  "WHERE surgeon = :surgeon");

    // Bind values to the placeholders
    query.bindValue(":diapowmax", ui->lineEdit_6->text().toInt());
    query.bindValue(":pump", ui->comboBox->currentText());
 query.bindValue(":viberationoff", ui->Vibration_onoff->text());
 query.bindValue(":speakeronoff", ui->ButSpeakerOnOff->text());
 query.bindValue(":swaponoff", ui->Swap_onoff->text());
    query.bindValue(":Epinaspmax", ui->lineEdit_2->text().toInt());
    query.bindValue(":Epinvacmax", ui->lineEdit_3->text().toInt());
    query.bindValue(":Epinpowmax", ui->lineEdit_4->text().toInt());
    query.bindValue(":quadpowmax", ui->lineEdit_10->text().toInt());
    query.bindValue(":quadvacmax", ui->lineEdit_9->text().toInt());
    query.bindValue(":quadaspmax", ui->lineEdit_8->text().toInt());
    query.bindValue(":Quadvacmode", ui->VacModeCom_phaco_2->currentText());
    query.bindValue(":Quadpowermethod", ui->PowMethodCom_phaco_2->currentText());
    query.bindValue(":Quadpowmode", ui->PowModeCom_phaco_2->currentText());
    query.bindValue(":caspmax", ui->lineEdit_13->text().toInt());
    query.bindValue(":cvacmax", ui->lineEdit_14->text().toInt());
    query.bindValue(":cpowmax", ui->lineEdit_15->text().toInt());
    query.bindValue(":Chopvacmode", ui->VacModeCom_phaco_3->currentText());
    query.bindValue(":Choppowermethod", ui->PowMethodCom_phaco_3->currentText());
    query.bindValue(":Choppowmode", ui->PowModeCom_phaco_3->currentText());
    query.bindValue(":saspmax", ui->lineEdit_16->text().toInt());
    query.bindValue(":svacmax", ui->lineEdit_17->text().toInt());
    query.bindValue(":spowmax", ui->lineEdit_18->text().toInt());
    query.bindValue(":Sculptvacmode", ui->VacModeCom_phaco_4->currentText());
    query.bindValue(":Sculptpowermethod", ui->PowMethodCom_phaco_4->currentText());
    query.bindValue(":Sculptpowmode", ui->PowModeCom_phaco_4->currentText());
    query.bindValue(":cortexaspmode", ui->VacModeCom_phaco_4->currentText());
    query.bindValue(":cortexvacmode", ui->VacModeCom_phaco_4->currentText());
    query.bindValue(":polishaspmode", ui->PowMethodCom_phaco_4->currentText());
    query.bindValue(":polishvacmode", ui->VacModeCom_phaco_4->currentText());
    query.bindValue(":ia1aspmax", ui->lineEdit_11->text().toInt());
    query.bindValue(":ia1vacmax", ui->lineEdit_12->text().toInt());
    query.bindValue(":ia2aspmax", ui->lineEdit_5->text().toInt());
    query.bindValue(":ia2vacmax", ui->lineEdit_7->text().toInt());
    query.bindValue(":vitcutmax", ui->lineEdit->text().toInt());
    query.bindValue(":vitvacmax", ui->lineEdit_19->text().toInt());
    query.bindValue(":vitaspmax", ui->lineEdit_20->text().toInt());
    query.bindValue(":vitcutmode", ui->CutMode_vitCom->currentText());
    query.bindValue(":vitvacmode", ui->VacMode_VitCom->currentText());
    query.bindValue(":Epinpowermethod", ui->PowMethodCom_phaco->currentText());
    query.bindValue(":Epinpowmode", ui->PowModeCom_phaco->currentText());
    query.bindValue(":Epinvacmode", ui->VacModeCom_phaco->currentText());
    query.bindValue(":footleft", ui->LeftFoot->currentText());
    query.bindValue(":footright", ui->RightFoot->currentText());
    query.bindValue(":footbottomleft", ui->BottomLFoot->currentText());
    query.bindValue(":footbottomright", ui->BottomRFoot->currentText());
    query.bindValue(":surgeon", surgeon);
qDebug()<<"the power method"<<ui->PowMethodCom_phaco->currentText()<<ui->PowMethodCom_phaco_2->currentText()<<ui->PowMethodCom_phaco_3->currentText()<<ui->PowMethodCom_phaco_4->currentText();
    // Debug: Log the update query and bound values
//    //qDebug() << "Executing update query for surgeon:" << surgeon;

    // Execute the update query
    if (!query.exec()) {
//qDebug() << "Error executing UPDATE query: " << query.lastError().text();
    } else {
//qDebug() << "Data updated successfully.";
        QMessageBox* msgBox = new QMessageBox(QMessageBox::Information, "Info", "Surgeon is updated");

        // Set the message box to show
        msgBox->show();

        // Create a QTimer to close the message box after 500 ms
        QTimer::singleShot(700, msgBox, [msgBox]() {
            msgBox->hide();  // Hide the message box
            delete msgBox;   // Delete the message box to free memory

    });
    }

emit sendValues(
    value,value1, dia, us1pow, us1vac, us1asp,
    us2pow, us2vac, us2asp, us3pow, us3vac, us3asp,
    us4pow, us4vac, us4asp, ia1vac, ia1asp,
    ia2vac, ia2asp, vitcut, vitvac, vitasp,
    powmode, vacmode, powmethod,
    us2powmode, us2vacmode, us2powermethod,
    us3powmode, us3vacmode, us3powermethod,
    us4powmode, us4vacmode, us4powermethod,
    ia1mode, ia2mode, vitmode, vitvacmode
);
    //qDebug()<<ia2asp<<ia1asp<<ia1vac<<ia2vac<<"these are sended to mainwindow1111111111111";
emit sendleftfootvalues(ui->LeftFoot->currentText());
//    //qDebug()<<"footleft is"<<footleft;
    emit sendrightfootvalues(ui->RightFoot->currentText());
//      //qDebug()<<"footright is"<<footright;
    emit sendbleftfootvalues(ui->BottomLFoot->currentText());
//    //qDebug()<<"footbottom left is"<<b_left;
    emit sendbrightfootvalues(ui->BottomRFoot->currentText());
//    //qDebug()<<"footbottom right is"<<b_right;
    //transmitval(fpzero,fpone,fptwo,fpthree);
   emit activatemainwindow();
     //qDebug()<<"values are transmitted"<<fpzero<<fpone<<fptwo<<fpthree;
    emit tx_viberation(ui->Vibration_onoff->text());
    //qDebug()<<"viberation in doctor"<<ui->Vibration_onoff->text();
    emit tx_speakeronoff(ui->ButSpeakerOnOff->text());
    this->close();
    emit togglbutton();
    emit tx_swaponoff(swaponoff);
    mydb.close();

    QSqlDatabase::removeDatabase("unique_connection_name");
}

void doctor::pumpvalue()
{
    QString value=ui->comboBox->currentText();


}

void doctor::on_SaveDiaBut_clicked()
{
    QMessageBox* msgBox = new QMessageBox(QMessageBox::Information, "Info", "Values are updated");

    // Set the message box to show
    msgBox->show();

    // Create a QTimer to close the message box after 500 ms
    QTimer::singleShot(1000, msgBox, [msgBox]() {
        msgBox->hide();  // Hide the message box
        delete msgBox;   // Delete the message box to free memory

});
   on_pushButton_clicked();
   this->close();
}
void doctor::handleDataSaved()
{

}
void doctor::onComboBoxIndexChanged(int index)
{


    // Step 2: Create a new database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(PATH);

    if (!db.open()) {
        // Uncomment for debugging
        // qDebug() << "Error: connection with database failed:";
        return;
    }

    // Step 3: Prepare and execute the SQL query to update the last selected index
    QSqlQuery query(db);
    query.prepare("UPDATE phacohigh SET lastupdate = :index");
    query.bindValue(":index", index);

    // Get the current text from the combo box
    QString text = ui->SelectSurgeon->itemText(index);
    emit surgeonNamefoot(text);

    if (!query.exec()) {
        // Uncomment for debugging
        // qDebug() << "Error updating last selected index:" << query.lastError();
    } else {
        // Uncomment for debugging
        // qDebug() << "Last selected index updated to" << index;
    }

   query.clear();
    db.close();
    // Removed unnecessary database removal; keep connection for future use
}

void doctor::setLastSelectedValue()
{


    // Create a new database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(PATH);

    if (!db.open()) {
        // Uncomment for debugging
        // qDebug() << "Error: connection with database failed:";
        return;
    }

    // Prepare and execute the query
    QSqlQuery query(db);
    query.prepare("SELECT lastupdate FROM phacohigh LIMIT 1");

    if (!query.exec()) {
        // Uncomment for debugging
        // qDebug() << "Error retrieving last index:";
        db.close();
        return;
    }

    // Check if we got a result
    if (query.next()) {
        int lastIndex = query.value(0).toInt();
        ui->SelectSurgeon->setCurrentIndex(lastIndex);
        QString text = ui->SelectSurgeon->currentText();
        emit surgeonNamefoot(text);
        // Uncomment for debugging
        // qDebug() << "Last selected index set to" << lastIndex;
    }
    query.clear();

    // No need to call query.finish() here; it's automatically cleaned up when going out of scope
    db.close();
    // No need to remove the database connection here; just keep the connection for future use if needed
}

void doctor::receivingvalueffs(int &val0, int &val1,int &val2,int &val3)
{
   int fpzero=val0;
   int fpone=val1;
  int fptwo=val2;
  int fpthree=val3;
  transmitval(fpzero,fpone,fptwo,fpthree);
}


void doctor::onSurgeonSelectionChanged(const QString &surgeonName)
{

    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (!QSqlDatabase::contains("qt_sql_default_connection")) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
        db.setDatabaseName(PATH);
    } else {
        QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
    }


    if (!db.isOpen() && !db.open()) {
        //qDebug() << "Failed to open database. Error:" << db.lastError().text();
        return;
    }
    // Prepare a single query to fetch all required data
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

    // Execute the query
    if (!query.exec()) {
        ////qDebug() << "Failed to execute query. Error:" << query.lastError().text();
        return;
    }

    // Check if data is available and update the UI
    if (query.next()) {
        // Retrieve the values from the query result
        int phacoPowerMax = query.value("diapowmax").toInt();
        ui->progressBar->setValue(phacoPowerMax);
        QString pump = query.value("pump").toString();
        QString vib_onoff = query.value("viberationoff").toString();
        ui->Vibration_onoff->setText(vib_onoff);
        QString speaker_onoff = query.value("speakeronoff").toString();
        ui->ButSpeakerOnOff->setText(speaker_onoff);
        if(ui->Vibration_onoff->text() == "Vibration ON"){
            ui->lab_vibonoff->setStyleSheet("image: url(:/images/vibrationon.png);border:2px solid skyblue;border-radius:20px;");
            ui->lab_vibonoff->update();
        }else if(ui->Vibration_onoff->text() == "Vibration OFF"){
            ui->lab_vibonoff->setStyleSheet("image: url(:/images/vibrationoff.png);border:2px solid skyblue;border-radius:20px;");
        }
        if(speaker_onoff == "Speaker ON"){
            ui->lab_vibonoff_2->setStyleSheet("image: url(:/images/speakeron.png);border:2px solid skyblue;border-radius:20px;");
        }else if(speaker_onoff == "Speaker OFF"){
            ui->lab_vibonoff_2->setStyleSheet("image: url(:/images/speakeroff.png);border:2px solid skyblue;border-radius:20px;");

        }
        QString swap_onoff = query.value("Swaponoff").toString();
        ui->Swap_onoff->setText(swap_onoff);
        // US1 (Epinucleus) parameters
        int us1power = query.value("Epinpowmax").toInt();
        int us1vacmax = query.value("Epinvacmax").toInt();
        int us1flowmax = query.value("Epinaspmax").toInt();
        QString us1mode = query.value("Epinpowmode").toString();
        QString us1vacmode = query.value("Epinvacmode").toString();
        QString us1powermethod = query.value("Epinpowermethod").toString();
        ui->progressBar_5->setValue(us1flowmax);
        ui->progressBar_6->setValue(us1vacmax);
        ui->progressBar_7->setValue(us1power);
        // US2 (Quadrant) parameters
        int us2power = query.value("quadpowmax").toInt();
        int us2vacmax = query.value("quadvacmax").toInt();
        int us2aspmax = query.value("quadaspmax").toInt();
        QString us2mode = query.value("Quadpowmode").toString();
        QString us2vacmode = query.value("Quadvacmode").toString();
        QString us2powermethod = query.value("Quadpowermethod").toString();
        ui->progressBar_8->setValue(us2aspmax);
        ui->progressBar_9->setValue(us2vacmax);
        ui->progressBar_10->setValue(us2power);
        // US3 (Chop) parameters
        int us3power = query.value("cpowmax").toInt();
        int us3vacmax = query.value("cvacmax").toInt();
        int us3aspmax = query.value("caspmax").toInt();
        QString us3mode = query.value("Choppowmode").toString();
        QString us3vacmode = query.value("Chopvacmode").toString();
        QString us3powermethod = query.value("Choppowermethod").toString();
        ui->progressBar_11->setValue(us3aspmax);
        ui->progressBar_12->setValue(us3vacmax);
        ui->progressBar_13->setValue(us3power);
        // US4 (Sculpt) parameters
        int us4power = query.value("spowmax").toInt();
        int us4vacmax = query.value("svacmax").toInt();
        int  us4aspmax= query.value("saspmax").toInt();
        QString us4mode = query.value("Sculptpowmode").toString();
        QString us4vacmode = query.value("Sculptvacmode").toString();
        QString us4powermethod = query.value("Sculptpowermethod").toString();
        ui->progressBar_2->setValue(us4aspmax);
        ui->progressBar_3->setValue(us4vacmax);
        ui->progressBar_4->setValue(us4power);
        // IA (Irrigation/Aspiration) parameters
        int ia1aspmax = query.value("ia1aspmax").toInt();
        int ia1vacmax = query.value("ia1vacmax").toInt();
        int ia2vacmax = query.value("ia2vacmax").toInt();
        int ia2aspmax = query.value("ia2aspmax").toInt();
        QString ia1mode = query.value("cortexvacmode").toString();
        QString ia2mode = query.value("polishvacmode").toString();
        ui->progressBar_15->setValue(ia1aspmax);
        ui->progressBar_14->setValue(ia1vacmax);
        ui->progressBar_16->setValue(ia2vacmax);
        ui->progressBar_17->setValue(ia2aspmax);
        // Vitrectomy parameters
        int vitcutmax = query.value("vitcutmax").toInt();
        int vitvacmax = query.value("vitvacmax").toInt();
        int vitaspmax = query.value("vitaspmax").toInt();
        QString vitcutmode = query.value("vitcutmode").toString();
        QString vitvacmode = query.value("vitvacmode").toString();
        ui->progressBar_19->setValue(vitvacmax);
        ui->progressBar_20->setValue(vitaspmax);
        ui->progressBar_21->setValue(vitcutmax);
        // Update UI with foot pedal data
      int footpedalzero = query.value("fzero").toInt();
      int footpedalone = query.value("fone").toInt();
      int footpedaltwo = query.value("ftwo").toInt();
      int footpedalthree = query.value("fthree").toInt();
            ui->LeftFoot->setCurrentText(query.value("footleft").toString());
            ui->RightFoot->setCurrentText(query.value("footright").toString());
            ui->BottomLFoot->setCurrentText(query.value("footbottomleft").toString());
            ui->BottomRFoot->setCurrentText(query.value("footbottomright").toString());


        // Update UI components with the retrieved values
        ui->lineEdit_6->setText(QString::number(phacoPowerMax));
       ui->comboBox->setCurrentText(pump);
        // Update US1 UI components
        ui->lineEdit_4->setText(QString::number(us1power));  // Power
        ui->lineEdit_3->setText(QString::number(us1vacmax)); // Vacuum
        ui->lineEdit_2->setText(QString::number(us1flowmax)); // Flow
        ui->PowModeCom_phaco->setCurrentText(us1mode);
        ui->VacModeCom_phaco->setCurrentText(us1vacmode);
        ui->PowMethodCom_phaco->setCurrentText(us1powermethod);

        // Update US2 UI components
        ui->lineEdit_10->setText(QString::number(us2power)); // Power
        ui->lineEdit_9->setText(QString::number(us2vacmax)); // Vacuum
        ui->lineEdit_8->setText(QString::number(us2aspmax)); // Aspiration
        ui->PowModeCom_phaco_2->setCurrentText(us2mode);
        ui->VacModeCom_phaco_2->setCurrentText(us2vacmode);
        ui->PowMethodCom_phaco_2->setCurrentText(us2powermethod);

        // Update US3 UI components
        ui->lineEdit_15->setText(QString::number(us3power)); // Power
        ui->lineEdit_14->setText(QString::number(us3vacmax)); // Vacuum
        ui->lineEdit_13->setText(QString::number(us3aspmax)); // Aspiration
        ui->PowModeCom_phaco_3->setCurrentText(us3mode);
        ui->VacModeCom_phaco_3->setCurrentText(us3vacmode);
        ui->PowMethodCom_phaco_3->setCurrentText(us3powermethod);

        // Update US4 UI components
        ui->lineEdit_16->setText(QString::number(us4aspmax)); // Power
        ui->lineEdit_17->setText(QString::number(us4vacmax)); // Vacuum
        ui->lineEdit_18->setText(QString::number(us4power)); // Aspiration
        ui->PowModeCom_phaco_4->setCurrentText(us4mode);   // Changed from us1mode to us4mode
        ui->VacModeCom_phaco_4->setCurrentText(us4vacmode);   // Changed from us1vacmode to us4vacmode
        ui->PowMethodCom_phaco_4->setCurrentText(us4powermethod);

        // Update IA1 and IA2 UI components
        ui->lineEdit_7->setText(QString::number(ia2vacmax));
        ui->lineEdit_5->setText(QString::number(ia2aspmax));
        ui->lineEdit_11->setText(QString::number(ia1aspmax));
        ui->lineEdit_12->setText(QString::number(ia1vacmax));
        ui->VacMode1_iacom->setCurrentText(ia1mode);
        ui->VacMode2_iacom->setCurrentText(ia2mode);

        // Update Vitrectomy UI components
        ui->lineEdit->setText(QString::number(vitcutmax));
        ui->lineEdit_20->setText(QString::number(vitaspmax));
        ui->lineEdit_19->setText(QString::number(vitvacmax));
        ui->CutMode_vitCom->setCurrentText(vitcutmode);
        ui->VacMode_VitCom->setCurrentText(vitvacmode);
        ui->Vibration_onoff->setText(vib_onoff);

         emit leftfoot(ui->LeftFoot->currentText());
        emit rightfoot(ui->RightFoot->currentText());
        emit bottomleft(ui->BottomLFoot->currentText());
        emit bottomright(ui->BottomRFoot->currentText());
        emit pumpsignal(pump);
        emit tx_swaponoff(swap_onoff);
      cFoot->receivelineEditval(footpedalzero,footpedalone,footpedaltwo,footpedalthree);
      emit tx_speakeronoff(speaker_onoff);
  emit activatemainwindow();
      on_clickedenter();
       query.clear();
    } if (db.isOpen()) {
        db.close();  // This closes the connection safely.
    }
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
}

void doctor::on_pushButton_2_clicked()
{
    cFoot->show();
    QString text=ui->SelectSurgeon->currentText();
    emit txfootpedalvalues(ui->LeftFoot->currentText(),ui->RightFoot->currentText(),ui->BottomLFoot->currentText(),ui->BottomRFoot->currentText());
    emit surgeonNamefoot(text);

}

void doctor::receivetopleft(const QString &text)
{
    ui->LeftFoot->setCurrentText(text);
   // qDebug()<<"the current text of topleft is"<<text;
}

void doctor::receivebottomleft(const QString &text)
{
   ui->BottomLFoot->setCurrentText(text);
  // qDebug()<<"the current text of bottomleft is"<<text;

}

void doctor::receivetopright(const QString &text)
{
    ui->RightFoot->setCurrentText(text);
    //qDebug()<<"the current text of topright is"<<text;

}

void doctor::receivebottomright(const QString &text)
{
    ui->BottomRFoot->setCurrentText(text);
 //  qDebug()<<"the current text of bottomright is"<<text;

}

void doctor::on_Vibration_onoff_clicked()
{
    vib_onoff=!vib_onoff;
    if(!vib_onoff){
        ui->Vibration_onoff->setText("Vibration ON");
        ui->lab_vibonoff->setStyleSheet("image: url(:/images/vibrationon.png);border:2px solid skyblue;border-radius:20px;");
    }else{
        ui->Vibration_onoff->setText("Vibration OFF");
        ui->lab_vibonoff->setStyleSheet("image: url(:/images/vibrationoff.png);border:2px solid skyblue;border-radius:20px;");

    }
}

void doctor::on_ButSpeakerOnOff_clicked()
{
    spealer_onoff = !spealer_onoff;
    if(!spealer_onoff){
        ui->ButSpeakerOnOff->setText("Speaker ON");
        ui->lab_vibonoff_2->setStyleSheet("image: url(:/images/speakeron.png);border:2px solid skyblue;border-radius:20px;");

    }else{
        ui->ButSpeakerOnOff->setText("Speaker OFF");
        ui->lab_vibonoff_2->setStyleSheet("image: url(:/images/speakeroff.png);border:2px solid skyblue;border-radius:20px;");

    }
}

void doctor::on_Swap_onoff_clicked()
{
    swap_onoff=!swap_onoff;
    if(!swap_onoff){
        ui->Swap_onoff->setText("Swap ON");

    }
    else{
        ui->Swap_onoff->setText("Swap OFF");

    }
}

void doctor::on_clickedclear()
{
    // Find whichever lineEdit has focus and clear it
    QList<QLineEdit*> allEdits = {
        ui->lineEdit_hp,
        ui->lineEdit_6,  ui->lineEdit_2,  ui->lineEdit_3,
        ui->lineEdit_4,  ui->lineEdit_8,  ui->lineEdit_9,
        ui->lineEdit_10, ui->lineEdit_13, ui->lineEdit_14,
        ui->lineEdit_15, ui->lineEdit_16, ui->lineEdit_17,
        ui->lineEdit_18, ui->lineEdit_5,  ui->lineEdit_7,
        ui->lineEdit_11, ui->lineEdit_12, ui->lineEdit,
        ui->lineEdit_19, ui->lineEdit_20
    };

    for (QLineEdit *le : allEdits) {
        if (le->hasFocus()) {
            le->clear();   // ← clears the text, shows empty
            break;
        }
    }
}
