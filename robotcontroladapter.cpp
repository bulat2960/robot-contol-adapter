#include "robotcontroladapter.h"

RobotControlAdapter::RobotControlAdapter()
{
    sceneSocket = new QTcpSocket(this);
    sceneSocket->connectToHost("localhost", 1111);

    if (this->listen(QHostAddress("localhost"), 5555))
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

    // Посылаем сокету запрос его имени
    socket->write("Name request");
}

void RobotControlAdapter::readyRead()
{
    // Находим объект-отправитель сигнала на чтение, кастуем в сокет
    QObject* object = QObject::sender();
    QTcpSocket* socket = static_cast<QTcpSocket*>(object);

    // Читаем данные
    QByteArray data = socket->readAll();
    QList<QByteArray> list = data.split(':');

    if (list.size() == 2) // Если был один разделитель-двоеточие
    {
        list[1].remove(0, 1); // Удаляем пробел
        if (list[0] == "Name") // Если ControlUnit отправил нам имя в формате "Name: xxx"
        {
            // Добавляем сокет в карту клиентов и удаляем из списка ожидания
            clients.insert(list[1], socket);
            waitSockets.removeOne(socket);
        }
        else if (list[0] == "\"id\"") // Если ControlUnit вернул ответ
        {
            // Преобразуем в json, отправляем сцене
            sceneSocket->write("{" + list[0] + " : " + list[1] + "}");
        }
        else // Если planner отправил нам сообщение в формате "CUnitName: someText"
        {
            // Клиенту с именем CUnitName отправляем someText
            clients[list[0]]->write(list[1]);
        }
    }
    else
    {
        if (list[0] == "e") // Если planner отправил нам сообщение "е"
        {
            qDebug() << "Planer shutdown";
        }
        else // Бессмысленная команда, скипаем
        {
            qDebug() << "Senseless command";
        }
    }
}

void RobotControlAdapter::deleteSocket()
{
    // Находим объект-отправитель сигнала на чтение, кастуем в сокет
    QObject* object = QObject::sender();
    QTcpSocket* socket = static_cast<QTcpSocket*>(object);

    // Описать отключение сокета
}
