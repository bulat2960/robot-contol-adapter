#include "controlunitconnector.h"

ControlUnitConnector::ControlUnitConnector(QTcpSocket* socket, QByteArray name)
{
    this->socket = socket;
    this->name = name;

    connect(socket, &QTcpSocket::readyRead, this, &ControlUnitConnector::slotReceive);
}

void ControlUnitConnector::slotSend(QByteArray msg)
{
    socket->write(msg);
}

void ControlUnitConnector::slotReceive()
{
    QByteArray msg = socket->readAll();
    emit signalMsgReceived(msg);
}

ControlUnitConnector::~ControlUnitConnector()
{
    socket->disconnectFromHost();
    socket->deleteLater();
}
