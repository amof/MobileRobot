#ifndef NAVCOMS
#define NAVCOMS

#endif // NAVCOMS

#include <QString>
#include <QByteArray>

namespace Ui {
class navcoms;
}

class navcoms {

public :

    struct COM {
        QString header;
        QString cmd;
        QByteArray data;
        quint8 checkSum;

    };

};
