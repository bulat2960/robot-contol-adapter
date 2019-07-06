#ifndef PLANNER_H
#define PLANNER_H

#include <QTcpSocket>
//#include <QtWidgets>

/**
 * It's a mock-object of Planner
 * Can connect to RCA and send him messages.
**/

class Planner : public QObject //public QWidget // Inherited from QWidget, can create simple window with buttons
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

    // Text edit and button for send message
    //QTextEdit* textEdit;
    //QPushButton* sendMsgButton;

    // Contains planner name
    QByteArray name;
public:
    // Basic constructor
    Planner(QString name, QString rcaIp, quint16 rcaPort);
public slots:
    // Slots for necessary actions
    void connectToServer();
    void sendName();
    void sendMsg();
    void disconnectFromServer();
};

#endif // PLANNER_H
