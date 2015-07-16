#-------------------------------------------------
#
# Project created by QtCreator 2015-06-20T11:05:54
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MobileRobot_Software
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h \
    navcoms.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    images.qrc
