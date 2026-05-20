#include "footpedal.h"
#include "ui_footpedal.h"
#include <QMessageBox>
#include<QDebug>
#include<QSqlError>

footpedal::footpedal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::footpedal)
{
    ui->setupUi(this);
    move(0,0);
    d1 = new doctor;

    // Export and set direction of GPIOs
        exportGPIO(961);
        exportGPIO(962);
        exportGPIO(963);
        exportGPIO(964);

        setGPIODirection(961, "in");
        setGPIODirection(962, "in");
        setGPIODirection(963, "in");
        setGPIODirection(964, "in");
        //calling
        readInitialGPIOValues(); // Read initial values
    // Set up connections
    setupConnections();
    //qDebug() << "readInitialGPIOValues function entered.";

    connect(ui->backbut, &QPushButton::clicked, this, &footpedal::Back);
    connect(ui->savebut, &QPushButton::clicked, this, &footpedal::on_pushButton_clicked);
    QTimer *timer=new QTimer;
    connect(timer,&QTimer::timeout,this,&footpedal::readInitialGPIOValues);
    timer->start(100);
    //qDebug()<<"the foorpedal widow is finished";
}

footpedal::~footpedal()
{
    delete ui;
db.close();
}

void footpedal::Back()
{
    this->close();
    emit activatemain();
}

void footpedal::setupConnections()
{
    connect(ui->left_footcom, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &footpedal::storeComboBoxSelection);
    connect(ui->right_footcom, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &footpedal::storeComboBoxSelection);
    connect(ui->bleft_footcom, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &footpedal::storeComboBoxSelection);
    connect(ui->bright_footcom, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &footpedal::storeComboBoxSelection);
}
void footpedal::storeComboBoxSelection(int index)
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    if (comboBox)
    {
        QString action = comboBox->itemText(index);

        if (comboBox == ui->left_footcom) {
            leftFootcomAction = action;
        } else if (comboBox == ui->right_footcom) {
            rightFootcomAction = action;
        } else if (comboBox == ui->bleft_footcom) {
            bleftFootcomAction = action;
        } else if (comboBox == ui->bright_footcom) {
            brightFootcomAction = action;
        }
    }
}

void footpedal::readInitialGPIOValues()
{
    int value1 = readGPIOValue(961);
    int value2 = readGPIOValue(962);
    int value3 = readGPIOValue(963);
    int value4 = readGPIOValue(964);


       if (leftFootcomAction == "Continuous Irrigation" ) {
          if (value1 == 1 && flag1 == 0 && state1 == 0) {
               state1 = 1;           }
           if (value1 == 0 && flag1 == 0 && state1 == 1) {
               flag1 = 1;
               state1 = 2;
               emit continous_irrigation(0);
           }
          if (value1 == 1 && flag1 == 1 && state1 == 2) {
               state1 = 3;
       }
      if (value1 == 0 && flag1 == 1 && state1 == 3) {
               emit continous_irrigation(0);
               flag1 = 0;
               state1 = 0;       }

}else if (leftFootcomAction == "Power ON/OFF") {
           if (value1 == 1 && flag1 == 0 && state1 == 0) {
                state1 = 1;           }
            if (value1 == 0 && flag1 == 0 && state1 == 1) {
                flag1 = 1;
                state1 = 2;
                emit togglePower(0);
            }
           if (value1 == 1 && flag1 == 1 && state1 == 2) {
                state1 = 3;
        }
       if (value1 == 0 && flag1 == 1 && state1 == 3) {
                emit togglePower(1);
                flag1 = 0;
                state1 = 0;       }
    }
else if (leftFootcomAction == "Increment") {
           if (value1 == 1 && flag1 == 0 && state1 == 0) {
                state1 = 1;           }
            if (value1 == 0 && flag1 == 0 && state1 == 1) {
                flag1 = 1;
                state1 = 2;
                emit moveTopToBottom(value1);
            }
           if (value1 == 1 && flag1 == 1 && state1 == 2) {
                state1 = 3;
        }
       if (value1 == 0 && flag1 == 1 && state1 == 3) {
               emit moveTopToBottom(value1);
                flag1 = 0;
                state1 = 0;       }
    } else if (leftFootcomAction == "Decrement") {

           if (value1 == 1 && flag1 == 0 && state1 == 0) {
                state1 = 1;           }
            if (value1 == 0 && flag1 == 0 && state1 == 1) {
                flag1 = 1;
                state1 = 2;
                emit moveBottomToTop(value1);
            }
           if (value1 == 1 && flag1 == 1 && state1 == 2) {
                state1 = 3;
        }
       if (value1 == 0 && flag1 == 1 && state1 == 3) {
               emit moveBottomToTop(value1);
                flag1 = 0;
                state1 = 0;       }
    } else if (leftFootcomAction == "PDM") {
           if (value1 == 1 && flag1 == 0 && state1 == 0) {
                state1 = 1;           }
            if (value1 == 0 && flag1 == 0 && state1 == 1) {
                flag1 = 1;
                state1 = 2;
                emit powerdm(value1);
                 emit powerdm1(value1);
                 emit powerdm2(value1);
                 emit powerdm3(value1);


            }
           if (value1 == 1 && flag1 == 1 && state1 == 2) {
                state1 = 3;
        }
       if (value1 == 0 && flag1 == 1 && state1 == 3) {
               emit powerdm(value1);
            emit powerdm1(value1);
            emit powerdm2(value1);
            emit powerdm3(value1);
                flag1 = 0;
                state1 = 0;       }
    } else if(ui->left_footcom->currentText() == "Reflux" || leftFootcomAction == "Reflux"){
           if (value1 == 1 && flag1 == 0 && state1 == 0) {
                state1 = 1;           }
            if (value1 == 0 && flag1 == 0 && state1 == 1) {
                flag1 = 1;
                state1 = 2;
                emit performReflux(value1);
            }
           if (value1 == 1 && flag1 == 1 && state1 == 2) {
                state1 = 3;
        }
       if (value1 == 0 && flag1 == 1 && state1 == 3) {
               emit performReflux(value1);
                flag1 = 0;
                state1 = 0;       }

       }
 //-------------------------------------------------
    //bottom right
    if (brightFootcomAction == "Continuous Irrigation") {//continuous

        if (value2 == 1 && flag2 == 0 && state2 == 0) {
             state2 = 1;           }
         if (value2 == 0 && flag2 == 0 && state2 == 1) {
             flag2 = 1;
             state2 = 2;
             emit continous_irrigation(0);
         }
        if (value2 == 1 && flag2 == 1 && state2 == 2) {
             state2 = 3;
     }
    if (value2 == 0 && flag2 == 1 && state2 == 3) {
             emit continous_irrigation(0);
             flag2 = 0;
             state2 = 0;       }
    }
    else if (brightFootcomAction == "Power ON/OFF") {//power onoff
        if (value2 == 1 && flag2 == 0 && state2 == 0) {
             state2 = 1;           }
         if (value2 == 0 && flag2 == 0 && state2 == 1) {
             flag2 = 1;
             state2 = 2;
             emit togglePower(0);
         }
        if (value2 == 1 && flag2 == 1 && state2 == 2) {
             state2 = 3;
     }
    if (value2 == 0 && flag2 == 1 && state2 == 3) {
             emit togglePower(1);
             flag2 = 0;
             state2 = 0;
    }
}
 else if (brightFootcomAction == "Increment") {
        if (value2 == 1 && flag2 == 0 && state2 == 0) {
             state2 = 1;           }
         if (value2 == 0 && flag2 == 0 && state2 == 1) {
             flag2 = 1;
             state2 = 2;
             emit moveTopToBottom(value2);
         }
        if (value2 == 1 && flag2 == 1 && state2 == 2) {
             state2 = 3;
     }
    if (value2 == 0 && flag2 == 1 && state2 == 3) {
            emit moveTopToBottom(value2);
             flag2 = 0;
             state2 = 0;       }
    }
    else if (brightFootcomAction == "Decrement") {

        if (value2 == 1 && flag2 == 0 && state2 == 0) {
             state2 = 1;           }
         if (value2 == 0 && flag2 == 0 && state2 == 1) {
             flag2 = 1;
             state2 = 2;
             emit moveBottomToTop(value2);
         }
        if (value2 == 1 && flag2 == 1 && state2 == 2) {
             state2 = 3;
     }
    if (value2 == 0 && flag2 == 1 && state2 == 3) {
            emit moveBottomToTop(value2);
             flag2 = 0;
             state2 = 0;       }
    }
    else if (brightFootcomAction == "PDM") {
        if (value2 == 1 && flag2 == 0 && state2 == 0) {
             state2 = 1;           }
         if (value2 == 0 && flag2 == 0 && state2 == 1) {
             flag2 = 1;
             state2 = 2;
       emit powerdm(value2);
             emit powerdm1(value2);
             emit powerdm2(value2);
             powerdm3(value2);
         }
        if (value2 == 1 && flag2 == 1 && state2 == 2) {
             state2 = 3;
     }
    if (value2 == 0 && flag2 == 1 && state2 == 3) {
        emit powerdm(value2);
              emit powerdm1(value2);
              emit powerdm2(value2);
              powerdm3(value2);
             flag2 = 0;
             state2 = 0;       }
    }
    else if(ui->bright_footcom->currentText()=="Reflux" || brightFootcomAction == "Reflux"){
        if (value2 == 1 && flag2 == 0 && state2 == 0) {
             state2 = 1;           }
         if (value2 == 0 && flag2 == 0 && state2 == 1) {
             flag2 = 1;
             state2 = 2;
             emit performReflux(value2);
         }
        if (value2 == 1 && flag2 == 1 && state2 == 2) {
             state2 = 3;
     }
    if (value2 == 0 && flag2 == 1 && state2 == 3) {
            emit performReflux(value2);
             flag2 = 0;
             state2 = 0;       }
    }
//------------------------------------------------------------
    //topright

    if (rightFootcomAction == "Continuous Irrigation") {
        if (value3 == 1 && flag3 == 0 && state3 == 0) {
             state3 = 1;           }
         if (value3 == 0 && flag3 == 0 && state3 == 1) {
             flag3 = 1;
             state3 = 2;
             emit continous_irrigation(0);
         }
        if (value3 == 1 && flag3 == 1 && state3 == 2) {
             state3 = 3;
     }
    if (value3 == 0 && flag3 == 1 && state3 == 3) {
             emit continous_irrigation(0);
             flag3 = 0;
             state3 = 0;       }
    } else if (rightFootcomAction == "Power ON/OFF") {
        if (value3 == 1 && flag3 == 0 && state3 == 0) {
             state3 = 1;           }
         if (value3 == 0 && flag3 == 0 && state3 == 1) {
             flag3 = 1;
             state3 = 2;
             emit togglePower(0);
         }
        if (value3 == 1 && flag3 == 1 && state3 == 2) {
             state3 = 3;
     }
    if (value3 == 0 && flag3 == 1 && state3 == 3) {
             emit togglePower(1);
             flag3 = 0;
             state3 = 0;       }
    } else if (rightFootcomAction == "Increment") {
        if (value3 == 1 && flag3 == 0 && state3 == 0) {
             state3 = 1;           }
         if (value3 == 0 && flag3 == 0 && state3 == 1) {
             flag3 = 1;
             state3 = 2;
             emit moveTopToBottom(value3);
         }
        if (value3 == 1 && flag3 == 1 && state3 == 2) {
             state3 = 3;
     }
    if (value3 == 0 && flag3 == 1 && state3 == 3) {
            emit moveTopToBottom(value3);
             flag3 = 0;
             state3 = 0;       }
    } else if (rightFootcomAction == "Decrement") {
        if (value3 == 1 && flag3 == 0 && state3 == 0) {
             state3 = 1;           }
         if (value3 == 0 && flag3 == 0 && state3 == 1) {
             flag3 = 1;
             state3 = 2;
             emit moveBottomToTop(value3);
         }
        if (value3 == 1 && flag3 == 1 && state3 == 2) {
             state3 = 3;
     }
    if (value3 == 0 && flag3 == 1 && state3 == 3) {
            emit moveBottomToTop(value3);
             flag3 = 0;
             state3 = 0;       }
    } else if (rightFootcomAction == "PDM") {
        if (value3 == 1 && flag3 == 0 && state3 == 0) {
             state3 = 1;           }
         if (value3 == 0 && flag3 == 0 && state3 == 1) {
             flag3 = 1;
             state3 = 2;
             emit powerdm(value3);
             emit powerdm1(value3);
             emit powerdm2(value3);
             emit powerdm3(value3);
         }
        if (value3 == 1 && flag3 == 1 && state3 == 2) {
             state3 = 3;
     }
    if (value3 == 0 && flag3 == 1 && state3 == 3) {
        emit powerdm(value3);
        emit powerdm1(value3);
        emit powerdm2(value3);
        emit powerdm3(value3);
             flag3 = 0;
             state3 = 0;       }
    }else if(ui->right_footcom->currentText() == "Reflux" || rightFootcomAction == "Reflux"){
        if (value3 == 1 && flag3 == 0 && state3 == 0) {
             state3 = 1;           }
         if (value3 == 0 && flag3 == 0 && state3 == 1) {
             flag3 = 1;
             state3 = 2;
             emit performReflux(value3);
         }
        if (value3 == 1 && flag3 == 1 && state3 == 2) {
             state3 = 3;
     }
    if (value3 == 0 && flag3 == 1 && state3 == 3) {
            emit performReflux(value3);
             flag3 = 0;
             state3 = 0;       }
    }
//---------------------------------------------------------------
    //bottom left
    if (bleftFootcomAction == "Continuous Irrigation") {
        if (value4 == 1 && flag4 == 0 && state4 == 0) {
             state4 = 1;           }
         if (value4 == 0 && flag4 == 0 && state4 == 1) {
             flag4 = 1;
             state4 = 2;
             emit continous_irrigation(0);
         }
        if (value4 == 1 && flag4 == 1 && state4 == 2) {
             state4 = 3;
     }
    if (value4 == 0 && flag4 == 1 && state4 == 3) {
             emit continous_irrigation(0);
             flag4 = 0;
             state4 = 0;       }
    } else if (bleftFootcomAction == "Power ON/OFF") {
        if (value4 == 1 && flag4 == 0 && state4 == 0) {
             state4 = 1;           }
         if (value4 == 0 && flag4 == 0 && state4 == 1) {
             flag4 = 1;
             state4 = 2;
             emit togglePower(0);
         }
        if (value4 == 1 && flag4 == 1 && state4 == 2) {
             state4 = 3;
     }
    if (value4 == 0 && flag4 == 1 && state4 == 3) {
             emit togglePower(1);
             flag4 = 0;
             state4 = 0;       }
    } else if (bleftFootcomAction == "Increment") {
        if (value4 == 1 && flag4 == 0 && state4 == 0) {
             state4 = 1;           }
         if (value4 == 0 && flag4 == 0 && state4 == 1) {
             flag4 = 1;
             state4 = 2;
             emit moveTopToBottom(value4);
         }
        if (value4 == 1 && flag4 == 1 && state4 == 2) {
             state4 = 3;
     }
    if (value4 == 0 && flag4 == 1 && state4 == 3) {
            emit moveTopToBottom(value4);
             flag4 = 0;
             state4 = 0;       }
    } else if (bleftFootcomAction == "Decrement") {
        if (value4 == 1 && flag4 == 0 && state4 == 0) {
             state4 = 1;           }
         if (value4 == 0 && flag4 == 0 && state4 == 1) {
             flag4 = 1;
             state4 = 2;
             emit moveBottomToTop(value4);
         }
        if (value4 == 1 && flag4 == 1 && state4 == 2) {
             state4 = 3;
     }
    if (value4 == 0 && flag4 == 1 && state4 == 3) {
            emit moveBottomToTop(value4);
             flag4 = 0;
             state4 = 0;       }
    } else if (bleftFootcomAction == "PDM") {
        if (value4 == 1 && flag4 == 0 && state4 == 0) {
             state4 = 1;           }
         if (value4 == 0 && flag4 == 0 && state4 == 1) {
             flag4 = 1;
             state4 = 2;
             emit powerdm(value4);
             emit powerdm1(value4);
             emit powerdm2(value4);
             emit powerdm3(value4);
         }
        if (value4 == 1 && flag4 == 1 && state4 == 2) {
             state4 = 3;
     }
    if (value4 == 0 && flag4 == 1 && state4 == 3) {
        emit powerdm(value4);
        emit powerdm1(value4);
        emit powerdm2(value4);
        emit powerdm3(value4);
             flag4 = 0;
             state4 = 0;       }
    }else if(ui->bleft_footcom->currentText() == "Reflux" || bleftFootcomAction == "Reflux"){
        if (value4 == 1 && flag4 == 0 && state4 == 0) {
             state4 = 1;           }
         if (value4 == 0 && flag4 == 0 && state4 == 1) {
             flag4 = 1;
             state4 = 2;
             emit performReflux(value4);
         }
        if (value4 == 1 && flag4 == 1 && state4 == 2) {
             state4 = 3;
     }
    if (value4 == 0 && flag4 == 1 && state4 == 3) {
            emit performReflux(value4);
             flag4 = 0;
             state4 = 0;       }
    }


}

void footpedal::receivedval(const int &text, const int &text1, const int &text2, const int &text3)
{
   f1=text;
   f2=text1;
   f3=text2;
   f4=text3;
}




void footpedal::exportGPIO(int gpioNumber)
{
    QFile file("/sys/class/gpio/export");
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << gpioNumber;
        file.close();
    }
}

void footpedal::setGPIODirection(int gpioNumber, const QString &direction)
{
    QFile file(QString("/sys/class/gpio/gpio%1/direction").arg(gpioNumber));
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << direction;
        file.close();
    }
}

void footpedal::writeGPIOValue(int gpioNumber, int value)
{
    QFile file(QString("/sys/class/gpio/gpio%1/value").arg(gpioNumber));
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << value;
        file.close();
    }
}

int footpedal::readGPIOValue(int gpioNumber)
{
    QFile file(QString("/sys/class/gpio/gpio%1/value").arg(gpioNumber));
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        int value;
        stream >> value;
        file.close();
        return value;
    }
    return -1;
}


void footpedal::updateFootpedalComboBoxes(const QString &surgeonName) {

    QSqlQuery query(db);
    query.prepare(
        "SELECT footleft, footright, footbottomleft, footbottomright "
        "FROM phacohigh "
        "WHERE surgeon = :surgeon"
    );
    query.bindValue(":surgeon", surgeonName);

  //  //qDebug() << "Selected surgeon in footpedal and fetching data for surgeon:" << surgeonName;

    if (!query.exec()) {
     //   //qDebug() << "Failed to fetch footpedal data for surgeon:" << query.lastError().text();
        return;
    }

    if (query.next()) {
        // Retrieve values from the query result
        QString footLeftValue = query.value("footleft").toString();
        QString footRightValue = query.value("footright").toString();
        QString footBLeftValue = query.value("footbottomleft").toString();
        QString footNRightValue = query.value("footbottomright").toString();

       //  Debug output to check retrieved values
//        qDebug() << "Foot pedal values retrieved:"
//                 << "footleft:" << footLeftValue
//                 << "footright:" << footRightValue
//                 << "footbleft:" << footBLeftValue
//                 << "footnright:" << footNRightValue;

        // Set the retrieved values to the combo boxes
        ui->left_footcom->setCurrentText(footLeftValue);
        ui->right_footcom->setCurrentText(footRightValue);
        ui->bleft_footcom->setCurrentText(footBLeftValue);
        ui->bright_footcom->setCurrentText(footNRightValue);

        // Debug output to confirm the values were set correctly
//        qDebug() << "ComboBox values set:"
//                 << "left_footcom:" << ui->left_footcom->currentText()
//                 << "right_footcom:" << ui->right_footcom->currentText()
//                 << "bleft_footcom:" << ui->bleft_footcom->currentText()
//                 << "bright_footcom:" << ui->bright_footcom->currentText();
    }
    db.close();
}


void footpedal::combobox1(const QString &text)
{
  ui->left_footcom->setCurrentText(text);
}

void footpedal::combobox2(const QString &text)
{
   ui->right_footcom->setCurrentText(text);
}

void footpedal::combobox3(const QString &text)
{
    ui->bleft_footcom->setCurrentText(text);
}

void footpedal::combobox4(const QString &text)
{
    ui->bright_footcom->setCurrentText(text);
}
void footpedal::updateFootpedalComboBoxes1(const QString &surgeonName)
{
    currentSurgeonName = surgeonName;  // Assign the passed surgeonName to currentSurgeonName
       //qDebug() << "Updated currentSurgeonName to:" << currentSurgeonName;
}
void footpedal::on_pushButton_clicked()
{
    QSqlDatabase db = QSqlDatabase::database(); // Use an existing connection if available

    // Ensure the database connection is valid
    if (!db.isValid()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(PATH);
    }

    // Check if the database connection is open
    if (!db.isOpen()) {
       // qDebug() << "Database is not open. Attempting to open it...";
        if (!db.open()) {
         //   qDebug() << "Failed to open the database:" << db.lastError().text();
            return;
        }
    }

    // Retrieve values from the combo boxes
    QString footLeftValue = ui->left_footcom->currentText();
    QString footRightValue = ui->right_footcom->currentText();
    QString footBLeftValue = ui->bleft_footcom->currentText();
    QString footNRightValue = ui->bright_footcom->currentText();



    // Prepare the SQL query with the correct syntax
    QSqlQuery query(db);
    query.prepare(
        "UPDATE phacohigh "
        "SET footleft = :footleft, "
        "footright = :footright, "
        "footbottomleft = :footbleft, "
        "footbottomright = :footnright "
        "WHERE surgeon = :surgeon"
    );

    // Bind values to the query
    query.bindValue(":footleft", footLeftValue);
    query.bindValue(":footright", footRightValue);
    query.bindValue(":footbleft", footBLeftValue);
    query.bindValue(":footnright", footNRightValue);

    query.bindValue(":surgeon", currentSurgeonName);

    // Execute the query and check for errors
    if (!query.exec()) {
        //qDebug() << "Failed to update footpedal data for surgeon:" << query.lastError().text();
        return;
    }

   // qDebug() << "Footpedal settings saved successfully for surgeon:" << currentSurgeonName;

    emit sendleftfootdoc(footLeftValue);
    emit sendrightfootdoc(footRightValue);
    emit sendbottomleftddoc(footBLeftValue);
    emit sendbottomrightdoc(footNRightValue);
    // Read GPIO values and close the window
    readInitialGPIOValues();
    emit activatemain();
    this->close();
}
