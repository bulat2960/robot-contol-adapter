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
    scene = new Scene(scenePort);
    rcaProcess.start(pathToRcaExec + "/System", QStringList());
    QTest::qWait(waitTime);
}

void ReconnectionTests::cleanup()
{
    scene->deleteLater();
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

void ReconnectionTests::reconnectRcaToScene()
{
    scene->closeServer();
    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);
    unit.sendMsgToScene("message1");
    QTest::qWait(waitTime);
    scene->startServer();
    QTest::qWait(waitTime);
    unit.connectToServer();
    QTest::qWait(waitTime);
    unit.sendMsgToScene("message2");
    QTest::qWait(waitTime);

    QVERIFY(scene->messagesCount() == 2);
}

void ReconnectionTests::reconnectPlannerToRca()
{
    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    Planner planner("p", rcaIp, rcaPort);
    bool connected = planner.connectToServer();
    QTest::qWait(waitTime);
    planner.sendMsg("t:123");
    QTest::qWait(waitTime);
    bool afterConnectCorrectState = (unit.getLastMessage() == "123" && unit.messagesCount() == 1);
    bool disconnected = planner.disconnectFromServer();
    QTest::qWait(waitTime);
    planner.sendMsg("t:456");
    QTest::qWait(waitTime);
    bool afterDisconnectCorrectState = (unit.getLastMessage() == "123" && unit.messagesCount() == 1);
    bool reconnected = planner.connectToServer();
    QTest::qWait(waitTime);
    planner.sendMsg("t:789");
    QTest::qWait(waitTime);
    bool afterReconnectCorrectState = (unit.getLastMessage() == "789" && unit.messagesCount() == 2);

    QCOMPARE(connected, true);
    QCOMPARE(disconnected, true);
    QCOMPARE(reconnected, true);
    QCOMPARE(afterConnectCorrectState, true);
    QCOMPARE(afterDisconnectCorrectState, true);
    QCOMPARE(afterReconnectCorrectState, true);
}
