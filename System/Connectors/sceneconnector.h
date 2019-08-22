#ifndef SCENECONNECTOR_H
#define SCENECONNECTOR_H

#include <QTcpSocket>
#include <QTime>
#include <QTimeLine>

class SceneConnector : public QObject
{
    Q_OBJECT
private:
    QTcpSocket* socket;

    QString ip;
    quint16 port;

    QTimeLine reconnectTimer;
    int reconnectTimes;
    int untilReconnectDuration;

    int attemptsToReconnect;

    QVector<QByteArray> unsentMessages;
public:
    SceneConnector(QString ip, quint16 port, int untilReconnectDuration, int reconnectTimes);

    SceneConnector(const SceneConnector&) = delete;
    SceneConnector& operator=(const SceneConnector&) = delete;
    SceneConnector(SceneConnector&&) = delete;
    SceneConnector& operator=(SceneConnector&&) = delete;

    ~SceneConnector();
public:
    bool isConnectedWithScene();

public slots:
    void slotSend(QByteArray msg);
    void slotSendAgain();
};

#endif // SCENECONNECTOR_H
