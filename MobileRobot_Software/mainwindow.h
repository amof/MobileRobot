#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTimer>

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
    void handleError(QSerialPort::SerialPortError error);
    void writeData(const QByteArray &data);
    void readData();
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent *event);





private:
    Ui::MainWindow *ui;
    SettingsDialog *settings;
    QTimer *timer ;
    QSerialPort *bluetooth;
};

#endif // MAINWINDOW_H
