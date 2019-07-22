#include "transfertests.h"

TransferTests::TransferTests(QString rcaIp, QString sceneIp, quint16 rcaPort, quint16 scenePort,
                             QString pathToRcaExec, int waitTime, QObject *parent)
    : QObject(parent),
      rcaIp(rcaIp), sceneIp(sceneIp), rcaPort(rcaPort), scenePort(scenePort),
      pathToRcaExec(pathToRcaExec), waitTime(waitTime), scene(nullptr)
{

}

void TransferTests::init()
{
    scene = new Scene(scenePort);
    scene->startServer();

    rcaProcess.start(pathToRcaExec + "/System", QStringList());
    QTest::qWait(waitTime);
}

void TransferTests::cleanup()
{
    scene->deleteLater();
    rcaProcess.kill();
    rcaProcess.waitForFinished();
    QTest::qWait(waitTime);
}

void TransferTests::sendFromPlannerToUnitWithoutResponse()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);
    planner.sendMsg("t:message");
    QTest::qWait(waitTime);
    bool check1 = (unit.getLastMessage() == "message");
    bool check2 = scene->getLastMessage().isEmpty();

    QCOMPARE(check1 && check2, true);
}

void TransferTests::sendFromUnitToScene()
{
    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);
    unit.sendMsgToScene("message");
    QTest::qWait(waitTime);

    QCOMPARE(scene->getLastMessage(), "message");
}

void TransferTests::sendFromPlannerToUnitWithResponse()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);
    planner.sendMsg("t:message");
    QTest::qWait(waitTime);
    bool check1 = (unit.getLastMessage() == "message");
    unit.sendMsgToScene("message");
    QTest::qWait(waitTime);
    bool check2 = (scene->getLastMessage() == "message");

    QCOMPARE(check1 && check2, true);
}

void TransferTests::sendFromPlannerToUnconnectedUnit()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    ControlUnit unit("t", rcaIp, rcaPort);
    QTest::qWait(waitTime);
    planner.sendMsg("t:message");

    QCOMPARE(unit.getLastMessage().isEmpty(), true);
}

void TransferTests::sendFromUnitToUnconnectedScene()
{
    scene->closeServer();
    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);
    unit.sendMsgToScene("message");
    QTest::qWait(waitTime);

    QCOMPARE(scene->getLastMessage().isEmpty(), true);
}

void TransferTests::sendExitCmd()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    ControlUnit unitT("t", rcaIp, rcaPort);
    unitT.connectToServer();
    ControlUnit unitF("f", rcaIp, rcaPort);
    unitF.connectToServer();
    QTest::qWait(waitTime);
    planner.sendMsg("e");
    QTest::qWait(waitTime);
    bool check1 = unitT.isDisconnected();
    bool check2 = unitF.isDisconnected();
    bool check3 = planner.isDisconnected();

    QCOMPARE(check1 && check2 && check3, true);
}
