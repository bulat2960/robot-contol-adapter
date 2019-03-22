#include "plannerconnector.h"

PlannerConnector::PlannerConnector(QTcpSocket* socket)
{
    this->socket = socket;
}

void PlannerConnector::slotRead()
{
    QByteArray msg = socket->readAll();

    if (msg == "e")
    {
        emit signalClose(msg);
    }
    else
    {
        emit signalMsgReceived(msg);
    }
}

PlannerConnector::~PlannerConnector()
{
    socket->disconnectFromHost();
    socket->deleteLater();
}
