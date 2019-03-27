#include "controlunitconnector.h"

ControlUnitConnector::ControlUnitConnector(QTcpSocket* socket, QByteArray name)
{
    this->socket = socket;
    this->name = name;
}

QByteArray ControlUnitConnector::getName() const
{
    return name;
}

void ControlUnitConnector::send(QByteArray msg)
{
    QTime timer;
    timer.restart();

    socket->write(msg);
    socket->waitForBytesWritten();

    if (msg == "e")
    {
        qInfo() << "Unit" << name << "has been disconnected";
    }
    else
    {
        qInfo() << "Send" << msg << "to unit" << name;
    }

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void ControlUnitConnector::slotRead()
{
    QTime timer;
    timer.restart();

    QByteArray msg = socket->readAll();

    qInfo() << "Receive message" << msg << "from unit" << name;

    //emit signalMsgReceived(msg);

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

ControlUnitConnector::~ControlUnitConnector()
{
    QTime timer;
    timer.restart();

    socket->disconnectFromHost();
    socket->deleteLater();

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}
