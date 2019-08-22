#ifndef ROBOTCONTROLADAPTER_H
#define ROBOTCONTROLADAPTER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>
#include <QEventLoop>

#include "Connectors/controlunitconnector.h"
#include "Connectors/plannerconnector.h"
#include "Connectors/sceneconnector.h"

/**
 * It's the main object - a server, who can connect to 3DScene,
 * contain and support connections of other clients and planner,
 * can receive their commands and retranslate them to 3DScene.
**/

class RobotControlAdapter : public QTcpServer // Simple tcp-server
{
    Q_OBJECT
private:
    // Sockets without name
    QList<QTcpSocket*> waitSockets;

    // Clients(units) with name
    QMap<QString, ControlUnitConnector*> unitConnectors;

    // Planner
    PlannerConnector* plannerConnector;

    // 3DScene socket
    SceneConnector* sceneConnector;

public:
    // Basic constructor
    RobotControlAdapter(quint16 rcaPort, QString sceneIp, quint16 scenePort, int untilReconnectDuration, int reconnectTimes);

    RobotControlAdapter(const RobotControlAdapter&) = delete;
    RobotControlAdapter& operator=(const RobotControlAdapter&) = delete;
    RobotControlAdapter(RobotControlAdapter&&) = delete;
    RobotControlAdapter& operator=(RobotControlAdapter&&) = delete;

    ~RobotControlAdapter() override;

private:
    void shutdown();

private slots:
    // Slots for necessary actions
    void slotRead();

public slots:
    void slotShutdown(QByteArray msg);
    void slotFromPlannerToUnit(QByteArray name, QByteArray msg);

protected slots:
    void incomingConnection(qintptr socketDescriptor) override;
signals:
    void signalShutdown();
};

#endif // ROBOTCONTROLADAPTER_H
