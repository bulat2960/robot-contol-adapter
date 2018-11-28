#include "controlunit.h"

ControlUnit::ControlUnit(QString unitName)
{
    timer.restart();

    qInfo() << "Create unit with name" << unitName;

    // Create socket, set name
    socket = new QTcpSocket(this);
    name.append(unitName);

    // Connect signals and slots
    connect(socket, &QTcpSocket::connected, this, &ControlUnit::sendName);
    connect(socket, &QTcpSocket::readyRead, this, &ControlUnit::readyRead);

    // Connect button
    connectButton = new QPushButton("ControlUnit " + name + "- Connect to Server", this);
    connectButton->setGeometry(0, 0, 400, 200);
    connect(connectButton, &QPushButton::clicked, this, &ControlUnit::connectToServer);

    // Disconnect button
    disconnectButton = new QPushButton("ControlUnit " + name + ": Disconnect from Server", this);
    disconnectButton->setGeometry(0, 200, 400, 200);
    connect(disconnectButton, &QPushButton::clicked, this, &ControlUnit::disconnectFromServer);

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void ControlUnit::connectToServer()
{
    timer.restart();

    // Try to connect to server
    socket->connectToHost("localhost", 5555);

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void ControlUnit::sendName()
{
    timer.restart();

    // Check the connection and send our name
    qDebug() << "Control Unit" << name << "- connection established";
    socket->write(name);

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void ControlUnit::readyRead()
{
    timer.restart();

    // Read what we received
    QByteArray data = socket->readAll();

    qDebug() << "ControlUnit" << name << "received message -" << data;

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

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void ControlUnit::disconnectFromServer()
{
    timer.restart();

    qDebug() << "Control Unit" << name << "- disconnect";
    socket->disconnectFromHost();

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}
