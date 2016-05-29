#ifndef PARSERCONFIG_H
#define PARSERCONFIG_H

#include <QString>
#include "singleton.h"
#include <QObject>

class ParserConfig  : public QObject
{
public:
    ParserConfig(QObject* parent = 0);
    void parseConfig();
    void writeConfig();
    QString version;
    qint32 bluetooth_baudrate;
    qint32 bluetooth_databits;
    QString bluetooth_parity;
    qint32 bluetooth_stopbits;
    QString bluetooth_flowcontrol;

};

#define ParserConfigInstance Singleton<ParserConfig>::instance()

#endif // PARSERCONFIG_H
