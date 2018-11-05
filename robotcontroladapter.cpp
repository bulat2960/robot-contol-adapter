#include "robotcontroladapter.h"

RobotControlAdapter::RobotControlAdapter()
{
    sceneSocket = new QTcpSocket(this);
    sceneSocket->connectToHost("localhost", 1111);

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
    // Создаем новый сокет - канал связи между одним из ControlUnit
    QTcpSocket* socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    // Добавляем сокет в список ожидания
    waitSockets.append(socket);

    // Необходимые соединения слотов и сигналов
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(deleteSocket()));

    qDebug() << waitSockets.size();
}

void RobotControlAdapter::readyRead()
{
    // Находим объект-отправитель сигнала на чтение, кастуем в сокет
    QObject* object = QObject::sender();
    QTcpSocket* socket = static_cast<QTcpSocket*>(object);

    // Читаем данные
    QByteArray data = socket->readAll();
    QList<QByteArray> list = data.split(':');

    if (list.size() == 1) // Команды имен объектов, либо команда 'e'
    {
        if (clients.key(socket) == "p" && list[0] == "e") // Если от планера пришла команда 'e'
        {
            qDebug() << "Planer shutdown";
        }
        else if (clients.key(socket) != "p") // Если сообщение - от одного из юнитов
        {
            if (clients.find(list[0]) == clients.end()) // Если не нашелся клиент с присланным именем
            {
                qDebug() << "Добавляем клиента";
                // Добавляем клиента, удаляем сокет из списка ожидания
                clients.insert(list[0], socket);
                waitSockets.removeOne(socket);
            }
            else // Бессмысленная команда
            {
                qDebug() << "Senseless command";
            }
        }
        else // Бессмысленная команда
        {
            qDebug() << "Senseless command";
        }
    }
    else if (list.size() == 2)
    {
        list[1].remove(0, 1); // Удаляем пробел

        if (clients.key(socket) == "p") // Если сокет-отправитель является планером
        {
            // Если нашелся клиент, к которому стучится планер && планер не стучится сам к себе
            if (clients.find(list[0]) != clients.end() && list[0] != "p")
            {
                // Пишем юниту
                clients[list[0]]->write(list[1]);
            }
            else // Бессмысленная команда
            {
                qDebug() << "Senseless command";
            }
        }
        else // Иначе это один из юнитов
        {
            // Отправляем json-пакет сцене
            sceneSocket->write("{" + list[0] + " : " + list[1] + "}");
        }
    }
    else // Бессмысленная команда
    {
        qDebug() << "Senseless command";
    }
}

void RobotControlAdapter::deleteSocket()
{
    // Находим объект-отправитель сигнала на чтение, кастуем в сокет
    QObject* object = QObject::sender();
    QTcpSocket* socket = static_cast<QTcpSocket*>(object);

    // Описать отключение сокета
}
