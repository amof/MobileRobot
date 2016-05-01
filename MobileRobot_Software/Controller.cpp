#include "Controller.h"

/************************************************************************/
/* Constants and macros                                                 */
/************************************************************************/
#define controller_length_command   7
#define controller_BUFFER_SIZE      20

//States for the analyze function
#define state_START			0
#define state_FLAG			1
#define state_VERSION		2
#define state_LENGTH		3
#define state_COMMAND       4
#define state_END           5


/************************************************************************/
/* Global variable                                                      */
/************************************************************************/

static const int f_STX=0x02;
static const int f_ETX=0x03;
static const int f_FLAG=0xAA;
static const int f_version=0x01;

Controller::Controller()
{
    bluetooth= new QSerialPort();
}

qint8 Controller::controller_open(){

    qint8 status=FALSE;

    if(bluetooth->isOpen())bluetooth->close();
    if (bluetooth->open(QIODevice::ReadWrite)) {
        status=TRUE;
        controller_sendCommand(C_C_ALOHA);
    }

    return status;

}
void Controller::controller_close(){
    if(bluetooth->isOpen()){
        controller_sendCommand(C_C_END_CON);
        bluetooth->close();
    }
}

void Controller::send(const QByteArray &data)
{
    if(bluetooth->isOpen()){
        bluetooth->write(data);
    }

}

QList<quint8> Controller::read()
{
    QList<quint8> response;
    int position=0;
    while(1){

        response.append(255);
        if(bluetooth->isOpen()){
            response[position]=0;
            if(bluetooth->bytesAvailable()>=controller_length_command){
                char buf[controller_BUFFER_SIZE];
                qint16 inBytes;

                inBytes = bluetooth->read(buf, controller_length_command);

                qDebug()<<"[Controller] Launching analyze ...";
                response[position]=analyze(buf, inBytes);
                position++;
            }
            else{
                break;
            }
        }
        else{
            break;
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

    //1. Append start + flag
    framed_data[0]=f_STX;
    framed_data[1]=f_FLAG;
    framed_data[2]=f_FLAG;

    //2. Append version + length
    framed_data[3]=f_version;
    framed_data[4]=1;

    //3. Append command
    framed_data[5]=command;

    //4. Append end
    framed_data[6]=f_ETX;

    // Send framed_data
    send(framed_data);
    //QMessageLogger("Controller",103,"controller_sendCommand").debug("Command sent("+QString::number(command)+")");

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
                if(frame[buffer_rx_examine]==f_STX){ //If there is a valid Start
                    state=state_FLAG;
                    buffer_rx_examine+=1;
                }
                else{
                    qDebug()<<"[Controller] state_START sent: Wrong Start";
                    buffer_rx_examine++;
                }
                break;
            }
            case state_FLAG:
            {
                quint8 flag=frame[buffer_rx_examine++];
                quint8 flag_2=frame[buffer_rx_examine++];
                if(flag==f_FLAG && flag_2==f_FLAG){
                    state=state_VERSION;
                }
                else{
                    qDebug()<<"[Controller] state_FLAG: Wrong Flag";
                }
                break;
            }
            case state_VERSION:
            {
                quint8 version=frame[buffer_rx_examine++];
                if(version==f_version){
                    state=state_LENGTH;
                }
                else{
                    qDebug()<<"[Controller] state_VERSION: Wrong version";
                }
                break;
            }
            case state_LENGTH:
            {
                quint8 length=frame[buffer_rx_examine++];
                state=state_COMMAND;

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
                if(frame[buffer_rx_examine++]==f_ETX){
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
