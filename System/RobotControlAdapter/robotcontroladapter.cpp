#include "robotcontroladapter.h"

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

void RobotControlAdapter::incomingConnection(int socketDescriptor)
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

    if (data == "p")
    {
        if (plannerConnector != nullptr)
        {
            qInfo() << "Planner connector exists, recreate";
            plannerConnector->deleteLater();
            plannerConnector = nullptr;
            plannerConnector = new PlannerConnector(socket);
        }
        else
        {
            qInfo() << "Create new planner connector";
            plannerConnector = new PlannerConnector(socket);
        }
        waitSockets.removeOne(socket);

        // (*) ... and connect to planner connector
        connect(socket, &QTcpSocket::readyRead, plannerConnector, &PlannerConnector::slotRead);
    }
    else
    {
        ControlUnitConnector* unitConnector = new ControlUnitConnector(socket, data);
        if (unitConnectors.contains(data))
        {
            qInfo() << "Unit connector exists, recreate";
            unitConnectors[data]->deleteLater();
            unitConnectors[data] = nullptr;
            unitConnectors[data] = unitConnector;
        }
        else
        {
            qInfo() << "Create new unit connector";
            unitConnectors.insert(data, unitConnector);
        }
        waitSockets.removeOne(socket);

        // (*) ... and connect to unit connector
        connect(socket, &QTcpSocket::readyRead, unitConnector, &ControlUnitConnector::slotRead);
        connect(plannerConnector, &PlannerConnector::signalClose, unitConnector, &ControlUnitConnector::slotSend);
        connect(unitConnector, &ControlUnitConnector::signalUnitClosed, &ControlUnitConnector::deleteLater);
    }

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void RobotControlAdapter::slotParseCmd(QByteArray cmd)
{

}

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
