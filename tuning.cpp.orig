#include "tuning.h"
#include "ui_tuning.h"

tuning::tuning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tuning)
{
    ui->setupUi(this);
    // Initialize the timer for updating the progress bar
    main=new MainWindow;
    timer = new QTimer(this);
    hand=new hwhandler;
    handpiece=new QTimer;
    vacSensor=new Vaccum;
    exportGPIO(960);
    setGPIODirection("in",960);
    readGPIOValue(960);
    connect(handpiece,&QTimer::timeout,this,&tuning::updatehandpieceStatus);
    handpiece->start(1);
     ui->But_value->hide();
    ui->But_value->setStyleSheet("font-size: 90px; font-weight: bold; color: white; background-color: transparent;");
    ui->But_Tune->show();
    currentCircle = 0;
    timer1=new QTimer;
    connect(timer1, &QTimer::timeout, this, &tuning::updateCircle);
    connect(ui->But_Tune,&QPushButton::clicked,this,&tuning::on_pushButton_clicked);
    connect(this,&tuning::activatemain,main,&MainWindow::disablefunction);
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
    int status = readGPIOValue(960);
    QString styleSheet4 = "QPushButton {"
          "image: url(:/images/connected.png);"
            "border:none;"
            "background-color:transparent;"
 "}";
    QString styleSheet5 = "QPushButton {"
            "image: url(:/images/notconnected.png);"
         "border:none;"
            "background-color:transparent;"
                                     "}";
   if(status==0)
   {
       ui->But_Handpiece->setStyleSheet(styleSheet4);
       ui->But_Tune->setEnabled(true);
       ui->But_Tune->setEnabled(true);
      // ui->But_Next->hide();
   }
   else
   {
     //   qDebug()<<status;
       ui->But_Handpiece->setStyleSheet(styleSheet5);
       ui->But_Tune->setEnabled(false);
       ui->But_Tune->setEnabled(false);
     //  ui->But_Next->show();

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
        pen.setColor(i < activeLines ? QColor("#00050B") : QColor("#e0e0e0")); // Active/Inactive colors
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
    int baseOffsetX = 430; // Base horizontal position (center of circles)
    int baseOffsetY = 340; // Base vertical position (center of circles)

    // Define colors for each circle
    QColor colors[14] = {
        QColor("#C0C0C0"), QColor("#C0C0C0"), QColor("#C0C0C0"), QColor("#C0C0C0"),
        QColor("#C0C0C0"), QColor("#C0C0C0"), QColor("#C0C0C0"), QColor("#C0C0C0"),
        QColor("#C0C0C0"), QColor("#C0C0C0"), QColor("#C0C0C0"), QColor("#C0C0C0"),
        QColor("#C0C0C0"), QColor("#33FF57") // The last color will always be green
    };
int calibration=4096/14;
   int fullCircle=m_value*14.0/calibration;

    // Draw each circle based on the currentCircle index (from inner to outer)
    for (int j = 0; j <= 14; ++j) {
        int radius = radii[j]; // Current circle radius (starting from inner, moving to outer)
     int lines=totalLines;
     if(radii[j]>fullCircle){
  break;
     }

        // Define the angle step for each line (360 degrees divided by total lines)
        double angleStep = 360.0 / totalLines;

        // Center for all circles is the same
        QPoint center(baseOffsetX, baseOffsetY); // Common center for all circles

        // Draw the line shapes around the full circle
        for (int i = 0; i < lines; ++i) {
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
            // Set the color for the entire circle if it is fully completed, else gray
            if (fullCircle) {
                pen.setColor(colors[j % 14]); // Assign color based on circle index
            } else {
                pen.setColor(QColor("#C0C0C0")); // Inactive lines (gray)
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
   // hand->buzz();
    if (!isRunning) { // Only start if the progress is not currently running
           ui->But_Tune->move(170, 280);
           ui->But_Handpiece->move(150,340);
           ui->But_value->show();

Tune_Phaco();
    }
}
//button value
void tuning::on_But_value_clicked()
{
  //  hand->buzz();
    if (!isRunning) { // Only start if the progress is not currently running
           ui->But_Tune->move(170, 280);
           ui->But_Handpiece->move(150,340);
           ui->But_value->show();

Tune_Phaco();
    }
}
// Slot for handling the button click
void tuning::on_pushButton_clicked()
{
   // hand->buzz();
    if (!isRunning) { // Only start if the progress is not currently running
           ui->But_Tune->move(170, 280);
           ui->But_Handpiece->move(150,340);
           ui->But_value->show();

Tune_Phaco();
    }
}

// Update the circle which reaches 100 and then moves to MainWindow
void tuning::updateProgress()
{
    m_value = 0; // Reset value
    ui->But_value->setText(QString::number(m_value)); // Update label to show 0
      ui->But_value->move(400, 270); // Move label if necessary

    ui->But_value->show(); // Show the label
    update();

    ui->But_Tune->move(170, 280); // Move button back to starting position
    ui->But_Tune->resize(541, 141); // Resize button back to original size
    ui->But_Handpiece->move(150,340);
    ui->But_Handpiece->resize(141,131);
   timer1->stop();

}
// Circular progress bar updating
int tuning::Tune_Phaco()
{
                                       updateProgress();
    // Start the timer if it's not running
    if (!isRunning) {
        isRunning = true; // Set the running status
        ui->But_value->show();
        ui->But_Tune->move(150, 260);
        ui->But_Tune->resize(271, 171);
        timer1->start(10);

         qDebug()<<"hand freq sended";
        // Loop until m_value reaches 100
        for (m_value; m_value <= 100; m_value++) {
            ui->But_value->setText(QString::number(m_value)); // Update label with current value
            int db_feed=vacSensor->convert(0XA7);

            hand->freq_count(2500);
             hand->phaco_on();
             hand->fs_count(1900);
             hand->phaco_power(100);
             hand->pdm_mode(1);
            update(); // Update the UI
            QCoreApplication::processEvents(); // Process events
            usleep(100000); // Sleep for a while to create the effect
        }
updateProgress();
isRunning=false;
emit activatemain();
main->show();
main->ULTRASONICBUT1();
main->setTuneMode();

    }
    return 0;
}

void tuning::on_But_Next_clicked()
{
   // hand->buzz();
    emit activatemain();
    main->show();
    main->DIATHERMYBUT();
}
