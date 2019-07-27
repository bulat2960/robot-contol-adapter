#ifndef TRANSFERCHECKTESTS_H
#define TRANSFERCHECKTESTS_H

#include <QObject>
#include <QtTest>

#include <QProcess>

#include "Objects/scene.h"
#include "Objects/controlunit.h"
#include "Objects/planner.h"

class TransferTests : public QObject
{
    Q_OBJECT
public:
    explicit TransferTests(QString rcaIp, QString sceneIp, quint16 rcaPort, quint16 scenePort,
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
    void sendFromPlannerToUnitWithoutResponse();
    void sendFromUnitToScene();
    void sendFromPlannerToUnitWithResponse();
    void sendFromPlannerToUnconnectedUnit();
    void sendFromPlannerToMultipleUnits();
    void sendFromPlannerToUnitWithoutDelay();
    void sendFromMultipleUnitsToScene();
    void sendFromUnitToUnconnectedScene();
    void sendExitCmd();

};

#endif // TRANSFERCHECKTESTS_H
