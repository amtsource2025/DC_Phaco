#include "ltc2614.h"

#include<QDebug>

static const char *ltc2604_device = "/dev/spidev2.1";

ltc2614::ltc2614(QObject *parent) : QObject(parent)
{
   // device=SPI_DEVICE;
    bits=SPI_BITS_PER_WORD;
    speed=SPI_SPEED;

    openDevice();

}

ltc2614::~ltc2614() {
    close(fd);
}

void ltc2614::writeDAC(int channel,uint16_t value) {
    uint8_t tx[4];
    uint8_t rx[4] = {0};

   // qDebug()<<"ltc:value"<<value;
    tx[0] = 0x00;

    // First byte is 0x31
    tx[1] = channel;

    // Second byte contains bits 13-6 of the value
    tx[2] = (value >> 8) & 0x00FF;//HHEXA

    // Third byte contains bits 5-0 of the value, shifted left by 2, with 2 don't-care bits at the end
    tx[3] = (value) & 0x00FF;//LHEXA

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = 4,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

  //  qDebug()<<tx[0]<<tx[1]<<tx[2]<<tx[3];

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) {
        throw std::runtime_error("Failed to send SPI message");
    }

}

void ltc2614::writeDAC550x(int channel, uint16_t value)
{
    uint8_t tx[4];
       uint8_t rx[4] = {0};

       tx[0] = 0x00;

       // First byte is 0x31
       tx[1] = channel;

       // Second byte contains bits 13-6 of the value
       tx[2] = (value >> 6) & 0xFF;

       // Third byte contains bits 5-0 of the value, shifted left by 2, with 2 don't-care bits at the end
       tx[3] = (value << 2) & 0xFC;

       struct spi_ioc_transfer tr = {
           .tx_buf = (unsigned long)tx,
           .rx_buf = (unsigned long)rx,
           .len = 4,
           .speed_hz = speed,
           .bits_per_word = bits,
       };

      // qDebug()<<tx[0]<<tx[1]<<tx[2]<<tx[3]<<fd;

       if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) {
           throw std::runtime_error("Failed to send SPI message");
       }
}

void ltc2614::openDevice() {
    fd = open(ltc2604_device,O_RDWR);

    uint8_t mode;
   // qDebug()<<mode<<fd;


    if (fd < 0) {
        throw std::runtime_error("Failed to open SPI device");
    }

    // Set SPI mode
    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) {
        throw std::runtime_error("Failed to set write SPI mode.");
    }

    if (ioctl(fd, SPI_IOC_RD_MODE, &mode) < 0) {
        throw std::runtime_error("Failed to set read SPI mode.");
    }

    // Set bits per word
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
        throw std::runtime_error("Failed to set bits per word.");
    }

    if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0) {
        throw std::runtime_error("Failed to set bits per word.");
    }

    // Set max speed (Hz)
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        throw std::runtime_error("Failed to set max speed.");
    }

    if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0) {
        throw std::runtime_error("Failed to set max speed.");
    }

}
