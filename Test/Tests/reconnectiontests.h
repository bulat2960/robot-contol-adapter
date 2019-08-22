#ifndef RECONNECTIONCHECKTESTS_H
#define RECONNECTIONCHECKTESTS_H

#include <QObject>
#include <QtTest>

#include <QProcess>

#include "Objects/scene.h"
#include "Objects/controlunit.h"
#include "Objects/planner.h"

class ReconnectionTests : public QObject
{
    Q_OBJECT
public:
    explicit ReconnectionTests(QString rcaIp, QString sceneIp, quint16 rcaPort, quint16 scenePort,
                               QString pathToRcaExec, int waitTime, QObject *parent = nullptr);
private:
    QString rcaIp;
    QString sceneIp;
    quint16 rcaPort;
    quint16 scenePort;

    QString pathToRcaExec;
    QProcess rcaProcess;

    int waitTime;

    Scene* scene;
signals:

public slots:

private slots:
    void init();
    void cleanup();
    void reconnectUnitToRca();
    void reconnectRcaToScene();
    void reconnectPlannerToRca();
};

#endif // RECONNECTIONCHECKTESTS_H
