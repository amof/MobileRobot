#include "Controller.h"

/************************************************************************/
/* Constants and macros                                                 */
/************************************************************************/
#define controller_length_command   7
#define controller_BUFFER_SIZE      20

//States for the analyze function
#define state_START			0
#define state_TYPE			1
#define state_COMMAND		2
#define state_END			3

/************************************************************************/
/* Global variable                                                      */
/************************************************************************/

static const int f_STX=0x02;
static const int f_ETX=0x03;
static const int f_DLE=0x10;
static const int f_COMMAND=0x9C;

Controller::Controller()
{
    bluetooth= new QSerialPort();
}

qint8 Controller::controller_open(){

    qint8 status=FALSE;

    if(bluetooth->isOpen())bluetooth->close();
    if (bluetooth->open(QIODevice::ReadWrite)) {
        status=TRUE;
    }

    return status;

}
void Controller::controller_close(){
    if(bluetooth->isOpen())bluetooth->close();
}

void Controller::send(const QByteArray &data)
{
    if(bluetooth->isOpen()){
        bluetooth->write(data);
    }

}

quint8 Controller::read()
{
    quint8 response=0;
    if(bluetooth->isOpen()){

        if(bluetooth->bytesAvailable()>=controller_length_command){
            char buf[controller_BUFFER_SIZE];
            qint16 inBytes;

            inBytes = bluetooth->read(buf, controller_BUFFER_SIZE);

            qDebug()<<"[Controller] Launching analyze ...";
            response=analyze(buf, inBytes);
        }
    }
    return response;
}

void Controller::controller_configure(SettingsDialog::Settings settings){

    bluetooth->setPortName(settings.name);
    bluetooth->setBaudRate(settings.baudRate);
    bluetooth->setDataBits(settings.dataBits);
    bluetooth->setParity(settings.parity);
    bluetooth->setStopBits(settings.stopBits);
    bluetooth->setFlowControl(settings.flowControl);
}


void Controller::controller_sendCommand(int command)
{
    QByteArray framed_data(controller_length_command, 0);

    //1. Append start flag
    framed_data[0]=f_DLE;
    framed_data[1]=f_STX;

    //2. Append command + CRC
    //Command
    framed_data[2]=f_COMMAND;
    framed_data[3]=command;

    //CRC

    //3. Append end flag
    framed_data[5]=f_DLE;
    framed_data[6]=f_ETX;

    // Send framed_data
    send(framed_data);
    qDebug()<<"[Controller] Command sent : "<<framed_data;
}

quint8 Controller::analyze(char frame[], int length){
    int state=state_START;
    int buffer_rx_examine = 0;
    int isFrameValid = 0;
    quint8 message=0;


    while(buffer_rx_examine < length){

        switch(state){
            case state_START:
            {
                if(frame[buffer_rx_examine]==f_DLE && frame[buffer_rx_examine+1]==f_STX){ //If there is a valid Start
                    state=state_TYPE;
                    buffer_rx_examine+=2;
                }
                else{
                    qDebug()<<"[Controller] state_START sent: Wrong Start";
                    buffer_rx_examine++;
                }
                break;
            }
            case state_TYPE:
            {
                quint8 type=frame[buffer_rx_examine++];
                if(type==f_COMMAND){
                    state=state_COMMAND;
                }
                else{
                    qDebug()<<"[Controller] state_TYPE: Wrong Type";
                }
                break;
            }
            case state_COMMAND:
            {
                quint8 command=frame[buffer_rx_examine++];
                message=command;
                state=state_END;
            }

            case state_END:
            {
                //If there is a valid end
                if(frame[buffer_rx_examine++]==f_DLE && frame[buffer_rx_examine++]==f_ETX){
                    state=state_START;
                    isFrameValid=1;
                }
                else{
                    qDebug()<<"[Controller] state_END: Wrong END";
                }
                break;
            }

            default:
            state=state_START;
            qDebug()<<"[Controller] default: Unknown error";
            break;
        }
        //Frame was valid -> GO OUT
        if(isFrameValid==1){
            qDebug()<<"[Controller] FOUND Message : "<<message;

            break;
        }
    }

    return message;
}
