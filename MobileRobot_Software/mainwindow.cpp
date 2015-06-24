#include "Mainwindow.h"
#include "ui_Mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionClose->setEnabled(false);

    bluetooth= new QSerialPort(this);
    settings = new SettingsDialog;
    timer = new QTimer(this);

    initActionsConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

    /** bluetooth_out Port --> ARDUINO */

void MainWindow::openSerialPort()
{
    if(bluetooth->isOpen())bluetooth->close();;

    SettingsDialog::Settings p = settings->settings();

    bluetooth->setPortName(p.name);
    bluetooth->setBaudRate(p.baudRate);
    bluetooth->setDataBits(p.dataBits);
    bluetooth->setParity(p.parity);
    bluetooth->setStopBits(p.stopBits);
    bluetooth->setFlowControl(p.flowControl);

    bluetooth->setFlowControl(p.flowControl);

    if (bluetooth->open(QIODevice::ReadWrite)) {
            ui->actionOpen->setEnabled(false);
            ui->actionClose->setEnabled(true);
            ui->actionConfiguration->setEnabled(false);
            ui->statusBar->showMessage(tr("Connected to Bluetooth (%1) in Read/Write Mode")
                                       .arg(p.name));
    } else {
        QMessageBox::critical(this, tr("Error"), bluetooth->errorString());
        ui->statusBar->showMessage(tr("Error while trying to open"));
    }
}

void MainWindow::closeSerialPort()
{
    bluetooth->close();
    ui->actionOpen->setEnabled(true);
    ui->actionClose->setEnabled(false);
    ui->actionConfiguration->setEnabled(true);
    ui->statusBar->showMessage(tr("Disconnected"));
}
void MainWindow::writeData(const QByteArray &data)
{
    if(bluetooth->isOpen()){
        bluetooth->write(data);
    }

}
void MainWindow::readData()
{
    if(bluetooth->isOpen()){
        QByteArray data = bluetooth->readAll();
        if(!data.isEmpty())
        ui->bluetooth_in_console->append(data);
    }


}
void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), bluetooth->errorString());
        closeSerialPort();
    }
}

    /** KEYS */

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch( event->key()){
        case Qt::Key_8:
            bluetooth->write("U#");
            break;
        case Qt::Key_2:
            bluetooth->write("D#");
            break;
        case Qt::Key_4:
            bluetooth->write("L#");
            break;
        case Qt::Key_6:
            bluetooth->write("R#");
            break;
        case Qt::Key_S:
            bluetooth->write("S#");
            break;
        }

}
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (!(event->isAutoRepeat()))
    bluetooth->write("S#");

}


    /** Connections */
void MainWindow::initActionsConnections()
{
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfiguration, SIGNAL(triggered()), settings, SLOT(show()));

    connect(timer, SIGNAL(timeout()), this, SLOT(readData()));
    timer->start(200);
}
