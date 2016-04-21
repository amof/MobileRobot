#include "Mainwindow.h"
#include "ui_Mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionClose->setEnabled(false);

    settings = new SettingsDialog;
    timer = new QTimer(this);
    controller = new Controller();
    config.parseConfig();
    plotExample();
    ui->view->setCurrentIndex(0);
    ui->label_home_2->setText("Version: "+config.version);

    initActionsConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

    /** bluetooth_out Port --> ARDUINO */

void MainWindow::openSerialPort()
{

    SettingsDialog::Settings p = settings->settings();

    controller->controller_configure(p);

    if (controller->controller_open()==TRUE) {
            ui->actionOpen->setEnabled(false);
            ui->actionClose->setEnabled(true);
            ui->actionConfiguration->setEnabled(false);
            ui->statusBar->showMessage(tr("Connected to Bluetooth (%1) in Read/Write Mode").arg(p.name));
    } else {
        ui->statusBar->showMessage(tr("Error while trying to open"));
    }
}

void MainWindow::closeSerialPort()
{
    controller->controller_close();
    ui->actionOpen->setEnabled(true);
    ui->actionClose->setEnabled(false);
    ui->actionConfiguration->setEnabled(true);
    ui->statusBar->showMessage(tr("Disconnected"));
}

void MainWindow::readController(){
    controller->read();
}

/*void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), bluetooth->errorString());
        closeSerialPort();
    }
}*/

    /** KEYS */

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(ui->view->currentIndex()==1 && !(event->isAutoRepeat())){
        switch( event->key()){
            case Qt::Key_8:
                ui->direction_up->setPixmap(QPixmap(":/images/up-2.png"));
                QMessageLogger("MainWindow",74,"keyPressEvent").debug("Key up pressed");
                controller->controller_sendCommand(C_M_FW);
                break;
            case Qt::Key_2:
                ui->direction_down->setPixmap(QPixmap(":/images/down-2.png"));
                QMessageLogger("MainWindow",79,"keyPressEvent").debug("Key down pressed");
                controller->controller_sendCommand(C_M_BW);
                break;
            case Qt::Key_4:
                ui->direction_left->setPixmap(QPixmap(":/images/left-2.png"));
                QMessageLogger("MainWindow",83,"keyPressEvent").debug("Key left pressed");
                controller->controller_sendCommand(C_M_LEFT);
                break;
            case Qt::Key_6:
                ui->direction_right->setPixmap(QPixmap(":/images/right-2.png"));
                QMessageLogger("MainWindow",88,"keyPressEvent").debug("Key right pressed");
                controller->controller_sendCommand(C_M_RIGHT);
                break;
            }
    }

}
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(ui->view->currentIndex()==1&& !(event->isAutoRepeat())){
        QMessageLogger("MainWindow",97,"keyReleaseEvent").debug("Key released");
        controller->controller_sendCommand(C_M_STOP);
        ui->direction_down->setPixmap(QPixmap(":/images/down.png"));
        ui->direction_up->setPixmap(QPixmap(":/images/up.png"));
        ui->direction_left->setPixmap(QPixmap(":/images/left.png"));
        ui->direction_right->setPixmap(QPixmap(":/images/right.png"));
    }
}

void MainWindow::plotExample(){
    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i]; // let's plot a quadratic function
    }
    // create graph and assign data to it:
    ui->plotAccelerometer->addGraph();
    ui->plotAccelerometer->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->plotAccelerometer->xAxis->setLabel("x");
    ui->plotAccelerometer->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->plotAccelerometer->xAxis->setRange(-1, 1);
    ui->plotAccelerometer->yAxis->setRange(0, 1);
    ui->plotAccelerometer->replot();
}

void MainWindow::view_home(){
    ui->view->setCurrentIndex(0);
}
void MainWindow::view_command(){
    ui->view->setCurrentIndex(1);
}
void MainWindow::view_sensors(){
    ui->view->setCurrentIndex(2);
}


    /** Connections */
void MainWindow::initActionsConnections()
{
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionConfiguration, SIGNAL(triggered()), settings, SLOT(show()));

    connect(ui->btn_home, SIGNAL(clicked()), this, SLOT(view_home()));
    connect(ui->btn_cmd, SIGNAL(clicked()), this, SLOT(view_command()));
    connect(ui->btn_sensor, SIGNAL(clicked()), this, SLOT(view_sensors()));

}
