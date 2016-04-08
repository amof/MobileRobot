#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtSerialPort>
#include <QtSerialPort/QSerialPort>
#include <QByteArray>
#include <QDebug>

#include "crc.h"
#include "settingsdialog.h"

class Controller
{
public:
    Controller();
    qint8 controller_open();
    void controller_close();
    void controller_sendCommand(int command);
    quint8 read();
    void controller_configure(SettingsDialog::Settings settings);

    #define C_C_ACK		6
    #define C_C_NACK	21
    #define C_C_DC		17
    #define TRUE    1
    #define FALSE   0

private:
    void send(const QByteArray &data);
    quint8 analyze(char frame[], int length);

    CRC *crc;
    QVector<QString> controllerProperties;
    QSerialPort *bluetooth;

};

#endif // CONTROLLER_H
