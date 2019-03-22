#include "controlunitconnector.h"

ControlUnitConnector::ControlUnitConnector(QTcpSocket* socket, QByteArray name)
{
    this->socket = socket;
    this->name = name;

    connect(socket, &QTcpSocket::readyRead, this, &ControlUnitConnector::slotRead);
}

void ControlUnitConnector::slotSend(QByteArray msg)
{
    socket->write(msg);
    socket->waitForBytesWritten();

    if (msg == "e")
    {
        emit signalUnitClosed();
    }
}

void ControlUnitConnector::slotRead()
{
    QByteArray msg = socket->readAll();
    emit signalMsgReceived(msg);
}

ControlUnitConnector::~ControlUnitConnector()
{
    socket->disconnectFromHost();
    socket->deleteLater();
}
