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

    // Connect button
    connectButton = new QPushButton("ControlUnit " + name + ": Connect to Server", this);
    connectButton->setGeometry(0, 0, 400, 200);
    connect(connectButton, &QPushButton::clicked, this, &ControlUnit::connectToServer);

    // Disconnect button
    disconnectButton = new QPushButton("ControlUnit " + name + ": Disconnect from Server", this);
    disconnectButton->setGeometry(0, 200, 400, 200);
    connect(disconnectButton, &QPushButton::clicked, this, &ControlUnit::disconnectFromServer);
}

void ControlUnit::connectToServer()
{
    // Try to connect to server
    socket->connectToHost(rcaIp, rcaPort);
}

void ControlUnit::sendName()
{
    // Check the connection and send our name
    qDebug() << "Control Unit" << name << "- connection established";
    socket->write(name);
}

void ControlUnit::readyRead()
{
    // Read what we received
    QByteArray data = socket->readAll();

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

void ControlUnit::disconnectFromServer()
{
    qDebug() << "Control Unit" << name << "- disconnect";
    socket->disconnectFromHost();
}
