#include "reconnectiontests.h"

ReconnectionTests::ReconnectionTests(QString rcaIp, QString sceneIp, quint16 rcaPort, quint16 scenePort,
                                     QString pathToRcaExec, int waitTime, QObject *parent) : QObject(parent)
{
    this->rcaIp = rcaIp;
    this->sceneIp = sceneIp;
    this->rcaPort = rcaPort;
    this->scenePort = scenePort;

    this->waitTime = waitTime;

    this->pathToRcaExec = pathToRcaExec;
}

void ReconnectionTests::init()
{
    rcaProcess.start(pathToRcaExec + "/System", QStringList());
    QTest::qWait(waitTime);
}

void ReconnectionTests::cleanup()
{
    rcaProcess.kill();
    rcaProcess.waitForFinished();
    QTest::qWait(waitTime);
}

void ReconnectionTests::reconnectToRcaUnitT()
{
    ControlUnit unit("t", rcaIp, rcaPort);
    bool connectedFirstTime = unit.connectToServer();
    QTest::qWait(waitTime);
    bool disconnected = unit.disconnectFromServer();
    QTest::qWait(waitTime);
    bool connectedSecondTime = unit.connectToServer();
    QTest::qWait(waitTime);
    QCOMPARE(connectedFirstTime && disconnected && connectedSecondTime, true);
}

void ReconnectionTests::reconnectToRcaUnitF()
{
    ControlUnit unit("f", rcaIp, rcaPort);
    bool connectedFirstTime = unit.connectToServer();
    QTest::qWait(waitTime);
    bool disconnected = unit.disconnectFromServer();
    QTest::qWait(waitTime);
    bool connectedSecondTime = unit.connectToServer();
    QTest::qWait(waitTime);
    QCOMPARE(connectedFirstTime && disconnected && connectedSecondTime, true);
}

void ReconnectionTests::reconnectToRcaPlanner()
{
    Planner planner("f", rcaIp, rcaPort);
    bool connectedFirstTime = planner.connectToServer();
    QTest::qWait(waitTime);
    bool disconnected = planner.disconnectFromServer();
    QTest::qWait(waitTime);
    bool connectedSecondTime = planner.connectToServer();
    QTest::qWait(waitTime);
    QCOMPARE(connectedFirstTime && disconnected && connectedSecondTime, true);
}
