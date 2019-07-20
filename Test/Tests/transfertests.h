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
signals:

public slots:

private slots:
    void init();
    void cleanup();
    void sendFromPlannerWithoutResponseToUnitT();
    void sendFromPlannerWithoutResponseToUnitF();
    void sendToSceneFromUnitT();
    void sendToSceneFromUnitF();
    void sendFromPlannerWithResponseToUnitT();
    void sendFromPlannerWithResponseToUnitF();
    void sendFromPlannerToUnconnectedUnitT();
    void sendFromPlannerToUnconnectedUnitF();
    void sendToUnconnectedSceneFromUnitT();
    void sendToUnconnectedSceneFromUnitF();
    void sendExitCmd();

};

#endif // TRANSFERCHECKTESTS_H
