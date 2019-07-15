#include "test.h"

Test::Test(QObject *parent) : QObject(parent)
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
}

void Test::connectToRcaUnitT()
{
    ControlUnit unit("t", rcaIp, rcaPort);
    bool isConnected = unit.connectToServer();
    QTest::qWait(1000);

    QCOMPARE(isConnected, true);
}

void Test::connectToRcaUnitF()
{
    ControlUnit unit("f", rcaIp, rcaPort);
    bool isConnected = unit.connectToServer();
    QTest::qWait(1000);

    QCOMPARE(isConnected, true);
}

void Test::connectPlannerToRca()
{
    Planner planner("p", rcaIp, rcaPort);
    bool isConnected = planner.connectToServer();
    QTest::qWait(1000);

    QCOMPARE(isConnected, true);
}

void Test::sendMsgFromPlannerToUnitT()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(1000);
    planner.sendMsg("t:message");
    QTest::qWait(1000);

    QString msg = unit.getLastMessage();

    QCOMPARE("message", msg);
}

void Test::sendMsgFromPlannerToUnitF()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    ControlUnit unit("f", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(1000);
    planner.sendMsg("f:message");
    QTest::qWait(1000);

    QString msg = unit.getLastMessage();

    QCOMPARE("message", msg);
}

void Test::disconnectFromRcaUnitT()
{
    ControlUnit unit("t", rcaIp, rcaPort);
    bool wasConnected = unit.connectToServer();
    bool nowDisconnected = unit.disconnectFromServer();
    QTest::qWait(1000);

    bool result = (wasConnected && nowDisconnected);

    QCOMPARE(result, true);
}

void Test::disconnectFromRcaUnitF()
{
    ControlUnit unit("f", rcaIp, rcaPort);
    bool wasConnected = unit.connectToServer();
    bool nowDisconnected = unit.disconnectFromServer();
    QTest::qWait(1000);

    bool result = (wasConnected && nowDisconnected);

    QCOMPARE(result, true);
}

void Test::sendExitCmd()
{
    Planner planner("p", rcaIp, rcaPort);
    bool isConnectedPlanner = planner.connectToServer();
    ControlUnit unitT("t", rcaIp, rcaPort);
    bool isConnectedUnitT = unitT.connectToServer();
    ControlUnit unitF("f", rcaIp, rcaPort);
    bool isConnectedUnitF = unitF.connectToServer();
    QTest::qWait(1000);
    planner.sendMsg("e");
    QTest::qWait(1000);

    bool isDisconnectedPlanner = planner.isDisconnected();
    bool isDisconnetedUnitT = unitT.isDisconnected();
    bool isDisconnectedUnitF = unitF.isDisconnected();

    bool wasConnected = (isConnectedPlanner && isConnectedUnitT && isConnectedUnitF);
    bool nowDisconnected = (isDisconnectedPlanner && isDisconnetedUnitT && isDisconnectedUnitF);
    bool result = (wasConnected && nowDisconnected);

    QCOMPARE(result, true);
}

