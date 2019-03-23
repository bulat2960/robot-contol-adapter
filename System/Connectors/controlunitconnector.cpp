#include "controlunitconnector.h"

ControlUnitConnector::ControlUnitConnector(QTcpSocket* socket, QByteArray name)
{
    this->socket = socket;
    this->name = name;

    connect(socket, &QTcpSocket::readyRead, this, &ControlUnitConnector::slotRead);
}

QByteArray ControlUnitConnector::getName() const
{
    return name;
}

void ControlUnitConnector::slotSend(QByteArray msg)
{
    QTime timer;
    timer.restart();

    socket->write(msg);

    if (msg == "e")
    {
        if (socket->waitForBytesWritten())
        {
            qInfo() << "Unit" << name << "has been disconnected";
            emit signalDisconnected();
        }
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

    qInfo() << "Unit" << name << "received message" << msg;

    emit signalMsgReceived(msg);

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
