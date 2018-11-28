#include "planner.h"

Planner::Planner(QString unitName)
{
    timer.restart();

    qInfo() << "Create planner with name" << unitName;

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

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void Planner::connectToServer()
{
    timer.restart();

    // Try to connect to server
    socket->connectToHost("localhost", 5555);
    sendMsgButton->setEnabled(true);

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void Planner::sendName()
{
    timer.restart();

    // Check the connection and send our name
    qDebug() << "Planner - connection established";
    socket->write("p");

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void Planner::sendMsg()
{
    timer.restart();

    QByteArray arr;
    arr.append(textEdit->toPlainText());
    socket->write(arr);

    if (arr == "e")
    {
        disconnectFromServer();
    }

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void Planner::readyRead()
{
    // ??????????????????

    // Read data
    QByteArray data = socket->readAll();
}

void Planner::disconnectFromServer()
{
    timer.restart();

    qDebug() << "Planner - disconnect";
    socket->disconnectFromHost();
    sendMsgButton->setEnabled(false);

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}
