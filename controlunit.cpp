#include "controlunit.h"

ControlUnit::ControlUnit(QString unitName)
{
    // Создаем сокет, устанавливаем имя ControlUnit'a
    socket = new QTcpSocket(this);
    name.append(unitName);

    // Необходимые коннекты
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(deleteSocket()));

    // Кнопочка подключения к серверу
    b = new QPushButton("ControlUnit " + name + ": Connect to Server", this);
    b->setGeometry(0, 0, 400, 400);
    connect(b, SIGNAL(clicked()), this, SLOT(connectToServer()));
}

void ControlUnit::connectToServer()
{
    // Стучимся на сервер
    socket->connectToHost("localhost", 5555);

    if (socket->isValid())
    {
        qDebug() << "Control Unit" << name << ": connection established";
    }

    socket->write(name);
}

void ControlUnit::readyRead()
{
    // Читаем данные
    QByteArray data = socket->readAll();

    qDebug() << "ControlUnit" << name << "received message:" << data;

    QByteArray s1 = "\"id\"";
    QByteArray s2 = "\"SomeData\"";
    socket->write(s1 + ":" + s2);
}

void ControlUnit::deleteSocket()
{
    // Удаляем сокет при отключении
    socket->deleteLater();
}
