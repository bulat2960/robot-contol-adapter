#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H

#include <QTcpSocket>
#include <QtWidgets>
#include <QTime>

/**
 * It's a mock-object of ControlUnit
 * Can connect to RCA, send its name
 * He will be able to turn the message into JSON-packet soon (in development)
**/

class ControlUnit : public QWidget // Inherited from QWidget, can create simple window with buttons
{
    Q_OBJECT
private:
    // Allows connect to server
    QTcpSocket* socket;

    // Interactive buttons
    QPushButton* connectButton;
    QPushButton* disconnectButton;

    // Contains unit name
    QByteArray name;

    // Function execution time
    QTime timer;
public:
    // Basic c-tor
    ControlUnit(QString name);
public slots:
    // Slots for necessary actions
    void connectToServer();
    void sendName();
    void readyRead();
    void disconnectFromServer();
};

#endif // CONTROLUNIT_H
