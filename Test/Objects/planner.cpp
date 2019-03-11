#include "planner.h"

Planner::Planner(QString unitName, QString rcaIp, quint16 rcaPort)
{
    // Set RCA ip and port
    this->rcaIp = rcaIp;
    this->rcaPort = rcaPort;

    // Create socket, set name
    socket = new QTcpSocket(this);
    name.append(unitName);

    // Connect signals and slots
    connect(socket, &QTcpSocket::connected, this, &Planner::sendName);
    connect(socket, &QTcpSocket::readyRead, this, &Planner::readyRead);

    // Connect button
    connectButton = new QPushButton("Planner: Connect to Server", this);
    connectButton->setGeometry(0, 0, 400, 100);
    connect(connectButton, &QPushButton::clicked, this, &Planner::connectToServer);

    // Disconnect button
    disconnectButton = new QPushButton("Planner: Disconnect from Server", this);
    disconnectButton->setGeometry(0, 100, 400, 100);
    connect(disconnectButton, &QPushButton::clicked, this, &Planner::disconnectFromServer);

    // Text edit in which to write a command
    textEdit = new QTextEdit(this);
    textEdit->setGeometry(0, 220, 400, 30);

    // Send message from text edit
    sendMsgButton = new QPushButton("Send Message", this);
    sendMsgButton->setGeometry(0, 250, 400, 30);
    sendMsgButton->setEnabled(false);
    connect(sendMsgButton, &QPushButton::clicked, this, &Planner::sendMsg);
}

void Planner::connectToServer()
{
    // Try to connect to server
    socket->connectToHost(rcaIp, rcaPort);
    sendMsgButton->setEnabled(true);
}

void Planner::sendName()
{
    // Check the connection and send our name
    qDebug() << "Planner - connection established";
    socket->write("p");
}

void Planner::sendMsg()
{
    QByteArray arr;
    arr.append(textEdit->toPlainText());
    socket->write(arr);

    if (arr == "e")
    {
        disconnectFromServer();
        socket->waitForBytesWritten(1000);
        emit closeAll();
    }
}

void Planner::readyRead()
{
    // Read data
    QByteArray data = socket->readAll();
}

void Planner::disconnectFromServer()
{
    qDebug() << "Planner - disconnect";
    socket->disconnectFromHost();
    sendMsgButton->setEnabled(false);
}
