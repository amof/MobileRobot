#ifndef CUSTOMLOGGER_H
#define CUSTOMLOGGER_H

#include <QtCore/QDebug>
#include <QtCore/QtGlobal>
#include <QtCore/QFile>
#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QtCore/QMessageLogger>
#include <QtCore/QMessageLogContext>
#include <QtCore/QtMessageHandler>
#include <QMutex>

class CustomLogger
{
public:
    CustomLogger();
    void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString msg);



};

#endif // CUSTOMLOGGER_H
