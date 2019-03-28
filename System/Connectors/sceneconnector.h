#ifndef SCENECONNECTOR_H
#define SCENECONNECTOR_H

#include <QTcpSocket>
#include <QTime>

class SceneConnector : public QObject
{
    Q_OBJECT
private:
    QTcpSocket* socket;
public:
    SceneConnector(QString ip, quint16 port);

    SceneConnector(const SceneConnector&) = delete;
    SceneConnector& operator=(const SceneConnector&) = delete;
    SceneConnector(SceneConnector&&) = delete;
    SceneConnector& operator=(SceneConnector&&) = delete;

    ~SceneConnector();

public:
    void send(QByteArray msg);
};

#endif // SCENECONNECTOR_H
