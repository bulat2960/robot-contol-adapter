#include "robotcontroladapter.h"

RobotControlAdapter::RobotControlAdapter()
{
    timer.restart();

    planner = nullptr;

    // Init scene socket and try to connect
    qInfo() << "Create scene socket";
    sceneSocket = new QTcpSocket(this);
    sceneSocket->connectToHost("localhost", 1111);

    // Start listening
    if (this->listen(QHostAddress::LocalHost, 5555))
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
    timer.restart();

    // Create socket and set descriptor
    qInfo() << "Create client socket";
    QTcpSocket* socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    // Add to waitlist
    waitSockets.append(socket);

    // Connect signals and slots
    connect(socket, &QTcpSocket::readyRead, this, &RobotControlAdapter::readyRead);

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void RobotControlAdapter::readyRead()
{
    timer.restart();

    // Find the sender and cast into the socket
    QObject* object = QObject::sender();
    QTcpSocket* socket = static_cast<QTcpSocket*>(object);

    // Read data
    QByteArray data = socket->readAll();

    if (data.size() == 1 && data != "e") // Object names, or shutdown command, or planner cmd with only 1 symbol, except for command 'e'
    {
        processSingleCharCmd(socket, data);
    }
    else // Other commands
    {
        QList<QByteArray> list = data.split('|'); // Get a list of commands

        for (int i = 0; i < list.size(); i++)
        {
            QByteArray cmd = list[i];

            if (socket == planner)
            {
                processPlannerCmd(cmd);
            }
            else
            {
                processUnitCmd(cmd);
            }
        }
    }

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

bool RobotControlAdapter::isConnectedState(QTcpSocket* socket) const
{
    return socket->state() == QTcpSocket::SocketState::ConnectedState;
}

bool RobotControlAdapter::isUnconnectedState(QTcpSocket* socket) const
{
    return socket->state() == QTcpSocket::SocketState::UnconnectedState;
}

void RobotControlAdapter::processPlannerCmd(QByteArray cmd)
{
    qInfo() << "Process planner command -" << cmd;

    timer.restart();

    if (cmd == "e") // Planner sends shutdown command (planner already exists at this moment)
    {
        for (const auto& client : clients) // Send shutdown command to all clients
        {
            client->write(cmd);
        }
        clients.clear();
    }
    else // Planner sends other command
    {
        QList<QByteArray> temp = cmd.split(':');

        if (temp.size() != 2) // Wrong command
        {
            qWarning() << "Wrong command";
            return;
        }

        if (clients.contains(temp[0])) // If client exists
        {
            clients[temp[0]]->write(temp[1]); // Send message to the unit
        }
        else // Not exist
        {
            qWarning() << "Client doesn't exists";
        }
    }

    qDebug() << "Elapsed -" << timer.elapsed() << "ms";
}

void RobotControlAdapter::processUnitCmd(QByteArray cmd)
{
    qInfo() << "Process unit command -" << cmd;

    timer.restart();

    QList<QByteArray> temp = cmd.split(':');
    sceneSocket->write("{" + temp[0] + " : " + temp[1] + "}");

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void RobotControlAdapter::processSingleCharCmd(QTcpSocket* socket, QByteArray cmd)
{
    timer.restart();

    if (cmd == "p") // Planner sends its name
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
        if (clients.contains(cmd) == false) // Client isn't in the list and sends us his name
        {
            clients.insert(cmd, socket);
        }
        else if (clients.contains(cmd) == true && isUnconnectedState(clients[cmd])) // Exists, but disconnected
        {
            clients[cmd]->deleteLater();
            clients[cmd] = socket;
        }
        waitSockets.removeOne(socket);
    }
    else
    {
        qWarning() << "Unknown command from planner";
    }

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}
