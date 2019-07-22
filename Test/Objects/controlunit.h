#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H

#include <QTcpSocket>

/**
 * It's a mock-object of ControlUnit
 * Can connect to RCA, send its name
 * He will be able to turn the message into JSON-packet soon (in development)
**/

class ControlUnit : public QObject
{
    Q_OBJECT
private:
    // RCA ip and port
    QString rcaIp;
    quint16 rcaPort;

    // Allows connect to server
    QTcpSocket* socket;

    // Contains unit name
    QByteArray name;

    QVector<QString> receivedMessages;
public:
    // Basic constructor
    ControlUnit(QString unitName, QString rcaIp, quint16 rcaPort);
    bool isConnected() const;
    bool isDisconnected() const;
public slots:
    // Slots for necessary actions
    bool connectToServer();
    void sendName();
    void readyRead();
    void sendMsgToScene(QString message);
    QString getLastMessage() const;
    bool disconnectFromServer();
};

#endif // CONTROLUNIT_H
