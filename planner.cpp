#include "planner.h"

Planner::Planner(QString unitName)
{
    socket = new QTcpSocket(this);

    name.append(unitName);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(deleteSocket()));

    b = new QPushButton("Planner: Connect to Server", this);
    b->setGeometry(0, 0, 400, 100);
    connect(b, SIGNAL(clicked()), this, SLOT(connectToServer()));

    textEdit = new QTextEdit(this);
    textEdit->setGeometry(0, 120, 400, 30);

    sendMsgButton = new QPushButton("SendMessage", this);
    sendMsgButton->setGeometry(0, 150, 400, 30);
    connect(sendMsgButton, SIGNAL(clicked()), this, SLOT(sendMsg()));
}

void Planner::connectToServer()
{
    socket->connectToHost("localhost", 5555);

    if (socket->isValid())
    {
        qDebug() << "Planner: connection established";
    }

    socket->write("p");
}

void Planner::sendMsg()
{
    QByteArray arr;
    arr.append(textEdit->toPlainText());
    socket->write(arr);
}

void Planner::readyRead()
{
    // Читаем данные
    QByteArray data = socket->readAll();

    if (data == "Name request")// Если спрашивают имя, говорим его
    {
        socket->write("Name: " + name);
    }
}

void Planner::deleteSocket()
{
    socket->deleteLater();
}
