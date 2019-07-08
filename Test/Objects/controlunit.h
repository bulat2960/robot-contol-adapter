#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H

#include <QTcpSocket>
//#include <QtWidgets>

/**
 * It's a mock-object of ControlUnit
 * Can connect to RCA, send its name
 * He will be able to turn the message into JSON-packet soon (in development)
**/

class ControlUnit : public QObject //: public QWidget // Inherited from QWidget, can create simple window with buttons
{
    Q_OBJECT
private:
    // RCA ip and port
    QString rcaIp;
    quint16 rcaPort;

    // Allows connect to server
    QTcpSocket* socket;

    // Interactive buttons
    //QPushButton* connectButton;
    //QPushButton* disconnectButton;

    // Contains unit name
    QByteArray name;
public:
    // Basic constructor
    ControlUnit(QString unitName, QString rcaIp, quint16 rcaPort);
public slots:
    // Slots for necessary actions
    bool connectToServer();
    void sendName();
    QString readyRead();
    bool disconnectFromServer();
};

#endif // CONTROLUNIT_H
