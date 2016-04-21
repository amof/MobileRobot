#include "mainwindow.h"
#include "CustomLogger.h"
#include "ParserConfig.h"
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QThread>
#include <QFile>
#include <QtCore/QMessageLogger>
#include <QtCore/QMessageLogContext>
#include <QtCore/QtMessageHandler>

CustomLogger logTest;
ParserConfig config;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    logTest.myMessageOutput(type, context, msg); //*** comment out this line to remove the segfault
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Splash Screen - beginning
    QPixmap pixmap(":/images/robot_splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage(QObject::tr("Loading modules..."),Qt::AlignRight | Qt::AlignTop, Qt::white);
    a.processEvents();

    // Logger
    config.parseConfig();
    qInstallMessageHandler(myMessageOutput);
    QMessageLogger("main",25,"main").debug("Configuration loaded");

    // Style sheet load
    QFile File("mobileRobot.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    a.setStyleSheet(StyleSheet);

    //Splash Screen - ending
    QThread::sleep(1);
    splash.showMessage(QObject::tr("Loading done"),Qt::AlignRight | Qt::AlignTop, Qt::white);
    QThread::sleep(1);

    //Launch window
    MainWindow w;
    w.setWindowTitle("Mobile Robot by @mof v"+config.version);
    w.show();
    splash.finish(&w);

    return a.exec();
}
