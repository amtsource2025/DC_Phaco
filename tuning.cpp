#include "tuning.h"
#include "ui_tuning.h"

// Static handpiece number - persists across restarts via file storage
QString tuning::m_handpieceNo = "HP00";

// File path where the handpiece number is saved
static const QString HP_SAVE_FILE = "/home/tune/handpiece_no.txt";

// Load saved handpiece number from file (called once at startup)
static QString loadHandpieceNo()
{
    QFile file(HP_SAVE_FILE);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString val = in.readLine().trimmed();
        file.close();
        if (!val.isEmpty()) {
            return val;
        }
    }
    return "HP00"; // default if no file found
}

// Save handpiece number to file so it survives restart
static void saveHandpieceNo(const QString &hpNo)
{
    QFile file(HP_SAVE_FILE);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream out(&file);
        out << hpNo;
        file.close();
    }
}

tuning::tuning(MainWindow *mainWin, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tuning),
    main(mainWin)
{
    ui->setupUi(this);
    // Load saved handpiece number from file on every startup
    m_handpieceNo = loadHandpieceNo();
    // Initialize the timer for updating the progress bar
    //main=new MainWindow;
    timer = new QTimer(this);
    hand=new hwhandler;
    handpiece=new QTimer;
    vacSensor=new Vaccum;
    hand->phaco_off();
    hand->phaco_power(0);
    hand->fs_count_limit(0);
    hand->freq_count(0);
    exportGPIO(960);
    setGPIODirection("in",960);
    readGPIOValue(960);
    connect(handpiece,&QTimer::timeout,this,&tuning::updatehandpieceStatus);
    handpiece->start(500);
     ui->But_value->hide();
    ui->But_value->setStyleSheet("font-size: 90px; font-weight: bold; color: white; background-color: transparent;");
    ui->But_Tune->show();
    currentCircle = 0;
    timer1=new QTimer;
    connect(timer1, &QTimer::timeout, this, &tuning::updateCircle);
    connect(ui->But_Tune,&QPushButton::clicked,this,&tuning::on_pushButton_clicked);
    connect(this,&tuning::sendfreq,main,&MainWindow::nReceiveFreq);
    connect(this,&tuning::activatemain,main,&MainWindow::disablefunction);
    ui->lblTuned->hide();
    ui->butTuned->hide();
    ui->butTuned->setText("Tuned");
    connect(main,&MainWindow::emittuning,this,&tuning::alreadyTune);
   // ui->lblinfo->hide();

}

tuning::~tuning()
{
    delete ui;
}
void tuning::exportGPIO(int pin)
{
    QFile file("/sys/class/gpio/export");
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << pin;
        file.close();
    }
}

void tuning::setGPIODirection(const QString &direction,int pin)
{
    QFile file(QString("/sys/class/gpio/gpio%1/direction").arg(pin));
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << direction;
        file.close();

    }
}

int tuning::readGPIOValue(int pin)
{
    QFile file(QString("/sys/class/gpio/gpio%1/value").arg(pin));
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        int value;
        stream >> value;
        file.close();

        return value;
    }
}
void tuning::updatehandpieceStatus()
{
    // Read GPIO value from pin 960
    statushp = readGPIOValue(960);

    // Define styles for connected and not connected states
    QString styleSheetConnected = "QPushButton {"
                                  "image: url(:/images/connected.png);"
                                  "border:none;"
                                  "background-color:transparent;"
                                  "}";

    QString styleSheetNotConnected = "QPushButton {"
                                     "image: url(:/images/notconnected.png);"
                                     "border:none;"
                                     "background-color:transparent;"
                                     "}";

    // Debug: Log the current status of the handpiece
    // qDebug() << "The handpiece status from GPIO pin 960: " << statushp;

    if (statushp == 0) {
        // GPIO status 0 means handpiece is connected and ready for tune
        if (!isTuned) {
            // If tuning is not yet done, set status to "Ready For Tune"
            ui->But_Handpiece->setStyleSheet(styleSheetConnected);
            ui->But_Tune->setEnabled(true);  // Enable the tune button
            ui->But_value->setEnabled(true); // Enable the value button
            ui->ButRTune->setText("Ready For Tune");
            ui->ButRTune->setStyleSheet("background-color: rgb(12, 40, 82); color: rgb(255,255,255); font: 20pt; font: bold;");
            ui->lblRTune->setStyleSheet("image: url(:/images/singletick.png); background-color: transparent; border: none;");
            ui->butTuned->hide();
            ui->lblTuned->hide();
            ui->ButRTune->show();
            ui->lblRTune->show();
        } else {
            // If already tuned, show "Already Tuned"
            //ui->But_Handpiece->setStyleSheet("border:none;background-color:transparent;image: url(:/images/doubled.png);outline:none");
            ui->But_value->show();  // Show the value button
            ui->butTuned->show();  // Show the 'Tuned' button
            ui->lblTuned->show();  // Show the 'Tuned' label
            ui->lblRTune->hide();  // Hide the 'RTune' label
            ui->ButRTune->hide();  // Hide the 'RTune' button

            ui->butTuned->setText("Already Tuned");  // Update text to "Already Tuned"
            ui->lblTuned->setStyleSheet("image: url(:/images/doubled.png);background-color:transparent;border:none;");
        }
        ui->butTuned->setEnabled(true);
        ui->ButRTune->setEnabled(true);
        ui->But_Handpiece->setEnabled(true);


    } else if (statushp == 1) {
        // GPIO status 1 means no handpiece connected
        isTuned = false;  // No tuning possible, so set isTuned to false
        ui->But_Handpiece->setStyleSheet(styleSheetNotConnected);
        ui->But_Tune->setEnabled(false);  // Disable tune button when no HP connected
        ui->But_value->hide();  // Hide the value button
        ui->But_Tune->move(170, 300);  // Move button back to starting position
        ui->But_Tune->resize(541, 141);  // Resize button back to original size
        ui->But_Handpiece->show();  // Show the handpiece button

        ui->ButRTune->setText("No HP Connected!");
        ui->ButRTune->setStyleSheet("background-color: rgb(12, 40, 82); color: rgb(255,255,255); font: 20pt; font: bold;");
        ui->lblRTune->setStyleSheet("image: url(:/images/notconnected.png); background-color: transparent; border: none;");

        // Update the status message for tuning
        ui->butTuned->setText("No HP Connected!");
        ui->butTuned->setStyleSheet("background-color: rgb(12, 40, 82); color: rgb(255,255,255); font: 20pt; font: bold;");
        ui->lblTuned->setStyleSheet("image: url(:/images/notconnected.png); background-color: transparent; border: none;");

        // Reset the previous GPIO value to 0
        previousGpioValue = 0;
        ui->butTuned->setEnabled(false);
        ui->ButRTune->setEnabled(false);
        ui->But_Handpiece->setEnabled(false);

        // Disable tuning mode function from main
        main->disablesetTunemode();

        // Set isRunning flag to false
        isRunning = false;
    }
}


void tuning::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    update(); // Trigger paint event on resize
}

void tuning::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#124E66")); // Replace with your color
    //painter.fillRect(rect(), QColor("#0C2852")); // Replace with your color
    // Ensure we handle the event properly
    if (!event) return; // Check for a valid event
    // Define the circle parameters
    int width = qMin(this->width(), this->height());
    QPoint center = rect().center();
    int radius = 200;
    // Calculate active lines based on progress
    int totalLines = 100;
    int activeLines = (m_value * totalLines) / 100;
    double angleStep = 360.0 / totalLines;

    // Draw lines around the circle
    for (int i = 0; i < totalLines; ++i) {
        double angle = angleStep * i - 270;
        double radAngle = qDegreesToRadians(angle);

        // Calculate line points
        int x1 = center.x() + (radius - 10) * qCos(radAngle);
        int y1 = center.y() + (radius - 10) * qSin(radAngle);
        int x2 = center.x() + radius * qCos(radAngle);
        int y2 = center.y() + radius * qSin(radAngle);

        // Set pen for lines
        QPen pen;
        pen.setWidth(8);
        pen.setColor(i < activeLines ? QColor("#e0e0e0") : QColor("#00050B")); // Active/Inactive colors
        painter.setPen(pen);
        painter.drawLine(x1, y1, x2, y2);
    }

    // Call your custom painting function
    paintEvent1(event); // Ensure this function is safe as well
}
void tuning::paintEvent1(QPaintEvent *event)
{
    Q_UNUSED(event); // Unused event parameter

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Define radii for each circle (smallest first for inner to outer drawing)
    int radii[14] = {30, 50, 70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290}; // Radii from smallest to largest
    int totalLines = 60; // Total number of lines for a full circle

    // Define a base center for positioning all circles
    int baseOffsetX = 440; // Base horizontal position (center of circles)
    int baseOffsetY = 390; // Base vertical position (center of circles)
    // int baseOffsetX = 430; // Base horizontal position (center of circles)
    // int baseOffsetY = 450; // Base vertical position (center of circles)

    // Define colors for each circle
    QColor colors[14] = {
        QColor("#FFCC00"), QColor("#FFFF00"), QColor("#FFFF00"), QColor("#FFFF00"),
        QColor("#FFCC00"), QColor("#FFFF00"), QColor("#FFFF00"), QColor("#FFFF00"),
        QColor("#FFCC00"), QColor("#FFFF00"), QColor("#FFFF00"), QColor("#FFFF00"),
        QColor("#FFCC00"), QColor("#FFFF00") // The last color will always be green
    };


    // Calculate how many circles are fully completed based on m_value
    //int completedCircles = ((nADCValue/14) * 20) / 100; // Assuming m_value is in percentage (0 to 100)
    //int completedCircles = ((nADCValue/4096) * 300.0); // Assuming m_value is in percentage (0 to 100)

    int circle=(int)(nADCValue-1024)*10.0/4096;


    // Draw each circle based on the currentCircle index (from inner to outer)
    for (int j = 0; j < circle; ++j) {
        //int completedCircles = ((nADCValue/4096) * 300.0);
        //int radius = radii[j]; // Current circle radius (starting from inner, moving to outer)
        int radius=radii[j];

        if(j>circle){
            break;
        }
        // Define the angle step for each line (360 degrees divided by total lines)
        double angleStep = 360.0 / totalLines;

        // Center for all circles is the same
        QPoint center(baseOffsetX, baseOffsetY); // Common center for all circles

        // Draw the line shapes around the full circle
        for (int i = 0; i < totalLines; ++i) {
            // Calculate the angle for the current line, starting from 0 degrees
            double angle = angleStep * i;

            // Convert angle to radians for trigonometric functions
            double radAngle = qDegreesToRadians(angle);
            int x1 = center.x() + (radius - 10) * qCos(radAngle);
            int y1 = center.y() + (radius - 10) * qSin(radAngle);
            int x2 = center.x() + radius * qCos(radAngle);
            int y2 = center.y() + radius * qSin(radAngle);

            // Set the pen for bold lines
            QPen pen;
  pen.setWidth(1);
            // Set the color for the entire circle if it is fully completed, else gray
            if (circle) {
                pen.setColor(colors[j % 14]); // Assign color based on circle index
            } else {
                pen.setColor(QColor("#FFFF00")); // Inactive lines (gray)
            }

            // Ensure the last circle is always green
            if (j == 13) {
                pen.setColor(QColor("#33FF57")); // Always green for the last circle
            }

            painter.setPen(pen); // Apply the pen settings
            painter.drawLine(x1, y1, x2, y2); // Draw the line
        }
    }

}
int tuning::ADC_value(){
    int value=vacSensor->convert(0XA7);
    return value;
}

void tuning::loose_tip()
{
    int value=Tune_Phaco();
    if(value==0){

    }
}

void tuning::updateCircle()
{
    if (currentCircle < 13) { // Ensure we stay within the bounds (0-4)
        currentCircle++; // Move to the next circle
    } else {
        currentCircle = 0; // Reset to the first circle
    }
    update(); // Repaint the widget to reflect changes
}
//handpiece
void tuning::on_But_Handpiece_clicked()
{
    if (!isRunning) { // Only start if the progress is not currently running
        ui->But_Tune->move(170,390);
    //       ui->But_Tune->move(150, 230);
        ui->But_Handpiece->hide();
           ui->But_value->setStyleSheet("font-size: 90px; font-weight: bold; color: white; background-color: transparent;");

           ui->But_value->show();
           ui->ButRTune->setText("Tuning");

Tune_Phaco();
    }
}
//button value
void tuning::on_But_value_clicked()
{
    if (!isRunning) { // Only start if the progress is not currently running
           ui->But_Tune->move(170,390);
           ui->But_Handpiece->hide();

           ui->But_value->setStyleSheet("font-size: 90px; font-weight: bold; color: white; background-color: transparent;");

           ui->But_value->show();
           ui->ButRTune->setText("Tuning");

Tune_Phaco();
    }
}
// Slot for handling the button click
void tuning::on_pushButton_clicked()
{
    if (!isRunning) { // Only start if the progress is not currently running
        ui->But_Tune->move(170,390);
        ui->But_Handpiece->hide();
        ui->But_value->setStyleSheet("font-size: 90px; font-weight: bold; color: white; background-color: transparent;");
        ui->But_value->show();
        ui->ButRTune->setText("Tuning");

Tune_Phaco();
    }
}

void tuning::rx_surgeonName(const int &text)
{
    surgeon=text;
    qDebug()<<"the value is received"<<text;
}

// Update the circle which reaches 100 and then moves to MainWindow
void tuning::updateProgress()
{
    m_value = 0; // Reset value
    ui->But_value->setText(QString::number(m_value)); // Update label to show 0
      ui->But_value->move(390,320); // Move label if necessary

    ui->But_value->show(); // Show the label
    update();
    ui->But_Tune->move(170, 290); // Move button back to starting position
   // ui->But_Tune->move(170, 280); // Move button back to starting position
    ui->But_Tune->resize(541, 141); // Resize button back to original size
  //  ui->But_Handpiece->show();
    // ui->But_Handpiece->move(270,360);
    // ui->But_Handpiece->resize(141,131);
    ui->lblRTune->setStyleSheet("image: url(:/images/singletick.png);background-color:transparent;");
    ui->lblRTune->move(150,620);
   ui->ButRTune->setText("Ready For Tune");
    ui->ButRTune->resize(521,41);

   timer1->stop();


}

int tuning::Tune_Phaco()
{

    int phacoPower=0;
    int nFreqCount;
    int loop1;
    long int nAveADC7841CurrentCount;
    int nADC7841CurrentCountPrev;
    int nADC7841CurrentCount1=0;
    int nADC7841CurrentCount[1024];
    int nInvADC7841CurrentCount[1024];
    int nResonantFreqCount;
    int nAveFeedBack;
    int iCount;
    int nNoOfCurrentCount;
    int ntmpDiffVolt;
    int loopcounter;
    int arrayCounter;
    static int fileCounter;
    float fResFrequency;
    bool trueFreqFound=false;
    int nLowCountValue=0;
    int nLowValueFreq=0;
    int i=0;
    int nResonantPosition;


    updateProgress();
    // Start the timer if it's not running
    if (!isRunning) {
        ui->But_Next->setEnabled(false);
        ui->But_value->show();
        isRunning = true; // Set the running status
          ui->But_Tune->move(170, 340);
        ui->But_Tune->move(150, 330);
        ui->But_Tune->resize(271, 171);
        timer1->start(100);
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString dateTimeString = currentDateTime.toString("yyyy-MM-dd_HH-mm-ss");  // e.g., 20240924_142530
        QDir dir = QDir("/home/tune/");
        QString fileName = dir.path()+"/" + QString("tuning_%1_%2.dat").arg(m_handpieceNo).arg(dateTimeString);

        QFile file(fileName);

        qDebug()<<fileName;

        QTextStream out(&file);

        file.open(QIODevice::WriteOnly | QIODevice::Text);
       // qDebug()<<"file opend";


        hand->phaco_power(0);
        usleep(100);
        //hand->phaco_on(0);

        hand->emitTuneStartPhaco();
        hand->phaco_on(TUNE_LOWERFREQ_COUNT);
        //usleep(500000);
        hand->phaco_power(100);usleep(100);

        nNoOfCurrentCount=0;
        // Loop until m_value reaches 100
        //qDebug()<<"PHACO_ON";
        for (iCount=TUNE_LOWERFREQ_COUNT; iCount >= TUNE_HIGHFREQ_COUNT; iCount--) {
            hand->freq_count(iCount);


            nAveADC7841CurrentCount =0;
            nADC7841CurrentCount1 =0;
            for(loop1=0;loop1<512;loop1++){
                nADC7841CurrentCount1 = vacSensor->convert(ADS7841_CURRENTSENSOR_CH2);
                nADCValue=nADC7841CurrentCount1;
                nAveADC7841CurrentCount += nADC7841CurrentCount1;

            }
            nADC7841CurrentCount[nNoOfCurrentCount] = (int) ((nAveADC7841CurrentCount/512.0));
            //qDebug()<<100000.0/(TUNE_LOWERFREQ_COUNT-nNoOfCurrentCount)<<nADC7841CurrentCount[nNoOfCurrentCount];
            // Update the UI
            nNoOfCurrentCount++;



            //m_value=(int)(nNoOfCurrentCount*90.0/(TUNE_LOWERFREQ_COUNT-TUNE_HIGHFREQ_COUNT));
            m_value=(int)(nNoOfCurrentCount*99.0/(TUNE_LOWERFREQ_COUNT-TUNE_HIGHFREQ_COUNT));
            ui->But_value->setText(QString::number(m_value)); // Update label with current value

            update();

            QCoreApplication::processEvents(); // Process events
           // usleep(5000); // Sleep for a while to create the effect
        }

        for(int nLoop=0;nLoop<nNoOfCurrentCount;nLoop++){
            nInvADC7841CurrentCount[nLoop]=4096-nADC7841CurrentCount[nLoop];
            out<<100000.0/(TUNE_LOWERFREQ_COUNT-nLoop)<<"\t"<<nInvADC7841CurrentCount[nLoop]<<"\n";

        }

        qDebug()<<"NoOfCurrentCount"<<nNoOfCurrentCount;
        for(i=0;i<nNoOfCurrentCount;i++)
        {
            //qDebug()<<i<<nInvADC7841CurrentCount[i] << nLowCountValue;
            if ((nInvADC7841CurrentCount[i] > nLowCountValue)){
                nLowCountValue =  nInvADC7841CurrentCount[i];
                nLowValueFreq = i;
                //qDebug()<<"Value="<<nLowValueFreq<<","<<nLowCountValue<<nInvADC7841CurrentCount[i];
            }
        }

        qDebug()<<"set object circle at " <<100000.0/(TUNE_LOWERFREQ_COUNT - nLowValueFreq)<<","<<nInvADC7841CurrentCount[nLowValueFreq]<<" radius 0.01"<<"\n";


        // With this:
        if (nLowValueFreq > 0){   // just ensure we found something
            for(i = nLowValueFreq; i > qMax(0, nLowValueFreq - 20); i--){
                qDebug() << trueFreqFound << i << nLowCountValue
                         << nLowValueFreq << nInvADC7841CurrentCount[i];

                if((nInvADC7841CurrentCount[i-1] <= nInvADC7841CurrentCount[i]) &&
                   ((nInvADC7841CurrentCount[i-1] <= 10) &&
                    (nInvADC7841CurrentCount[i+1] >= nInvADC7841CurrentCount[i]))) {

                    trueFreqFound = true;
                    nResonantFreqCount = TUNE_LOWERFREQ_COUNT - i;
                    nResonantPosition = i;
                    nLowCountValue = nInvADC7841CurrentCount[i];
                    nADC7841CurrentCountPrev = nADC7841CurrentCount[i];
                    break;
                } else {
                    trueFreqFound = false;
                }
            }

            // If still not found, just use the peak directly
            if(!trueFreqFound){
                nResonantPosition = nLowValueFreq;
                nResonantFreqCount = TUNE_LOWERFREQ_COUNT - nResonantPosition;
                trueFreqFound = true;
                qDebug() << "Using best found frequency:" << 100000.0/nResonantFreqCount;
            }

            qDebug() << "Resonant at " << 100000.0/(TUNE_LOWERFREQ_COUNT - nResonantPosition)
                     << nInvADC7841CurrentCount[nResonantPosition];
        }
        else{
            // Truly nothing found, still proceed to main with default
            nResonantPosition = nLowValueFreq;
            nResonantFreqCount = TUNE_LOWERFREQ_COUNT - nResonantPosition;
            qDebug() << "No peak found, using fallback frequency";
        }

//        if (nLowValueFreq > 50 && nLowValueFreq < 250){
//           for(i=nLowValueFreq;i>nLowValueFreq-20;i--){
//                qDebug()<<trueFreqFound<<i<<nLowCountValue<<nLowValueFreq<<nInvADC7841CurrentCount[i];
//               if((nInvADC7841CurrentCount[i-1] <= nInvADC7841CurrentCount[i])  &&
//                    ((nInvADC7841CurrentCount[i-1] <= 10) && (nInvADC7841CurrentCount[i+1] >= nInvADC7841CurrentCount[i]))) {

//                trueFreqFound = true;
//                nResonantFreqCount=TUNE_LOWERFREQ_COUNT-i;
//                nResonantPosition=i;
//                nLowCountValue =  nInvADC7841CurrentCount[i];
//                nADC7841CurrentCountPrev=nADC7841CurrentCount[i];
//                //qDebug()<<"set object circle at " <<100000.0/(TUNE_LOWERFREQ_COUNT - i)<<","<<nInvADC7841CurrentCount[i]<<" radius 0.01"<<"\n";
//                //qDebug()<<i<<100000.0/(TUNE_LOWERFREQ_COUNT - i)<<nInvADC7841CurrentCount[i]<<nADC7841CurrentCount[i];
//                break;
//               }
//               else{
//                // qDebug()<<nADC7841CurrentCount[i] << nADC7841CurrentCount[nLowValueFreq];
//                trueFreqFound = false;
//               }
//            }
//              qDebug()<<"set object circle at " <<100000.0/(TUNE_LOWERFREQ_COUNT - nResonantPosition)<<","<<nInvADC7841CurrentCount[nResonantPosition]<<" radius 0.01"<<"\n";


//        }
//        else{
//            file.close();
//            qDebug()<<"Error < 5";
//            ui->ButRTune->resize(681,41);
//            ui->lblRTune->move(10,620);
//            ui->lblRTune->setStyleSheet("image: url(:/images/information.png);background-color:transparent;");
//            ui->lblinfo->setText("May Be Loose Tip");
//         ui->ButRTune->move(80,621);
//            // Reset the text, size, and hide the label after 1 second
//                   QTimer::singleShot(1000, this, [this]() {
//                       hand->phaco_off();
//                       hand->phaco_power(0);

//                       isRunning=false;
//                       updateProgress();
//                       update();
//                   });
//                   hand->emitTuneStopPhaco();

//            trueFreqFound=false;
//           return -1;
//            isRunning = false;
//            hand->phaco_off();
//            main->show();
//            main->ULTRASONICBUT1();
//            main->activategpio();

//            main->setTuneMode();
//            updateProgress();
//             emit sendfreq(2781);
//            trueFreqFound=true;

//            return 0;
//        }


        if (nLowValueFreq < 5){
            qDebug() << "Warning: Low frequency position near boundary, continuing anyway";
            nResonantPosition = nLowValueFreq;
            nResonantFreqCount = TUNE_LOWERFREQ_COUNT - nResonantPosition;
        }


//        if (nLowValueFreq < 5){
//            qDebug()<<"Error < 5";
//            hand->phaco_off();
//            hand->phaco_power(0);
//            ui->lblinfo->setText("May Be Loose Tip");
//          /// ui->ButRTune->resize(621,41);
//            //ui->lblRTune->move(70,620);
//           // usleep(1000000);
//            ui->ButRTune->resize(441,41);
//            ui->ButRTune->move(130,620);
//            ui->But_value->setStyleSheet("border:none;background-color:transparent;image: url(:/images/singletick.png);outline:none");
//            qDebug()<<"remote crashedddddddd";
//            isRunning=false;
//            updateProgress();
//            update();
//           trueFreqFound=false;
//           hand->emitTuneStopPhaco();

////            trueFreqFound=true;
////            isRunning = false;
////            hand->phaco_off();
////            main->show();
////            main->ULTRASONICBUT1();
////            main->activategpio();

////            main->setTuneMode();
////            updateProgress();
////            emit sendfreq(2781);

////            return 0;
//           return -1;
//        }
        else{
            nResonantFreqCount=TUNE_LOWERFREQ_COUNT-nResonantPosition;
            qDebug()<<"set object circle at " <<100000.0/nResonantFreqCount<<","<<"10"<<" radius 0.01"<<"\n";

            fResFrequency = 100000.0/nResonantFreqCount;
            nADC7841CurrentCountPrev=nADC7841CurrentCount[nResonantPosition];
            qDebug()<<trueFreqFound<<fResFrequency<<nResonantFreqCount<<nResonantPosition;
            if ( nADC7841CurrentCountPrev >= 2048){

                hand->phaco_power(100);
                hand->freq_count(nResonantFreqCount);
                usleep(50);
                nAveFeedBack=0;
                for(iCount=0;iCount<100;iCount++){
                    nAveADC7841CurrentCount =0;
                    for(loop1=0;loop1<1024;loop1++){
                        nADC7841CurrentCount1=vacSensor->convert(ADS7841_CURRENTSENSOR_CH2);
                        nAveADC7841CurrentCount += nADC7841CurrentCount1;
                    }
                    nADC7841CurrentCount1 = (int) (nAveADC7841CurrentCount/1024.0);
                    nAveFeedBack += nADC7841CurrentCount1;
                    // qDebug()<<iCount<<nADC7841CurrentCount1;
                    //m_value=90+(int)(iCount/10.0);
                    //qDebug()<<m_value;
                    ui->But_value->setText(QString::number(m_value)); // Update label with current value
                    QCoreApplication::processEvents(); // Process events
                    usleep(100);
                }
                fResFrequency = (int)(nAveFeedBack/100.0);
                ntmpDiffVolt = abs(nADC7841CurrentCountPrev - fResFrequency);
                //qDebug()<<nADC7841CurrentCountPrev<<fResFrequency<<"diff "<<ntmpDiffVolt;
                hand->phaco_off();
                hand->emitTuneStopPhaco();
                hand->phaco_power(0);


                qDebug()<<"diff "<<ntmpDiffVolt;
                if(ntmpDiffVolt<500){
                    if(nADC7841CurrentCountPrev > 4095){

                        qDebug()<<"Tune Completed H"<<nResonantFreqCount;

                    }
                    else{
                        qDebug()<<"Tune Completed H"<<nResonantFreqCount;
                       // return nResonantFreqCount;

                    }
                    //nTuneFreq=nResonantFreqCount;


                }
                ui->lblinfo->setText("Tune is Completed H");

            }
            else{
                ui->lblinfo->setText("Tune is Completed L");

            }
        }
        out<<"set object circle at " <<100000.0/(TUNE_LOWERFREQ_COUNT - (nLowValueFreq+25))<<","<<nADC7841CurrentCount[nLowValueFreq+25]<<" radius 0.01\n";

        isRunning = false;
        ui->ButRTune->setText("Tune Completed.Ready For Phaco");
        hand->emitTuneStopPhaco();
        hand->phaco_off();
        hand->phaco_power(0);
        qDebug() << "[TUNING DONE]";
        main->show();
        main->ULTRASONICBUT1();
        main->activategpio();
        main->setTuneMode();
        hand->phaco_off();          // ADD THIS
        hand->phaco_power(0);       // ADD THIS - re-zero after setTuneMode
        qDebug() << "[TUNING DONE] ";
        updateProgress();
        emit sendfreq(nResonantFreqCount);
        emit activatemain();
  ui->But_Next->setEnabled(true);
        qDebug()<<nResonantFreqCount;
        if (statushp == 0 && !isTuned) {
               // Start tuning process here
               // After tuning, set the flag to indicate tuning is complete
               isTuned = true;  // Mark the tuning as complete
               updatehandpieceStatus();  // Call the update function to reflect the "Already Tuned" status
           }




        file.close();

    }

    return 0;

}

    void tuning::on_But_Next_clicked()
    {
        QString buttonstate = ui->butTuned->text();

        if (buttonstate == "Tuned" ||
            buttonstate == "No HP Connected!" ||
            buttonstate == "Ready For Tune")
        {
            qDebug() << "the button state is" << ui->butTuned->text();
            emit activatemain();      // → goes to MainWindow::disablefunction()
            main->show();
            main->raise();
            main->activateWindow();
            main->DIATHERMYBUT();
            main->disablegpio();
        }

        if (buttonstate == "Already Tuned")
        {
            emit activatemain();      // → goes to MainWindow::disablefunction()
            main->show();
            main->raise();
            main->activateWindow();
            main->ULTRASONICBUT1();
            main->activategpio();
        }
    }


void tuning::on_ButRTune_clicked()
{
    if (!isRunning) { // Only start if the progress is not currently running
        ui->But_Tune->move(170,390);
        ui->But_Handpiece->hide();
         ui->ButRTune->setText("Tuning");
        ui->But_value->setStyleSheet("font-size: 90px; font-weight: bold; color: white; background-color: transparent;");
        ui->But_value->show();


        Tune_Phaco();
    }
}


void tuning::on_butTuned_clicked()
{
    if (!isRunning) { // Only start if the progress is not currently running
        ui->But_Tune->move(170,390);
        ui->But_Handpiece->hide();

        ui->But_value->setStyleSheet("font-size: 90px; font-weight: bold; color: white; background-color: transparent;");

        ui->But_value->show();


        Tune_Phaco();
    }
}

void tuning::alreadyTune()
{
  previousGpioValue=1;
}

void tuning::setHandpieceNo(const QString &hpNo)
{
    if (!hpNo.trimmed().isEmpty()) {
        m_handpieceNo = hpNo.trimmed();
        saveHandpieceNo(m_handpieceNo); // ← save to disk immediately
    }
    // If empty, retain the previous value — do not overwrite
}
