#include "controlunit.h"

ControlUnit::ControlUnit(QString unitName, QString rcaIp, quint16 rcaPort)
{
    // Set RCA ip and port
    this->rcaIp = rcaIp;
    this->rcaPort = rcaPort;

    // Create socket, set name
    socket = new QTcpSocket(this);
    name.append(unitName);

    // Connect signals and slots
    connect(socket, &QTcpSocket::connected, this, &ControlUnit::sendName);
    connect(socket, &QTcpSocket::readyRead, this, &ControlUnit::readyRead);
}

bool ControlUnit::connectToServer()
{
    // Try to connect to server
    socket->connectToHost(rcaIp, rcaPort);
    if (socket->waitForConnected())
    {
        return true;
    }
    return false;
}

void ControlUnit::sendName()
{
    // Check the connection and send our name
    qDebug() << "Control Unit" << name << "- connection established";
    socket->write(name);
    socket->waitForBytesWritten();
}

void ControlUnit::readyRead()
{
    // Read what we received
    QByteArray data = socket->readAll();

    receivedMessages.push_back(QString(data));

    qDebug() << "ControlUnit" << name << "received message -" << data;

    if (data == "e")
    {
        disconnectFromServer();
    }
    else
    {
        socket->write(data);
        socket->waitForBytesWritten();
    }  
}

QString ControlUnit::getLastMessage() const
{
    return receivedMessages.size() > 0 ? receivedMessages.back() : "";
}

bool ControlUnit::disconnectFromServer()
{
    qDebug() << "Control Unit" << name << "- disconnect";
    socket->disconnectFromHost();
    if (socket->state() == QAbstractSocket::UnconnectedState || socket->waitForDisconnected())
    {
        return true;
    }
    return false;
}

bool ControlUnit::isConnected() const
{
    return socket->state() == QAbstractSocket::ConnectedState;
}

bool ControlUnit::isDisconnected() const
{
    return socket->state() == QAbstractSocket::UnconnectedState;
}
