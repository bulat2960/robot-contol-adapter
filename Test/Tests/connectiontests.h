#ifndef CONNECTIONCHECKTESTS_H
#define CONNECTIONCHECKTESTS_H

#include <QObject>
#include <QtTest>

#include <QProcess>

#include "Objects/scene.h"
#include "Objects/controlunit.h"
#include "Objects/planner.h"

class ConnectionTests : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionTests(QString rcaIp, QString sceneIp, quint16 rcaPort, quint16 scenePort,
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
    void connectToRcaUnitT();
    void connectToRcaUnitF();
    void connectToRcaPlanner();
    void connectRcaToScene();
    void connectRcaToNotRunningScene();
    void disconnectRcaFromScene();
    void disconnectFromRcaUnitT();
    void disconnectFromRcaUnitF();
    void disconnectFromRcaPlanner();
};

#endif // CONNECTIONCHECKTESTS_H
