#include "sceneconnector.h"

SceneConnector::SceneConnector(QString ip, quint16 port, int untilReconnectDuration, int reconnectTimes)
{
    this->ip = ip;
    this->port = port;

    this->untilReconnectDuration = untilReconnectDuration;
    this->reconnectTimes = reconnectTimes;

    this->attemptsToReconnect = 0;

    socket = new QTcpSocket(this);
    socket->connectToHost(ip, port);

    reconnectTimer.setDuration(untilReconnectDuration);
    connect(&reconnectTimer, &QTimeLine::finished, this, &SceneConnector::slotSendAgain);
}

void SceneConnector::slotSend(QByteArray msg)
{
    QTime timer;
    timer.restart();

    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        socket->connectToHost(ip, port);

        if (attemptsToReconnect < reconnectTimes)
        {
            attemptsToReconnect++;
            reconnectTimer.start();
            unsentMessages.push_back(msg);
        }
    }
    else if (socket->waitForConnected())
    {
        attemptsToReconnect = 0;
        socket->write(msg);
        socket->waitForBytesWritten();
    }

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void SceneConnector::slotSendAgain()
{
    QByteArray s = unsentMessages.front();
    unsentMessages.pop_front();
    slotSend(s);
}

SceneConnector::~SceneConnector()
{
    QTime timer;
    timer.restart();

    socket->disconnectFromHost();
    socket->deleteLater();

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}
