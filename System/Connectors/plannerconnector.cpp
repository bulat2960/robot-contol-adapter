#include "plannerconnector.h"

PlannerConnector::PlannerConnector(QTcpSocket* socket)
{
    this->socket = socket;
}

PlannerConnector::~PlannerConnector()
{
    socket->disconnectFromHost();
    socket->deleteLater();
}
