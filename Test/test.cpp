#include "test.h"

Test::Test(QObject *parent) : QObject(parent)
{
    const QString defaultRcaIp     = "localhost";
    const QString defaultSceneIp   = "localhost";
    const quint16 defaultRcaPort   = 8000;
    const quint16 defaultScenePort = 8080;

    QSettings settings("config.ini", QSettings::IniFormat);
    rcaIp = defaultRcaIp; //settings.value("HOSTS/Rca", defaultRcaIp).toString();
    sceneIp = defaultSceneIp; //settings.value("HOSTS/Scene", defaultSceneIp).toString();
    rcaPort = defaultRcaPort; //static_cast<quint16>(settings.value("PORTS/Rca", defaultRcaPort).toInt());
    scenePort  = defaultScenePort; //static_cast<quint16>(settings.value("PORTS/Scene", defaultScenePort).toInt());
}

void Test::connectUnitToRca()
{
    ControlUnit unit("t", rcaIp, rcaPort);
    bool isConnected = unit.connectToServer();

    QCOMPARE(isConnected, true);
}

