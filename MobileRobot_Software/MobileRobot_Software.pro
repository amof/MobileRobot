#-------------------------------------------------
#
# Project created by QtCreator 2015-06-20T11:05:54
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = MobileRobot_Software
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp \
    Controller.cpp \
    qcustomplot.cpp \
    CustomLogger.cpp \
    ParserConfig.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h \
    Controller.h \
    qcustomplot.h \
    CustomLogger.h \
    ParserConfig.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    images.qrc
