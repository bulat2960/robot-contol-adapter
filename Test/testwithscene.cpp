#include "testwithscene.h"

TestWithScene::TestWithScene(QObject *parent) : QObject(parent)
{
    const QString defaultRcaIp     = "localhost";
    const QString defaultSceneIp   = "localhost";
    const quint16 defaultRcaPort   = 8000;
    const quint16 defaultScenePort = 8080;

    QSettings settings("../config.ini", QSettings::IniFormat);
    rcaIp = settings.value("HOSTS/Rca", defaultRcaIp).toString();
    sceneIp = settings.value("HOSTS/Scene", defaultSceneIp).toString();
    rcaPort = static_cast<quint16>(settings.value("PORTS/Rca", defaultRcaPort).toInt());
    scenePort  = static_cast<quint16>(settings.value("PORTS/Scene", defaultScenePort).toInt());

    QDir dir = QDir::current();
    dir.cdUp();
    dir.cdUp();
    dir.cd("System/release");
    pathToRcaExec = dir.path();

    waitTime = 100;
}

void TestWithScene::init()
{
    if (scene)
    {
        scene->deleteLater();
    }
    QTest::qWait(waitTime);
    scene = new Scene(scenePort);

    rcaProcess.start(pathToRcaExec + "/System", QStringList());
    QTest::qWait(waitTime);
}

void TestWithScene::cleanup()
{
    rcaProcess.kill();
    rcaProcess.waitForFinished();
    QTest::qWait(waitTime);
}

void TestWithScene::connectToRcaUnitT()
{
    ControlUnit unit("t", rcaIp, rcaPort);
    bool isConnected = unit.connectToServer();
    QTest::qWait(waitTime);

    QCOMPARE(isConnected, true);
}

void TestWithScene::connectToRcaUnitF()
{
    ControlUnit unit("f", rcaIp, rcaPort);
    bool isConnected = unit.connectToServer();
    QTest::qWait(waitTime);

    QCOMPARE(isConnected, true);
}

void TestWithScene::connectPlannerToRca()
{
    Planner planner("p", rcaIp, rcaPort);
    bool isConnected = planner.connectToServer();
    QTest::qWait(waitTime);

    QCOMPARE(isConnected, true);
}

void TestWithScene::sendMsgFromPlannerToUnitT()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);
    planner.sendMsg("t:message");
    QTest::qWait(waitTime);

    QString msg = unit.getLastMessage();

    QCOMPARE("message", msg);
}

void TestWithScene::sendMsgFromPlannerToUnitF()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    ControlUnit unit("f", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);
    planner.sendMsg("f:message");
    QTest::qWait(waitTime);

    QString msg = unit.getLastMessage();

    QCOMPARE("message", msg);
}

void TestWithScene::sendMsgFromPlannerToUnconnectedUnitT()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    ControlUnit unit("t", rcaIp, rcaPort);
    QTest::qWait(waitTime);
    planner.sendMsg("t:message");
    QTest::qWait(waitTime);

    QString msg = unit.getLastMessage();

    QVERIFY(msg.isEmpty());
}

void TestWithScene::sendMsgFromPlannerToUnconnectedUnitF()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    ControlUnit unit("f", rcaIp, rcaPort);
    QTest::qWait(waitTime);
    planner.sendMsg("f:message");
    QTest::qWait(waitTime);

    QString msg = unit.getLastMessage();

    QVERIFY(msg.isEmpty());
}

void TestWithScene::disconnectFromRcaUnitT()
{
    ControlUnit unit("t", rcaIp, rcaPort);
    bool wasConnected = unit.connectToServer();
    bool nowDisconnected = unit.disconnectFromServer();
    QTest::qWait(waitTime);

    bool result = (wasConnected && nowDisconnected);

    QCOMPARE(result, true);
}

void TestWithScene::disconnectFromRcaUnitF()
{
    ControlUnit unit("f", rcaIp, rcaPort);
    bool wasConnected = unit.connectToServer();
    bool nowDisconnected = unit.disconnectFromServer();
    QTest::qWait(waitTime);

    bool result = (wasConnected && nowDisconnected);

    QCOMPARE(result, true);
}

void TestWithScene::sendExitCmd()
{
    Planner planner("p", rcaIp, rcaPort);
    bool isConnectedPlanner = planner.connectToServer();
    ControlUnit unitT("t", rcaIp, rcaPort);
    bool isConnectedUnitT = unitT.connectToServer();
    ControlUnit unitF("f", rcaIp, rcaPort);
    bool isConnectedUnitF = unitF.connectToServer();
    QTest::qWait(waitTime);
    planner.sendMsg("e");
    QTest::qWait(waitTime);

    bool isDisconnectedPlanner = planner.isDisconnected();
    bool isDisconnetedUnitT = unitT.isDisconnected();
    bool isDisconnectedUnitF = unitF.isDisconnected();

    bool wasConnected = (isConnectedPlanner && isConnectedUnitT && isConnectedUnitF);
    bool nowDisconnected = (isDisconnectedPlanner && isDisconnetedUnitT && isDisconnectedUnitF);
    bool result = (wasConnected && nowDisconnected);

    QCOMPARE(result, true);
}

