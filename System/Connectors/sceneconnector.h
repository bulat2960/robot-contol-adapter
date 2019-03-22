#ifndef SCENECONNECTOR_H
#define SCENECONNECTOR_H

#include <QTcpSocket>

class SceneConnector : public QObject
{
    Q_OBJECT
private:
    QTcpSocket* socket;
public:
    SceneConnector(QString ip, quint16 port);
};

#endif // SCENECONNECTOR_H
