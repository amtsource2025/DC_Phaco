#ifndef HHANDLER_H
#define HHANDLER_H
#include <QThread>
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <QDebug>
#include <QFile>
#include <QCoreApplication>
#include <QEventLoop>
#include <QTime>
#include<QLineEdit>

#define XPAR_AXI_COMBINED_BASEADDR 0x43C20000;
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

#define VIT_ONOFF_REG               24
#define VIT_ONTIME_REG              25
#define VIT_COUNT_REG               26

#define ON_MASK                     0x01
#define OFF_MASK                    0x00

#define VSO_PWM_ON_REG              40
#define VSO_PWM_PERIOD_REG          42

#define DIA_ONOFF_REG               20
#define DIA_COUNT_REG               22

#define AI_ONOFF_REG                44
#define AI_PRESET_REG               48
#define AI_COUNT_REG                50

#define CHANNEL_0                   0x97
#define CHANNEL_1                   0xD7
#define CHANNEL_2                   0xA7
#define CHANNEL_3                   0xE7


#define REG1 32
#define REG2 36







class hhandler: public QThread
{
    Q_OBJECT
    int Flow_LUT[42]={95,95,  //0
                    110,110,  //2
                    112,112,  //4
                    115,115,  //6
                    122,122,  //8
                    130,130, //10
                    145,145, //12
                    155,150, //14
                    165,155, //16
                    175,160, //18
                    185,170, //20
                    195,195, //22
                    205,205, //24
                    215,215, //26
                    225,225, //28
                    245,245, //30
                    255,255, //32
                    265,265, //34
                    275,275, //36
                    290,290, //38
                    299,299  //40
                    };
public:
    explicit hhandler(QObject *parent = 0);
    int memfd;
    static void vit_on(int count);
    static void vit_off();
    static void vit_ontime(int ontime);

    static void vso_off();
    static void vso_ontime(int ontime);
    static void vso_period(int count);

    static void dia_on();
    static void dia_off();
    static void dia_count(int count);

    void write_motor(uint16_t status, uint16_t direction, uint16_t value);
    



signals:

};
#endif // HHANDLER_H
