#include "footswitch.h"
#include "ui_footswitch.h"
#include<QPainter>
#include<QColor>
#include<QSqlError>
#include<QtMath>
footswitch::footswitch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::footswitch)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(PATH);

    if (!db.open()) {
        qWarning() << "Error: Could not open the database." << db.lastError().text();
    } else {
        //qDebug() << "Database opened successfully.";
    }
    connect(ui->lineEdit,&QLineEdit::textChanged,this,&footswitch::setValues);
    connect(ui->lineEdit_2,&QLineEdit::textChanged,this,&footswitch::setValues);
    connect(ui->lineEdit_3,&QLineEdit::textChanged,this,&footswitch::setValues);
    connect(ui->lineEdit_4,&QLineEdit::textChanged,this,&footswitch::setValues);
    ui->Right_footcom->hide();
    ui->LeftFoot_com->hide();
    ui->Bottom_leftcom->hide();
    ui->Bottom_rightcom->hide();
    ui->label_2->hide();
    setValues();
}

footswitch::~footswitch()
{
    delete ui;
    db.close();  // Close the database connection when the dialog is destroyed
}
void footswitch::setValues() {
    // Get values from line edits and validate input
    part1Value = qMin(ui->lineEdit->text().toInt(), 100); // Cap at 100
    part2Value = qMin(ui->lineEdit_2->text().toInt(), 100); // Cap at 100
    part3Value = qMin(ui->lineEdit_3->text().toInt(), 100); // Cap at 100

    // Calculate remaining value for lineEdit_4
    int totalValue = part1Value + part2Value + part3Value;

    // Ensure part4Value does not go below 0
    part4Value = qMax(0, 100 - totalValue); // Remaining value to make total 100

    // Set the calculated value to lineEdit_4
    ui->lineEdit_4->setText(QString::number(part4Value));

    // Optional: If you want to limit the line edit inputs directly
    // Validate the input and update the line edits accordingly
    ui->lineEdit->setText(QString::number(part1Value));
    ui->lineEdit_2->setText(QString::number(part2Value));
    ui->lineEdit_3->setText(QString::number(part3Value));

    update(); // Request a repaint
}


void footswitch::paintEvent(QPaintEvent *event) {
    //if (showingpaint) {
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect(30, -10, 800, 800); // Rectangle for pie
    QPointF center(430, 390); // Center of the pie

    int startAngle = 60 * 16; // Start at 60 degrees
    int spanAngle = -90 * 16; // Span of -90 degrees for each part

    // Helper lambda to calculate text position based on angle
    auto getTextPosition = [&](int start, int span, int value) -> QPointF {
        int angle = start + (span * value) / 200; // Middle of the slice
        qreal radians = angle / 16.0 * M_PI / 180.0; // Convert to radians
        qreal radius = 250; // Adjust radius for text placement
        return QPointF(center.x() + radius * cos(radians), center.y() - radius * sin(radians));
    };
    QFont font = painter.font();
       font.setPointSize(16); // Increase font size (you can adjust this value as needed)
       painter.setFont(font);
    // Part 1
    painter.setBrush(QColor("#62A0EA"));
    painter.drawPie(rect, startAngle, spanAngle * part1Value / 100);

    // Calculate and draw text for part 1
    QPointF part1TextPos = getTextPosition(startAngle, spanAngle, part1Value);
    painter.setPen(Qt::white);
    painter.drawText(part1TextPos, QString::number(part1Value) + "%");

    // Part 2
    painter.setBrush(QColor("#00B2FF"));
    int part2StartAngle = startAngle + spanAngle * part1Value / 100;
    painter.drawPie(rect, part2StartAngle, spanAngle * part2Value / 100);

    // Calculate and draw text for part 2
    QPointF part2TextPos = getTextPosition(part2StartAngle, spanAngle, part2Value);
    painter.setPen(Qt::white);
    painter.drawText(part2TextPos, QString::number(part2Value) + "%");

    // Part 3
    painter.setBrush(QColor("#007BFF"));
    int part3StartAngle = part2StartAngle + spanAngle * part2Value / 100;
    painter.drawPie(rect, part3StartAngle, spanAngle * part3Value / 100);

    // Calculate and draw text for part 3
    QPointF part3TextPos = getTextPosition(part3StartAngle, spanAngle, part3Value);
    painter.setPen(Qt::white);
    painter.drawText(part3TextPos, QString::number(part3Value) + "%");

    // Part 4
    painter.setBrush(QColor("#0056b3"));
    int part4StartAngle = part3StartAngle + spanAngle * part3Value / 100;
    painter.drawPie(rect, part4StartAngle, spanAngle * part4Value / 100);

    // Calculate and draw text for part 4
    QPointF part4TextPos = getTextPosition(part4StartAngle, spanAngle, part4Value);
    painter.setPen(Qt::white);
    painter.drawText(part4TextPos, QString::number(part4Value) + "%");
    // }
}

void footswitch::SurgeonFoot(const QString &surgeon)
{
    surgeonName = surgeon;

}

void footswitch::receivelineEditval(int &val1, int &val2, int &val3,int &val4)
{
    ui->lineEdit->setText(QString::number(val1));
    ui->lineEdit_2->setText(QString::number(val2));
    ui->lineEdit_3->setText(QString::number(val3));
    ui->lineEdit_4->setText(QString::number(val4));


}

void footswitch::rxfootpedalmodes(const QString &topleft, const QString &topright, const QString &bottomleft, const QString &bottomright)
{
    ui->But_topleft->setText(topleft);
    ui->But_topright->setText(topright);
    ui->But_bottomleft->setText(bottomleft);
    ui->But_bottomright->setText(bottomright);
    ui->LeftFoot_com->setCurrentText(topleft);
    ui->Right_footcom->setCurrentText(topright);
    ui->Bottom_leftcom->setCurrentText(bottomleft);
    ui->Bottom_rightcom->setCurrentText(bottomright);

}
void footswitch::on_But_save_clicked()
{
    int value1 = ui->lineEdit->text().toInt();
    int value2 = ui->lineEdit_2->text().toInt();
    int value3= ui->lineEdit_3->text().toInt();
    int value4 = ui->lineEdit_4->text().toInt();


    // Ensure 'tex' is properly initialized
    if (surgeonName.isEmpty()) {
        qWarning() << "Error: Surgeon name 'tex' is not initialized.";
        return;
    }
    if (!db.isOpen()) {
        db = QSqlDatabase::addDatabase("QSQLITE", "myConnection"); // Unique connection name
        db.setDatabaseName(PATH);

        if (!db.open()) {
            qWarning() << "Error: Could not open the database." << db.lastError().text();
            return;
        }
    }

    QSqlQuery query(db);  // Use the specific database connection
    if (!query.prepare("UPDATE phacohigh SET fzero = :value1, fone = :value2, ftwo = :value3, fthree = :value4, "
                       "footleft = :footleft, footright = :footright, footbottomleft = :footbottomleft, footbottomright = :footbottomright "

                       " WHERE surgeon = :surgeonName")) {
        qWarning() << "Error preparing query:" << query.lastError();
        return;
    }
    query.bindValue(":surgeonName", surgeonName);
    query.bindValue(":value1", value1);
    query.bindValue(":value2", value2);
    query.bindValue(":value3", value3);
    query.bindValue(":value4", value4);
    query.bindValue(":footleft", ui->LeftFoot_com->currentText());
    query.bindValue(":footright", ui->Right_footcom->currentText());
    query.bindValue(":footbottomleft", ui->Bottom_leftcom->currentText());
    query.bindValue(":footbottomright", ui->Bottom_rightcom->currentText());
    if (!query.exec()) {
        qWarning() << "Error: Could not update data in the database:" << query.lastError();
    } else {
        sendvaltomain(value1,value2,value3,value4);
        emit topleft(ui->LeftFoot_com->currentText());
        emit topright(ui->Right_footcom->currentText());
        emit bottomleft(ui->Bottom_leftcom->currentText());
        emit bottomright(ui->Bottom_rightcom->currentText());
        qDebug()<<"the left right bottom left bottom right is"<<ui->LeftFoot_com->currentText()<<ui->Right_footcom->currentText()<<ui->Bottom_leftcom->currentText()<<ui->Bottom_rightcom->currentText();

    }

    db.close();
    QSqlDatabase::removeDatabase("myConnection");
    this->close();
}




void footswitch::on_But_zero_clicked()
{
    int value=ui->lineEdit->text().toInt();
    value+=1;
    if(value>=100){
        value=100;

    }
    ui->lineEdit->setText(QString::number(value));

}

void footswitch::on_But_zerodown_clicked()
{
    int value=ui->lineEdit->text().toInt();
    value-=1;
    if(value<1){
        value=5;
        value--;
    }
    ui->lineEdit->setText(QString::number(value));
}

void footswitch::on_But_oneup_clicked()
{
    int value=ui->lineEdit_2->text().toInt();
    value+=1;
    if(value>=100){
        value=100;

    }
    ui->lineEdit_2->setText(QString::number(value));
}

void footswitch::on_But_onedown_clicked()
{
    int value=ui->lineEdit_2->text().toInt();
    value-=1;
    if(value<5){
        value=5;
        value--;
    }
    ui->lineEdit_2->setText(QString::number(value));
}

void footswitch::on_But_twoup_clicked()
{
    int value=ui->lineEdit_3->text().toInt();
    value+=1;
    if(value>=100){
        value=100;

    }
    ui->lineEdit_3->setText(QString::number(value));
}

void footswitch::on_But_twodown_clicked()
{
    int value=ui->lineEdit_3->text().toInt();
    value-=1;
    if(value<5){
        value=5;
        value--;
    }
    ui->lineEdit_3->setText(QString::number(value));
}

void footswitch::on_But_threeup_clicked()
{
    int value=ui->lineEdit_4->text().toInt();
    value+=1;
    if(value>=100){
        value=100;

    }
    ui->lineEdit_4->setText(QString::number(value));
}

void footswitch::on_But_threedown_clicked()
{
    int value=ui->lineEdit_4->text().toInt();
    value-=1;
    if(value<5){
        value=5;
        value--;
    }
    ui->lineEdit_4->setText(QString::number(value));
}

void footswitch::on_pushButton_clicked()
{
    this->close();
}

void footswitch::on_But_footswitch_clicked()
{
    showingpaint=!showingpaint;
    if(showingpaint){
        //    ui->label->setStyleSheet("border:none;background-color:transparent;image: url(:/images/footswitch.png);");
        //    ui->label->move(-30,10);
        //    ui->label->resize(531,731);
        //    ui->label_2->show();
        //    ui->label_2->move(400,350);
        //    ui->label_2->resize(71,61);
        //    ui->But_footswitch->move(100,310);
        //    ui->But_footswitch->resize(271,111);
        //    ui->But_topleft->move(330,230);//ok
        //    ui->But_topleft->resize(61,71);
        //    ui->But_topright->move(330,440);
        //    ui->But_topright->resize(61,71);
        //    ui->But_bottomleft->move(200,230);//ok
        //    ui->But_bottomleft->resize(61,71);
        //    ui->But_bottomright->move(205,440);//ok
        //    ui->But_bottomright->resize(61,71);

    }


}

void footswitch::on_But_topleft_clicked()
{
    ui->LeftFoot_com->showPopup();
    ui->Right_footcom->hidePopup();
    ui->Bottom_rightcom->hidePopup();
    ui->Bottom_leftcom->hidePopup();
    // Connect to the signal that triggers when the user selects an option from the combo box
    connect(ui->LeftFoot_com, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        ui->But_topleft->setText(ui->LeftFoot_com->currentText());
    });



}

void footswitch::on_But_topright_clicked()
{
    ui->Right_footcom->showPopup();
    ui->Bottom_rightcom->hidePopup();
    ui->Bottom_leftcom->hidePopup();
    ui->LeftFoot_com->hidePopup();
    // emit topright(ui->Right_footcom->currentText());
    connect(ui->Right_footcom, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        ui->But_topright->setText(ui->Right_footcom->currentText());
    });


}

void footswitch::on_But_bottomleft_clicked()
{
    ui->Bottom_leftcom->showPopup();
    ui->Right_footcom->hidePopup();
    ui->LeftFoot_com->hidePopup();
    ui->Bottom_rightcom->hidePopup();
    connect(ui->Bottom_leftcom, QOverload<const QString &>::of(&QComboBox::currentTextChanged), this, [this](const QString &text) {
        ui->But_bottomleft->setText(ui->Bottom_leftcom->currentText());
    });
}

void footswitch::on_But_bottomright_clicked()
{
    ui->Bottom_leftcom->hidePopup();
    ui->Right_footcom->hidePopup();
    ui->LeftFoot_com->hidePopup();
    ui->Bottom_rightcom->showPopup();
    connect(ui->Bottom_rightcom, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        ui->But_bottomright->setText(ui->Bottom_rightcom->currentText());
    });

}

void footswitch::on_But_save_2_clicked()
{
    // Check if the surgeon name is set
    if (surgeonName.isEmpty()) {
        qWarning() << "Error: Surgeon name 'tex' is not initialized.";
        return;
    }

    // Open the database connection if it's not already open
    if (!db.isOpen()) {
        db = QSqlDatabase::addDatabase("QSQLITE", "myConnection"); // Unique connection name
        db.setDatabaseName(PATH);

        if (!db.open()) {
            qWarning() << "Error: Could not open the database." << db.lastError().text();
            return;
        }
    }

    // Prepare the new values to be updated
    int dfp0 =4; // Replace with your actual new value
    int dfp1 =4; // Replace with your actual new value
    int dfp2 =46; // Replace with your actual new value
    int dfp3 =46; // Replace with your actual new value

    // Prepare the UPDATE query to save the new values
    QSqlQuery query(db);
    if (!query.prepare("UPDATE phacohigh SET fzero = :value1, fone = :value2, ftwo = :value3, fthree = :value4 WHERE surgeon = :surgeonName")) {
        qWarning() << "Error preparing query:" << query.lastError();
        return;
    }
    query.bindValue(":surgeonName", surgeonName);
    query.bindValue(":value1", dfp0);
    query.bindValue(":value2", dfp1);
    query.bindValue(":value3", dfp2);
    query.bindValue(":value4", dfp3);
    if (!query.exec()) {
        qWarning() << "Error: Could not update data in the database:" << query.lastError();
    }

    // Emit the new values to MainWindow after the update
    sendvaltomain(dfp0, dfp1, dfp2, dfp3);
    ui->lineEdit->setText(QString::number(dfp0));
    ui->lineEdit_2->setText(QString::number(dfp1));
    ui->lineEdit_3->setText(QString::number(dfp2));
    ui->lineEdit_4->setText(QString::number(dfp3));

    db.close();
    QSqlDatabase::removeDatabase("myConnection");
    this->close();
}
