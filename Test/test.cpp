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

void Test::connectUnitToRca()
{
    ControlUnit unit("t", rcaIp, rcaPort);
    bool isConnected = unit.connectToServer();

    QCOMPARE(isConnected, true);
}

