#include "controlunit.h"

ControlUnit::ControlUnit(QString unitName)
{
    // Create socket, set name
    socket = new QTcpSocket(this);
    name.append(unitName);

    // Connect signals and slots
    connect(socket, SIGNAL(connected()), this, SLOT(sendName()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    //connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectFromServer()));

    // Connect button
    connectButton = new QPushButton("ControlUnit " + name + ": Connect to Server", this);
    connectButton->setGeometry(0, 0, 400, 200);
    connect(connectButton, SIGNAL(clicked()), this, SLOT(connectToServer()));

    // Disconnect button
    connectButton = new QPushButton("ControlUnit " + name + ": Disconnect from Server", this);
    connectButton->setGeometry(0, 200, 400, 200);
    connect(connectButton, SIGNAL(clicked()), this, SLOT(disconnectFromServer()));
}

void ControlUnit::connectToServer()
{
    // Try to connect to server
    socket->connectToHost("localhost", 5555);
}

void ControlUnit::sendName()
{
    // Check the connection and send our name
    qDebug() << "Control Unit" << name << ": connection established";
    socket->write(name);
}

void ControlUnit::readyRead()
{
    // Read what we received
    QByteArray data = socket->readAll();

    qDebug() << "ControlUnit" << name << "received message:" << data;

    if (data == "e")
    {
        disconnectFromServer();
    }
    else
    {
        // Some sommand from CUnit
        QByteArray s1 = "\"id\"";
        QByteArray s2 = "\"SomeData\"";
        socket->write(s1 + ":" + s2);
    }
}

void ControlUnit::disconnectFromServer()
{
    qDebug() << "Control Unit" << name << ": disconnect";
    socket->disconnectFromHost();
}
