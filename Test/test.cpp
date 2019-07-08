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

void Test::connectUnitToRca()
{
    //ControlUnit unit("t", rcaIp, rcaPort);
    //bool isConnected = unit.connectToServer();
    //QCOMPARE(isConnected, true);
}

void Test::connectPlannerToRca()
{
    //Planner planner("p", rcaIp, rcaPort);
    //bool isConnected = planner.connectToServer();
    //QCOMPARE(isConnected, true);
}

void Test::sendMsgFromPlannerToUnitT()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    QTest::qSleep(3000);
    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qSleep(3000);
    planner.sendMsg("t:message");
    QTest::qSleep(3000);
    QString msg = unit.getLastMessage();
    QCOMPARE("message", msg);
}

