#include "ParserConfig.h"

#include <QSettings>
#include <QFile>
#include <QDebug>

ParserConfig::ParserConfig()
{

}

void ParserConfig::parseConfig(){
    if(QFile("config.ini").exists())
    {
        QSettings settings("config.ini",QSettings::IniFormat);

        settings.sync();
        settings.beginGroup("INFO");
        version = settings.value("version",8).toString();
        settings.endGroup();
        settings.beginGroup("BLUETOOTH");
        bluetooth_baudrate= settings.value("baudrate",8).toInt();
        bluetooth_databits= settings.value("databits",8).toInt();
        bluetooth_parity= settings.value("parity",8).toString();
        bluetooth_stopbits= settings.value("stopbits",8).toInt();
        bluetooth_flowcontrol= settings.value("flowcontrol",8).toString();
        settings.endGroup();
    }
    else
        qDebug() << "The path to the config file doesn't exist";
}
void ParserConfig::writeConfig(){
    if(QFile("config.ini").exists())
    {
        QSettings settings("config.ini",QSettings::IniFormat);

        settings.sync();
        settings.beginGroup("BLUETOOTH");
        settings.setValue("baudrate", QString::number(bluetooth_baudrate));
        settings.setValue("databits", QString::number(bluetooth_databits));
        settings.setValue("parity", bluetooth_parity);
        settings.setValue("stopbits", QString::number(bluetooth_stopbits));
        settings.setValue("flowcontrol", bluetooth_flowcontrol);
        settings.endGroup();

    }
    else
        qDebug() << "The path to the config file doesn't exist";
}
