#ifndef PARSERCONFIG_H
#define PARSERCONFIG_H

#include <QString>

class ParserConfig
{
public:
    ParserConfig();
    void parseConfig();
    void writeConfig();
    QString version;
    qint32 bluetooth_baudrate;
    qint32 bluetooth_databits;
    QString bluetooth_parity;
    qint32 bluetooth_stopbits;
    QString bluetooth_flowcontrol;

};

#endif // PARSERCONFIG_H
