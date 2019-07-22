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
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    ControlUnit unit("t", rcaIp, rcaPort);
    bool connected = unit.connectToServer();
    QTest::qWait(waitTime);
    planner.sendMsg("t:messageOnConnect");
    QTest::qWait(waitTime);
    QString receivedOnConnect = unit.getLastMessage();
    bool disconnected = unit.disconnectFromServer();
    QTest::qWait(waitTime);
    bool reconnected = unit.connectToServer();
    QTest::qWait(waitTime);
    planner.sendMsg("t:messageOnReconnect");
    QTest::qWait(waitTime);
    QString receivedOnReconnect = unit.getLastMessage();

    QCOMPARE(connected, true);
    QCOMPARE(receivedOnConnect, "messageOnConnect");
    QCOMPARE(disconnected, true);
    QCOMPARE(receivedOnReconnect, "messageOnReconnect");
    QCOMPARE(reconnected, true);
}

void ReconnectionTests::reconnectPlannerToRca()
{
    Planner planner("p", rcaIp, rcaPort);
    bool connected = planner.connectToServer();
    QTest::qWait(waitTime);
    bool disconnected = planner.disconnectFromServer();
    QTest::qWait(waitTime);
    bool reconnected = planner.connectToServer();
    QTest::qWait(waitTime);

    QCOMPARE(connected, true);
    QCOMPARE(disconnected, true);
    QCOMPARE(reconnected, true);
}
