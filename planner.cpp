#include "planner.h"

Planner::Planner(QString unitName)
{
    // Create socket, set name
    socket = new QTcpSocket(this);
    name.append(unitName);

    // Connect signals and slots
    connect(socket, SIGNAL(connected()), this, SLOT(sendName()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    //connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectFromServer()));

    // Connect button
    connectButton = new QPushButton("Planner: Connect to Server", this);
    connectButton->setGeometry(0, 0, 400, 100);
    connect(connectButton, SIGNAL(clicked()), this, SLOT(connectToServer()));

    // Disconnect button
    disconnectButton = new QPushButton("Planner: Disconnect from Server", this);
    disconnectButton->setGeometry(0, 100, 400, 100);
    connect(disconnectButton, SIGNAL(clicked()), this, SLOT(disconnectFromServer()));

    // Text edit in which to write a command
    textEdit = new QTextEdit(this);
    textEdit->setGeometry(0, 220, 400, 30);

    // Send message from text edit
    sendMsgButton = new QPushButton("Send Message", this);
    sendMsgButton->setGeometry(0, 250, 400, 30);
    sendMsgButton->setEnabled(false);
    connect(sendMsgButton, SIGNAL(clicked()), this, SLOT(sendMsg()));
}

void Planner::connectToServer()
{
    // Try to connect to server
    socket->connectToHost("localhost", 5555);
    sendMsgButton->setEnabled(true);
}

void Planner::sendName()
{
    // Check the connection and send our name
    qDebug() << "Planner: connection established";
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
    }
}

void Planner::readyRead()
{
    // Читаем данные
    QByteArray data = socket->readAll();
}

void Planner::disconnectFromServer()
{
    qDebug() << "Planner: disconnect";
    socket->disconnectFromHost();
    sendMsgButton->setEnabled(false);
}
