#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTimer>
#include <QPixmap>
#include <QDebug>
#include <QMessageLogger>

#include "Controller.h"
#include "ParserConfig.h"
#include "settingsdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void initActionsConnections();
    void openSerialPort();
    void closeSerialPort();
    void readController();
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent *event);
    void view_home();
    void view_command();
    void view_sensors();
    void plotExample();
private:
    Ui::MainWindow *ui;
    SettingsDialog *settings;
    QTimer *timer ;
    Controller *controller;
    ParserConfig config;
};

#endif // MAINWINDOW_H
