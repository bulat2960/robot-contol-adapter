﻿#include "robotcontroladapter.h"

RobotControlAdapter::RobotControlAdapter(quint16 rcaPort, QString sceneIp, quint16 scenePort)
{
    QTime timer;
    timer.restart();

    plannerConnector = nullptr;

    // Init scene socket and try to connect
    qInfo() << "Create scene connector";
    sceneConnector = new SceneConnector(sceneIp, scenePort);

    // Start listening
    if (this->listen(QHostAddress::Any, rcaPort))
    {
        qDebug() << "Listening RCA";
    }
    else
    {
        qDebug() << "Not listening RCA";
    }

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void RobotControlAdapter::incomingConnection(qintptr socketDescriptor)
{
    QTime timer;
    timer.restart();

    // Create socket and set descriptor
    qInfo() << "Create client socket";
    QTcpSocket* socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    // Add to waitlist
    waitSockets.append(socket);

    // Connect signals and slots
    connect(socket, &QTcpSocket::readyRead, this, &RobotControlAdapter::slotRead);

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void RobotControlAdapter::slotRead()
{
    QTime timer;
    timer.restart();

    // Find the sender and cast into the socket
    QObject* object = QObject::sender();
    QTcpSocket* socket = static_cast<QTcpSocket*>(object);

    // Disconnect from this object... (*)
    disconnect(socket, &QTcpSocket::readyRead, this, &RobotControlAdapter::slotRead);

    // Read data
    QByteArray data = socket->readAll();

    if (data == "p") // Planner tries to connect
    {
        // If planner already exists, delete him...
        if (plannerConnector != nullptr)
        {
            qInfo() << "Planner connector exists, delete";
            plannerConnector->deleteLater();
        }

        // ... and reinit him
        qInfo() << "Create new planner connector";
        plannerConnector = new PlannerConnector(socket);

        // (*) ... and connect to planner connector
        connect(socket, &QTcpSocket::readyRead, plannerConnector, &PlannerConnector::slotRead);
        // On sending shutdown signal, call shutdown preparing
        connect(plannerConnector, &PlannerConnector::signalShutdown, this, &RobotControlAdapter::slotPrepareShutdown);
    }
    else // Unit tries to connect
    {
        // Create new unit connector
        ControlUnitConnector* unitConnector = new ControlUnitConnector(socket, data);

        // If it exists earlier, delete him...
        if (unitConnectors.contains(data))
        {
            qInfo() << "Unit connector exists, delete";
            unitConnectors[data]->deleteLater();
            unitConnectors.remove(data);
        }

        // ... and reinit him
        qInfo() << "Create new unit connector";
        unitConnectors.insert(data, unitConnector);

        // (*) ... and connect to unit connector
        connect(socket, &QTcpSocket::readyRead, unitConnector, &ControlUnitConnector::slotRead);
        // On getting disconnect request from planner, send it to unit connector
        connect(this, &RobotControlAdapter::signalDisconnectRequest, unitConnector, &ControlUnitConnector::slotSend);
        // On receiving disconnect signal from unit connector, destroy him
        connect(unitConnector, &ControlUnitConnector::signalDisconnected, this, &RobotControlAdapter::slotClearUnitConnector);
    }

    // Remove wait socket, it is useless here now
    waitSockets.removeOne(socket);

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void RobotControlAdapter::slotPrepareShutdown(QByteArray msg)
{
    QTime timer;
    timer.restart();

    // Delete planner
    plannerConnector->deleteLater();

    // If there was no connected units, shutdown the system
    // And send disconnect request to connected units otherwise
    if (unitConnectors.isEmpty())
    {
        emit signalShutdown();
    }
    else
    {
        emit signalDisconnectRequest(msg);
    }

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void RobotControlAdapter::slotClearUnitConnector()
{
    QTime timer;
    timer.restart();

    // Find UnitConnector-sender
    QObject* object = QObject::sender();
    ControlUnitConnector* unitConnector = static_cast<ControlUnitConnector*>(object);

    // Delete him
    QByteArray name = unitConnector->getName();
    unitConnectors[name]->deleteLater();
    unitConnectors.remove(name);

    // If there are no UnitConnectors and planner destroyed too, shutdown the system
    if (unitConnectors.isEmpty() && plannerConnector == nullptr)
    {
        emit signalShutdown();
    }

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void RobotControlAdapter::slotParseCmd(QByteArray cmd)
{

}

RobotControlAdapter::~RobotControlAdapter()
{
    QTime timer;
    timer.restart();

    // Units and planner had to be deleted earlier, so one need to delete scene connector
    sceneConnector->deleteLater();

    // Delete wait sockets, if some of them are still here
    for (const auto& socket : waitSockets)
    {
        socket->disconnectFromHost();
        socket->deleteLater();
    }

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

// Following code need to be rewrited!!!!! Don't delete it

/* RobotControlAdapter::processPlannerCmd(QByteArray plannerCmd)
{
    qInfo() << "Process planner command -" << plannerCmd;

    QTime timer;
    timer.restart();

    if (plannerCmd == "e") // Planner sends shutdown command (planner already exists at this moment)
    {
        for (const auto& client : clients) // Send shutdown command to all clients
        {
            client->write(plannerCmd);
        }
        clients.clear();

        emit closeAll();
    }
    else // Planner sends other command
    {
        QList<QByteArray> pairNameAndCmd = plannerCmd.split(':');

        if (pairNameAndCmd.size() != 2) // Wrong command
        {
            qWarning() << "Wrong command";
            return;
        }

        QByteArray name = pairNameAndCmd[0];
        QByteArray cmd  = pairNameAndCmd[1];

        if (clients.contains(name)) // If client exists
        {
            clients[name]->write(cmd); // Send message to the unit
        }
        else // Not exist
        {
            qWarning() << "Client doesn't exists";
        }
    }

    qDebug() << "Elapsed -" << timer.elapsed() << "ms";
}

void RobotControlAdapter::processUnitCmd(QByteArray unitCmd)
{
    qInfo() << "Process unit command -" << unitCmd;

    QTime timer;
    timer.restart();

    QList<QByteArray> pairNameAndCmd = unitCmd.split(':');

    QByteArray name = pairNameAndCmd[0];
    QByteArray cmd  = pairNameAndCmd[1];
    sceneSocket->write("{" + name + " : " + cmd + "}");

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void RobotControlAdapter::processSingleCharCmd(QTcpSocket* socket, QByteArray name)
{
    qInfo() << "Process single char command -" << name;

    QTime timer;
    timer.restart();

    if (name == "p") // Planner sends its name
    {
        if (planner != nullptr && isUnconnectedState(planner)) // Already exist, but disconnected
        {
            planner->deleteLater();
        }
        planner = socket; // Init or reinit socket
        waitSockets.removeOne(socket);
    }
    else if (socket != planner) // It's from one of units
    {
        if (clients.contains(name)) // Client isn't in the list and sends us his name
        {
            clients.insert(name, socket);
        }
        else if (clients.contains(name) && isUnconnectedState(clients[name])) // Exists, but disconnected
        {
            clients[name]->deleteLater();
            clients[name] = socket;
        }
        waitSockets.removeOne(socket);
    }
    else
    {
        qWarning() << "Unknown command from planner";
    }

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}*/
