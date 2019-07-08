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

    /*
    // Connect button
    connectButton = new QPushButton("ControlUnit " + name + ": Connect to Server", this);
    connectButton->setGeometry(0, 0, 400, 200);
    connect(connectButton, &QPushButton::clicked, this, &ControlUnit::connectToServer);

    // Disconnect button
    disconnectButton = new QPushButton("ControlUnit " + name + ": Disconnect from Server", this);
    disconnectButton->setGeometry(0, 200, 400, 200);
    connect(disconnectButton, &QPushButton::clicked, this, &ControlUnit::disconnectFromServer);
    */
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
    if (socket->waitForDisconnected())
    {
        return true;
    }
    return false;
}
