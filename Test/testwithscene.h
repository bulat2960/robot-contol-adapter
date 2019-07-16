#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <QtTest>

#include <QDir>
#include <QProcess>

#include "Objects/scene.h"
#include "Objects/controlunit.h"
#include "Objects/planner.h"

class TestWithScene : public QObject
{
    Q_OBJECT
private:
    QString rcaIp;
    QString sceneIp;
    quint16 rcaPort;
    quint16 scenePort;

    QString pathToRcaExec;
    QProcess rcaProcess;

    Scene* scene;

    int waitTime;
public:
    explicit TestWithScene(QObject *parent = nullptr);
signals:

public slots:

private slots:
    void init();
    void cleanup();
    void connectToRcaUnitT();
    void connectToRcaUnitF();
    void connectPlannerToRca();
    void sendMsgFromPlannerToUnitT();
    void sendMsgFromPlannerToUnitF();
    void sendMsgFromPlannerToUnconnectedUnitT();
    void sendMsgFromPlannerToUnconnectedUnitF();
    void disconnectFromRcaUnitT();
    void disconnectFromRcaUnitF();
    void sendExitCmd();
};

#endif // TEST_H
