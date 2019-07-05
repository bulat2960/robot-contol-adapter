#include "test.h"

Test::Test(QObject *parent) : QObject(parent)
{
    const QString defaultRcaIp     = "localhost";
    const QString defaultSceneIp   = "localhost";
    const quint16 defaultRcaPort   = 5555;
    const quint16 defaultScenePort = 6666;

    QSettings settings("config.ini", QSettings::IniFormat);
    rcaIp = settings.value("HOSTS/Rca", defaultRcaIp).toString();
    sceneIp = settings.value("HOSTS/Scene", defaultSceneIp).toString();
    rcaPort = static_cast<quint16>(settings.value("PORTS/Rca", defaultRcaPort).toInt());
    scenePort  = static_cast<quint16>(settings.value("PORTS/Scene", defaultScenePort).toInt());
}

bool Test::connectUnitToRca()
{
    RobotControlAdapter RCA(rcaPort, sceneIp, scenePort);
    ControlUnit unit("t", rcaIp, rcaPort);
    bool isConnected = unit.connectToServer();

    return isConnected;
}

void Test::test_data()
{
    QTest::addColumn<bool>("FunctionCall");
    QTest::addColumn<bool>("Result");

    QTest::newRow("UnitToRca") << connectUnitToRca() << true;
}

void Test::test()
{
    QFETCH(bool, FunctionCall);
    QFETCH(bool, Result);

    QCOMPARE(FunctionCall, Result);
}
