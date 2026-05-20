#include "prime.h"
#include "ui_prime.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QTimer>
#include <QFile>
#include <QImage>
#include <QDebug>
#include <QIODevice>
#include <QSqlError>

prime::prime(MainWindow *mainWin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::prime),
    m_mainWin(mainWin),
    timer1(nullptr),
    pretimer(nullptr),
    cleantimer(nullptr),
    statusUpdateTimer(nullptr),
    hand(nullptr),
    tune(nullptr),
    sur(nullptr),
    nFsCount(0)
{
    ui->setupUi(this);
    move(0,0);

    sur  = new doctor;
    hand = new hwhandler;
    tune = new tuning(m_mainWin, this);  // pass real mainWin — fixes nullptr crash

    cleantimer        = new QTimer(this);
    timer1            = new QTimer(this);
    pretimer          = new QTimer(this);
    statusUpdateTimer = new QTimer(this);

    // Open DB once for the lifetime of this object
    db = QSqlDatabase::addDatabase("QSQLITE", "prime_connection");
    db.setDatabaseName(PATH);
    if (!db.open()) {
        qDebug() << "prime: Failed to open DB:" << db.lastError().text();
    }

    // When tuning finishes, hide tune and restore prime
//    connect(tune, &tuning::activatemain, this, [this]() {
//        tune->hide();
//        this->show();
//        this->raise();
//        this->activateWindow();
//        current(1);
//    });

    // In prime.cpp constructor — add this connection
    connect(m_mainWin, &MainWindow::emittuning, this, [this]() {
        this->show();
        this->raise();
        this->activateWindow();
        on_Tune_but_clicked();   // switches to the tune tab inside prime
    });
    hand->phaco_off();
    hand->phaco_power(0);
    hand->freq_count(0);
    ui->progressBar_2->setRange(0,100);
    hand->fs_count_limit(0);
    hand->phaco_off();
    hand->freq_count(0);
    hand->safetyvent_off();
    hand->pinchvalve_off();
    hand->speaker_off();
    ui->progressBar_2->hide();

    QString styleSheet = "QPushButton {"
                         "    font-family: Ubuntu;"
                         "    font-size: 40pt;"
                         "font:bold;"
                         "    background-color: transparent;"
                         "    image: url(:/images/primeddd.png);"
                         "    color: white;"
                         "    min-width: 380px;"
                         "    max-width: 380px;"
                         "    min-height: 200px;"
                         "    max-height: 200px;"
                         "    border-radius: 20px;"
                         "width: 401;"
                         "height:211;"
                         "}"
                         "QPushButton:focus {"
                         "    outline: none;"
                         "    border: none;"
                         "}";

    setLastSelectedValue();

    QString tabStyle = "QTabBar::tab:selected { background-color: black; color: #ffffff; border-radius:50px}";
    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setStyleSheet("QTabWidget::pane { border: 0; }");

    ui->tabWidget->setStyleSheet(tabStyle);
    ui->tabWidget->setTabText(0, "     PRIME        ");
    ui->tabWidget->setTabText(1, "     CLEAN        ");
    ui->tabWidget->insertTab(1, tune, "    TUNE       ");

    serialnumber();

    connect(ui->comboBox_4, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &prime::onComboBoxIndexChanged);
    connect(this, &prime::sendsurgeon_tune, tune, &tuning::rx_surgeonName);

    connect(ui->prime1_but, &QPushButton::clicked, this, &prime::Prime);
    connect(ui->clean_but,  &QPushButton::clicked, this, &prime::Clean);

    connect(ui->prime1_but, &QPushButton::clicked, [=](){
        ui->prime1_but->setStyleSheet(styleSheet);
        ui->prime1_but->move(20,70);
        ui->label->move(20,180);
        ui->prime1_but->raise();
        ui->label->raise();
        hand->buzz();
    });

    connect(ui->Tune_but, &QPushButton::clicked, [=](){
        ui->Tune_but->setStyleSheet(styleSheet);
        ui->Tune_but->move(20,240);
        ui->label_2->move(30,380);
        ui->Tune_but->raise();
        ui->label_2->raise();
        hand->buzz();
    });

    connect(ui->clean_but, &QPushButton::clicked, [=](){
        ui->clean_but->setStyleSheet(styleSheet);
        ui->clean_but->move(20,450);
        ui->label_3->move(30,580);
        ui->clean_but->raise();
        ui->label_3->raise();
        hand->buzz();
    });

    connect(timer1,   &QTimer::timeout, this, &prime::timer);
    connect(pretimer, &QTimer::timeout, this, &prime::primetimer);

    QDateTime date = QDateTime::currentDateTime();
    ui->lab_date->setText(date.toString("dd.MM.yyyy"));

    QTime time = QTime::currentTime();
    ui->lab_time->setText(time.toString("hh:mm:ss"));

    ui->progressBar_2->setRange(0, 100);
    current(1);
    on_Tune_but_clicked();
}

prime::~prime()
{
    if (timer1        && timer1->isActive())        timer1->stop();
    if (pretimer      && pretimer->isActive())      pretimer->stop();
    if (cleantimer    && cleantimer->isActive())    cleantimer->stop();
    if (statusUpdateTimer && statusUpdateTimer->isActive()) statusUpdateTimer->stop();

    disconnect();

    // Detach the QSqlDatabase member BEFORE calling removeDatabase
    if (db.isOpen()) db.close();
    db = QSqlDatabase();
    QSqlDatabase::removeDatabase("prime_connection");

    delete ui;
    delete timer1;
    delete pretimer;
}

bool prime::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::TouchBegin ||
        event->type() == QEvent::TouchUpdate ||
        event->type() == QEvent::TouchEnd)
    {
        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
        QList<QTouchEvent::TouchPoint> points = touchEvent->touchPoints();
        foreach (const QTouchEvent::TouchPoint &point, points) {
            if (point.state() == Qt::TouchPointPressed) { }
            if (point.state() == Qt::TouchPointReleased) { }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void prime::click()
{
    QString styleSheet = "QPushButton {"
                         "    font-family: Ubuntu;"
                         "    font-size: 20pt;"
                         "    image: url(:/images/tuneddd.png);"
                         "    background-color: transparent;"
                         "    color: white;"
                         "    min-width: 341px;"
                         "    max-width: 341px;"
                         "    min-height: 141px;"
                         "    max-height: 141px;"
                         "    border-radius: 40px;"
                         "}"
                         "QPushButton:focus {"
                         "    outline: none;"
                         "    border: none;"
                         "}";
    QString style  = "QLabel{ image: url(:/images/prime.png); background-color:transparent; border:none; width:71; height:71; }";
    QString style1 = "QLabel{ image: url(:/images/wifi.png);  background-color:transparent; border:none; width:71; height:71; }";
    QString style2 = "QLabel{ image: url(:/images/clean.png); background-color:transparent; border:none; width:71; height:71; }";

    ui->prime1_but->setStyleSheet(styleSheet);
    ui->Tune_but->setStyleSheet(styleSheet);
    ui->clean_but->setStyleSheet(styleSheet);
    ui->label->show();
    ui->label_2->show();
    ui->label_3->show();
    ui->label->setStyleSheet(style);
    ui->label_2->setStyleSheet(style1);
    ui->label_3->setStyleSheet(style2);
}

void prime::current(int tab)
{
    QString styleSheet = "QPushButton {"
                         "    font-family: Ubuntu;"
                         "    font-size: 40pt;"
                         "font:bold;"
                         "    image: url(:/images/primeddd.png);"
                         "    background-color: transparent;"
                         "    color: white;"
                         "    min-width: 380px;"
                         "    max-width: 380px;"
                         "    min-height: 200px;"
                         "    max-height: 200px;"
                         "}"
                         "QPushButton:focus {"
                         "    outline: none;"
                         "    border: none;"
                         "}";

    QString styleSheet1 = "QPushButton {"
                          "    font-family: Ubuntu;"
                          "    font-size: 20pt;"
                          "    image: url(:/images/tuneddd.png);"
                          "    background-color: transparent;"
                          "    color: black;"
                          "    min-width: 341px;"
                          "    max-width: 341px;"
                          "    min-height: 141px;"
                          "    max-height: 141px;"
                          "    font-weight: bold;"
                          "}"
                          "QPushButton:focus {"
                          "    outline: none;"
                          "    border: none;"
                          "}";

    QString style  = "QLabel{ image: url(:/images/prime.png); background-color:transparent; border:none; width:71; height:71; }";
    QString style1 = "QLabel{ image: url(:/images/wifi.png);  background-color:transparent; border:none; width:71; height:71; }";
    QString style2 = "QLabel{ image: url(:/images/clean.png); background-color:transparent; border:none; width:71; height:71; }";

    // Reset all buttons to default style
    ui->prime1_but->setStyleSheet(styleSheet1);
    ui->prime1_but->move(0,70);
    ui->Tune_but->setStyleSheet(styleSheet1);
    ui->Tune_but->move(0,280);
    ui->clean_but->setStyleSheet(styleSheet1);
    ui->clean_but->move(0,500);
    ui->label->setStyleSheet(style);
    ui->label_2->setStyleSheet(style1);
    ui->label_3->setStyleSheet(style2);

    if (tab == 0) {
        ui->prime1_but->setStyleSheet(styleSheet);
        ui->label->setStyleSheet(style);
        ui->prime1_but->move(0,10);
        ui->label->move(30,380);
        ui->prime1_but->raise();
    } else if (tab == 1) {
        ui->Tune_but->setStyleSheet(styleSheet);
        ui->Tune_but->move(20,240);
        ui->label_2->move(30,380);
        ui->label_2->setStyleSheet(style1);
        pretimer->stop();
        timer1->stop();
        ui->Tune_but->raise();
    } else if (tab == 2) {
        ui->clean_but->setStyleSheet(styleSheet);
        ui->label_3->setStyleSheet(style2);
        ui->clean_but->move(20,500);
        ui->label_3->move(30,580);
        pretimer->stop();
        timer1->stop();
        ui->clean_but->raise();
    }
}

void prime::onUpdateStatusTimeout() { }

void prime::timer() { }

void prime::primetimer()
{
    int value = ui->progressBar_2->value();
    if (value < 100) {
        ui->progressBar_2->setValue(value + 1);
    } else {
        pretimer->stop();
    }
}

void prime::on_start_prime_but_2_clicked()
{
    ui->progressBar_2->setValue(0);
    ui->start_check_2->setChecked(false);
    ui->motor_Check_2->setChecked(false);
    ui->wait_Check_2->setChecked(false);
    ui->done_Check_2->setChecked(false);

    if (pretimer->isActive()) pretimer->stop();

    hand->buzz();
    motoron();
    hand->pinchvalve_on();
}

void prime::start_irrigation()
{
    ui->start_check_2->setChecked(true);
    pretimer->start(420);
    QTimer::singleShot(15000, this, &prime::champer_Filled);
}

void prime::motoron()
{
    hand->write_motor(0x01, 0x03, 40);
    ui->motor_Check_2->setChecked(true);
    pretimer->start(420);
    QTimer::singleShot(15000, this, &prime::start_irrigation);
}

void prime::motoroff()
{
    hand->write_motor(0x00, 0x00, 0);
}

void prime::champer_Filled()
{
    ui->wait_Check_2->setChecked(true);
    pretimer->start(420);
    QTimer::singleShot(15000, this, &prime::done);
}

void prime::done()
{
    ui->done_Check_2->setChecked(true);
    hand->safetyvent_off();
    hand->pinchvalve_off();
    motoroff();
}

void prime::Prime()
{
    QString styleSheet = "QPushButton {"
                         "    font-family: Ubuntu;"
                         "    font-size: 20pt;"
                         "    image: url(:/images/tuneddd.png);"
                         "    background-color: transparent;"
                         "    color: black;"
                         "    min-width: 341px;"
                         "    max-width: 341px;"
                         "    min-height: 141px;"
                         "    max-height: 141px;"
                         "}"
                         "QPushButton:focus {"
                         "    outline: none;"
                         "    border: none;"
                         "}";
    QString style = "QLabel{ image: url(:/images/prime.png); background-color:transparent; border:none; width:71; height:71; }";

    ui->label->setStyleSheet(style);
    ui->tabWidget->setCurrentIndex(0);
    ui->prime1_but->raise();
    timer1->stop();
    ui->clean_but->setStyleSheet(styleSheet);
    ui->clean_but->move(0,500);
    ui->Tune_but->setStyleSheet(styleSheet);
    ui->Tune_but->move(0,280);
    ui->label->raise();
    ui->label_2->raise();
    ui->label_3->raise();
    ui->label_2->move(270,360);
    ui->label_3->move(270,570);
    cleantimer->stop();
    ui->start_check_2->setChecked(false);
    ui->motor_Check_2->setChecked(false);
    ui->wait_Check_2->setChecked(false);
    ui->done_Check_2->setChecked(false);
    ui->progressBar_2->setValue(0);
}

void prime::Tune()
{
    QString styleSheet = "QPushButton {"
                         "    font-family: Ubuntu;"
                         "    font-size: 60pt;"
                         "font:bold;"
                         "    background-color: transparent;"
                         "    image: url(:/images/primeddd.png);"
                         "    color: black;"
                         "    min-width: 411px;"
                         "    max-width: 411px;"
                         "    min-height: 241px;"
                         "    max-height: 241px;"
                         "    border-radius: 20px;"
                         "width: 401;"
                         "height:211;"
                         "}"
                         "QPushButton:focus {"
                         "    outline: none;"
                         "    border: none;"
                         "}";
    ui->Tune_but->setStyleSheet(styleSheet);
    ui->Tune_but->move(20,240);
    ui->label_2->move(30,380);
    ui->Tune_but->raise();
    ui->label_2->raise();
}

void prime::comboboxselected() { }

void prime::onComboBoxIndexChanged(int index)
{
    if (!db.isOpen()) {
        qDebug() << "prime::onComboBoxIndexChanged: DB not open";
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE phacohigh SET lastupdate = :index");
    query.bindValue(":index", index);

    if (!query.exec()) {
        qDebug() << "Error updating last selected index:" << query.lastError().text();
    }

    // Also refresh serial number when surgeon selection changes
    serialnumber();
}

void prime::setLastSelectedValue()
{
    if (!db.isOpen()) {
        qDebug() << "prime::setLastSelectedValue: DB not open";
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT lastupdate FROM phacohigh LIMIT 1");
    if (!query.exec()) {
        qDebug() << "Error retrieving last index:" << query.lastError().text();
        return;
    }

    if (query.next()) {
        int lastIndex = query.value(0).toInt();
        if (lastIndex >= 0 && lastIndex <= 4) {
            ui->comboBox_4->setCurrentIndex(lastIndex);
        }
    }
}

void prime::serialnumber()
{
    if (!db.isOpen()) {
        qDebug() << "prime::serialnumber: DB not open";
        return;
    }

    QString surgeonName = ui->comboBox_4->currentText();

    QSqlQuery query(db);
    query.prepare(
        "SELECT iNanoPlusVP_SerialNo, fzero, fone, ftwo "
        "FROM phacohigh "
        "WHERE surgeon = :surgeon"
    );
    query.bindValue(":surgeon", surgeonName);

    if (query.exec() && query.next()) {
        QString serialNumber = query.value(0).toString();
        int nfpzero = static_cast<int>((query.value("fzero").toInt() / 100.0) * 4090);
        int nfpone  = static_cast<int>((query.value("fone").toInt()  / 100.0) * 4090);
        int nfptwo  = static_cast<int>((query.value("ftwo").toInt()  / 100.0) * 4090);
        nFsCount = nfpzero + nfpone + nfptwo;
        ui->label_6->setText(serialNumber);
        emit sendsurgeon_tune(nFsCount);
    } else {
        ui->label_6->setText("N/A");
    }
}

void prime::on_Tune_but_clicked()
{
    QString styleSheet = "QPushButton {"
                         "    font-family: Ubuntu;"
                         "    font-size: 20pt;"
                         "    image: url(:/images/tuneddd.png);"
                         "    background-color: transparent;"
                         "    color: black;"
                         "    min-width: 341px;"
                         "    max-width: 341px;"
                         "    min-height: 141px;"
                         "    max-height: 141px;"
                         "}"
                         "QPushButton:focus {"
                         "    outline: none;"
                         "    border: none;"
                         "}";
    QString style1 = "QLabel{ image: url(:/images/wifi.png); background-color:transparent; border:none; width:71; height:71; }";

    ui->label_2->setStyleSheet(style1);
    ui->tabWidget->setCurrentIndex(1);
    ui->prime1_but->setStyleSheet(styleSheet);
    ui->prime1_but->move(0,80);
    ui->clean_but->setStyleSheet(styleSheet);
    ui->clean_but->move(0,500);
    ui->label->move(270,140);
    ui->label_3->move(270,570);
    ui->label->raise();
    ui->label_2->raise();
    ui->label_3->raise();
    cleantimer->stop();
    tune->updatehandpieceStatus();
    emit sendsurgeon_tune(nFsCount);
}

void prime::Clean()
{
    QString styleSheet = "QPushButton {"
                         "    font-family: Ubuntu;"
                         "    font-size: 20pt;"
                         "    image: url(:/images/tuneddd.png);"
                         "    background-color: transparent;"
                         "    color: black;"
                         "    min-width: 341px;"
                         "    max-width: 341px;"
                         "    min-height: 141px;"
                         "    max-height: 141px;"
                         "}"
                         "QPushButton:focus {"
                         "    outline: none;"
                         "    border: none;"
                         "}";
    QString style2 = "QLabel{ image: url(:/images/clean.png); background-color:transparent; border:none; width:71; height:71; }";

    ui->label_3->setStyleSheet(style2);
    ui->tabWidget->setCurrentIndex(2);
    pretimer->stop();
    ui->prime1_but->setStyleSheet(styleSheet);
    ui->prime1_but->move(0,80);
    ui->Tune_but->setStyleSheet(styleSheet);
    ui->Tune_but->move(0,280);
    ui->label->move(270,140);
    ui->label_2->move(270,360);
    ui->label->raise();
    ui->label_2->raise();
    ui->label_3->raise();
}

void prime::Start_Tune()
{
    motoroff();
    hand->pinchvalve_off();
    hand->buzz();

    if (cleantimer->isActive()) cleantimer->stop();

    ui->done_Check_2->setChecked(true);
}

void prime::on_begin_clean_but_2_clicked()
{
    hand->buzz();

    ui->start_check_2->setChecked(false);
    ui->motor_Check_2->setChecked(false);
    ui->wait_Check_2->setChecked(false);
    ui->done_Check_2->setChecked(false);
    ui->progressBar_2->setValue(0);

    if (pretimer->isActive()) pretimer->stop();

    motoron();
    hand->pinchvalve_on();

    cleantimer->start(60000);
    connect(cleantimer, &QTimer::timeout, this, &prime::Start_Tune);
}
