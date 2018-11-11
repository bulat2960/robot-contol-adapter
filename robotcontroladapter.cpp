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
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    connect(socket, SIGNAL(disconnected()), this, SLOT(showAll()));
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
        if (data == "p") // Planner sends his name
        {
            if (planner != nullptr && planner->state() == QAbstractSocket::SocketState::UnconnectedState) // Already exist, but disconnected
            {
                planner->deleteLater();
            }
            planner = socket; // Init or reinit socket
            waitSockets.removeOne(socket);
        }
        else if (data == "e") // Planner sends shutdown command (planner already exists at this moment)
        {
            qDebug() << "Planer shutdown";
            for (const auto& client : clients) // Send shutdown command to all clients
            {
                client->write(data);
            }
            clients.clear();
        }
        else if (socket != planner) // Message not from planner
        {
            if (clients.contains(data) == false) // Client isn't in the list and sends us his name
            {
                clients.insert(data, socket);
            }
            else if (clients.contains(data) == true && clients[data]->state() == QAbstractSocket::SocketState::UnconnectedState) // Exists, but disconnected
            {
                clients[data]->deleteLater();
                clients[data] = socket;
            }
            waitSockets.removeOne(socket);
        }
        else // Senseless command
        {
            qDebug() << "Wrong command from the planer";
        }
    }
    else // Other commands
    {
        QList<QByteArray> list = data.split('|'); // Get a list of commands

        for (int i = 0; i < list.size(); i++)
        {
            QByteArray cmd = list[i];
            QList<QByteArray> temp = cmd.split(':');

            if (temp.size() != 2) // Command without symbol ':', skip
            {
                qDebug() << "Command without split symbol :";
                continue;
            }

            if (socket == planner) // Signal from planner
            {
                if (clients.contains(temp[0])) // If client exists
                {
                    clients[temp[0]]->write(temp[1]); // Send message to the unit
                }
                else // Not exist
                {
                    qDebug() << "Client doesn't exists";
                }
            }
            else // Signal from one of the units
            {
                // Send message to the scene
                sceneSocket->write("{" + temp[0] + " : " + temp[1] + "}");
            }
        }
    }

    qDebug() << clients;
}

void RobotControlAdapter::showAll()
{
    qDebug() << clients;
}
