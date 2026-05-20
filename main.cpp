#include "prime.h"
#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QProcess>
#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QCoreApplication>
#include "hwhandler.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    hwhandler h;
    h.speaker_off();

    // Single instance guard — check BEFORE creating any windows
    QSharedMemory sharedMemory;
    QSystemSemaphore semaphore("unique_application_semaphore", 1);

    if (!semaphore.acquire()) {
        return 0;
    }

    sharedMemory.setKey("unique_application_key");

    if (sharedMemory.attach()) {
        return 0;
    }

    if (!sharedMemory.create(1)) {
        return -1;
    }

    // Create MainWindow first (stays hidden — prime will show it when needed)
    MainWindow *mainWin = new MainWindow();

    // Pass mainWin into prime
    prime *p = new prime(mainWin);

    // Splash screen
    QLabel *splashLabel = new QLabel;
    QMovie *movie = new QMovie(":/images/spalshscreen.png");

    splashLabel->setFixedSize(1280, 720);
    splashLabel->move(0, 0);
    movie->setScaledSize(splashLabel->size());

    QObject::connect(movie, &QMovie::frameChanged, [=]() {
        splashLabel->setFixedSize(splashLabel->size());
    });

    splashLabel->setMovie(movie);
    movie->start();
    splashLabel->show();

    // After 5 seconds: close splash, show prime
    QTimer::singleShot(5000, splashLabel, SLOT(close()));
    QTimer::singleShot(5000, p, SLOT(show()));

    // Restart app on exit
    QObject::connect(&a, &QApplication::aboutToQuit, [&]() {
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    });

    return a.exec();
}
