#ifndef FOOTLIB_H
#define FOOTLIB_H

#include <stdint.h>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include <QObject>
#include <QLabel>
#include <QDebug>

class footlib
{
public:
    footlib();
    ~footlib();
    void init();
    int convert(uint8_t channel);
    int stabilize();
private:
    int spi_fd;
};

#endif // FOOTLIB_H
