#include "plannerconnector.h"

PlannerConnector::PlannerConnector(QTcpSocket* socket)
{
    this->socket = socket;
}

void PlannerConnector::slotRead()
{
    QTime timer;
    timer.restart();

    QByteArray msg = socket->readAll();

    qInfo() << "Planner sends cmd -" << msg;

    if (msg == "e")
    {
        emit signalShutdown(msg);
    }
    else
    {
        emit signalMsgReceived(msg);
    }

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

PlannerConnector::~PlannerConnector()
{
    QTime timer;
    timer.restart();

    socket->disconnectFromHost();
    socket->deleteLater();

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}
