#ifndef PLANNER_H
#define PLANNER_H

#include <QTcpSocket>
//#include <QtWidgets>

/**
 * It's a mock-object of Planner
 * Can connect to RCA and send him messages.
**/

class Planner : public QObject
{
    Q_OBJECT
private:
    // RCA ip and port
    QString rcaIp;
    quint16 rcaPort;

    // Allows connect to server
    QTcpSocket* socket;

    // Contains planner name
    QByteArray name;
public:
    // Basic constructor
    Planner(QString name, QString rcaIp, quint16 rcaPort);
    bool isConnected() const;
    bool isDisconnected() const;
public slots:
    // Slots for necessary actions
    bool connectToServer();
    void sendName();
    void sendMsg(QString msg);
    bool disconnectFromServer();
};

#endif // PLANNER_H
