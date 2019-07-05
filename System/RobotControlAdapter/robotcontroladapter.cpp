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
        connect(plannerConnector, &PlannerConnector::signalShutdown, this, &RobotControlAdapter::slotShutdown);
        // On sending command to control unit from planner, exclude shutdown command
        connect(plannerConnector, &PlannerConnector::signalFromPlannerToUnit, this, &RobotControlAdapter::slotFromPlannerToUnit);
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
        // On sending msg from unit to scene
        connect(unitConnector, &ControlUnitConnector::signalFromUnitToScene, sceneConnector, &SceneConnector::slotSend);
    }

    // Remove wait socket, it is useless here now
    waitSockets.removeOne(socket);

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void RobotControlAdapter::slotShutdown(QByteArray msg)
{
    QTime timer;
    timer.restart();

    for (auto& unitConnector : unitConnectors)
    {
        unitConnector->send(msg);
    }

    emit signalShutdown();

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void RobotControlAdapter::slotFromPlannerToUnit(QByteArray name, QByteArray msg)
{
    if (!unitConnectors.contains(name))
    {
        qInfo() << "Unit doesn't exist";
        return;
    }

    qInfo() << "Retranslate msg" << msg << "to unit" << name;
    unitConnectors[name]->send(msg);
}

RobotControlAdapter::~RobotControlAdapter()
{
    QTime timer;
    timer.restart();

    // Delete all connectors
    sceneConnector->deleteLater();
    if (plannerConnector)
    {
        plannerConnector->deleteLater();
    }

    for (const auto& unitConnector : unitConnectors)
    {
        unitConnector->deleteLater();
    }
    unitConnectors.clear();

    // Delete wait sockets, if some of them are still here
    for (const auto& socket : waitSockets)
    {
        socket->disconnectFromHost();
        socket->deleteLater();
    }

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}
