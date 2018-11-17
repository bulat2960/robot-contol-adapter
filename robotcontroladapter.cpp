#include "robotcontroladapter.h"

RobotControlAdapter::RobotControlAdapter()
{
    planner = nullptr;

    // Init scene socket and try to connect
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
}

void RobotControlAdapter::incomingConnection(int socketDescriptor)
{
    // Create socket and set descriptor
    QTcpSocket* socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    // Add to waitlist
    waitSockets.append(socket);

    // Connect signals and slots
    connect(socket, &QTcpSocket::readyRead, this, &RobotControlAdapter::readyRead);
}

void RobotControlAdapter::readyRead()
{
    // Find the sender and cast into the socket
    QObject* object = QObject::sender();
    QTcpSocket* socket = static_cast<QTcpSocket*>(object);

    // Read data
    QByteArray data = socket->readAll();

    if (data.size() == 1) // Object names, or shutdown command
    {
        processObjectWithoutName(socket, data);
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
    if (cmd == "e") // Planner sends shutdown command (planner already exists at this moment)
    {
        qDebug() << "Planer shutdown";
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
            qDebug() << "Wrong command";
            return;
        }

        if (clients.contains(temp[0])) // If client exists
        {
            qDebug() << "SEND" << temp[0] << temp[1];
            clients[temp[0]]->write(temp[1]); // Send message to the unit
        }
        else // Not exist
        {
            qDebug() << "Client doesn't exists";
        }
    }
}

void RobotControlAdapter::processUnitCmd(QByteArray cmd)
{
    QList<QByteArray> temp = cmd.split(':');
    sceneSocket->write("{" + temp[0] + " : " + temp[1] + "}");
}

void RobotControlAdapter::processObjectWithoutName(QTcpSocket* socket, QByteArray cmd)
{
    if (cmd == "p") // Planner sends its name
    {
        if (planner != nullptr && isUnconnectedState(planner)) // Already exist, but disconnected
        {
            planner->deleteLater();
        }
        planner = socket; // Init or reinit socket
        waitSockets.removeOne(socket);
    }
    else // It's from one of units
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
}
