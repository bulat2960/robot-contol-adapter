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

    if (msg == "e")
    {
        qInfo() << "Planner sends shutdown cmd";
        emit signalShutdown(msg);
    }
    else
    {
        QList<QPair<QByteArray, QByteArray>> pairList = parseMsg(msg);

        for (const auto& cmd : pairList)
        {
            qInfo() << "Planner wants to send" << cmd.second << "to unit" << cmd.first;
            emit signalFromPlannerToUnit(cmd.first, cmd.second);
        }
    }

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

QList<QPair<QByteArray, QByteArray>> PlannerConnector::parseMsg(QByteArray msg)
{
    QList<QByteArray> separatedCommands = msg.split('|');

    QList<QPair<QByteArray, QByteArray>> pairList;

    for (const auto& command : separatedCommands)
    {
        QList<QByteArray> unitNameAndMsg = command.split(':');

        if (unitNameAndMsg.size() == 1)
        {
            qInfo() << "Senseless cmd" << unitNameAndMsg[0];
        }
        else
        {
            pairList.append(qMakePair(unitNameAndMsg[0], unitNameAndMsg[1]));
        }
    }

    return pairList;
}

PlannerConnector::~PlannerConnector()
{
    QTime timer;
    timer.restart();

    socket->disconnectFromHost();
    socket->deleteLater();

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}
