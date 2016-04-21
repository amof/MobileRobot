#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtSerialPort>
#include <QtSerialPort/QSerialPort>
#include <QByteArray>
#include <QDebug>
#include <QMessageLogger>

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

    #define C_C_ACK		0x10
    #define C_C_NACK	0x15
    #define C_C_ALIVE   0x17
    #define C_C_END_CON 0x19
    #define C_M_STOP    0x20
    #define C_M_FW      0x21
    #define C_M_BW      0x22
    #define C_M_RIGHT   0x23
    #define C_M_LEFT    0x24
    #define TRUE    1
    #define FALSE   0

private:
    void send(const QByteArray &data);
    quint8 analyze(char frame[], int length);

    QVector<QString> controllerProperties;
    QSerialPort *bluetooth;

};

#endif // CONTROLLER_H
