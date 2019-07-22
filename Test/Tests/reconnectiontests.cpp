#include "reconnectiontests.h"

ReconnectionTests::ReconnectionTests(QString rcaIp, QString sceneIp, quint16 rcaPort, quint16 scenePort,
                                     QString pathToRcaExec, int waitTime, QObject *parent)
    : QObject(parent),
      rcaIp(rcaIp), sceneIp(sceneIp), rcaPort(rcaPort), scenePort(scenePort),
      pathToRcaExec(pathToRcaExec), waitTime(waitTime)
{

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

void ReconnectionTests::reconnectUnitToRca()
{
    ControlUnit unit("t", rcaIp, rcaPort);
    bool connectedFirstTime = unit.connectToServer();
    QTest::qWait(waitTime);
    bool disconnected = unit.disconnectFromServer();
    QTest::qWait(waitTime);
    bool connectedSecondTime = unit.connectToServer();
    QTest::qWait(waitTime);

    QCOMPARE(connectedFirstTime, true);
    QCOMPARE(disconnected, true);
    QCOMPARE(connectedSecondTime, true);
}

void ReconnectionTests::reconnectPlannerToRca()
{
    Planner planner("p", rcaIp, rcaPort);
    bool connectedFirstTime = planner.connectToServer();
    QTest::qWait(waitTime);
    bool disconnected = planner.disconnectFromServer();
    QTest::qWait(waitTime);
    bool connectedSecondTime = planner.connectToServer();
    QTest::qWait(waitTime);

    QCOMPARE(connectedFirstTime, true);
    QCOMPARE(disconnected, true);
    QCOMPARE(connectedSecondTime, true);
}
