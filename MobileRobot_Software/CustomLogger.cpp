#include "CustomLogger.h"

#include <stdio.h>
#include <stdlib.h>

CustomLogger::CustomLogger()
{

}



void CustomLogger::myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString msg)
{
    QString timestr;
    timestr = QDateTime::currentDateTime().toString("[dd/MM/yy hh:mm:ss]");
    QByteArray txt;
    txt += timestr;
    // Switch structure left to be converted to write into the file in the future
    switch ( type )
    {
        case QtDebugMsg:
            txt += QString(" %1 ").arg("DEBUG", -8);
            break;
        case QtWarningMsg:
            txt += QString(" %1 ").arg("WARNING", -8);
            break;
        case QtCriticalMsg:
            txt += QString(" %1 ").arg("CRITICAL", -8);
            break;
        case QtFatalMsg:
            txt += QString(" %1 ").arg("FATAL", -8);
            break;
    }
    txt += QString("%1").arg(context.file, -15);
    txt += QString("%1").arg(context.function, -20);
    txt += QString("(%1):").arg(context.line, -3);
    txt += QString("%1").arg(msg);


    QFile outFile("debug.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream textStream(&outFile);
    textStream << txt << endl;

}
