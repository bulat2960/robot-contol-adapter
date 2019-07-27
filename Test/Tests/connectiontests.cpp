#include "connectiontests.h"

ConnectionTests::ConnectionTests(QString rcaIp, QString sceneIp, quint16 rcaPort, quint16 scenePort,
                                 QString pathToRcaExec, int waitTime, QObject *parent)
    : QObject(parent),
      rcaIp(rcaIp), sceneIp(sceneIp), rcaPort(rcaPort), scenePort(scenePort),
      pathToRcaExec(pathToRcaExec), waitTime(waitTime), scene(nullptr)
{

}

void ConnectionTests::init()
{
    scene = new Scene(scenePort);
}

void ConnectionTests::cleanup()
{
    scene->deleteLater();
    rcaProcess.kill();
    rcaProcess.waitForFinished();
    QTest::qWait(waitTime);
}

void ConnectionTests::connectUnitToRca()
{
    rcaProcess.start(pathToRcaExec + "/System", QStringList());
    QTest::qWait(waitTime);

    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);

    QCOMPARE(unit.isConnected(), true);
}

void ConnectionTests::connectPlannerToRca()
{
    rcaProcess.start(pathToRcaExec + "/System", QStringList());
    QTest::qWait(waitTime);

    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    QTest::qWait(waitTime);

    QCOMPARE(planner.isConnected(), true);
}

void ConnectionTests::connectRcaToScene()
{
    scene->startServer();
    QTest::qWait(waitTime);
    rcaProcess.start(pathToRcaExec + "/System", QStringList());
    QTest::qWait(waitTime);

    QCOMPARE(scene->isRcaConnected(), true);
}

void ConnectionTests::connectRcaToNotRunningScene()
{
    rcaProcess.start(pathToRcaExec + "/System", QStringList());
    QTest::qWait(waitTime);
    scene->startServer();
    QTest::qWait(waitTime);
    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);
    unit.sendMsgToScene("message");
    QTest::qWait(waitTime);

    QVERIFY(scene->getLastMessage().isEmpty());
}

void ConnectionTests::disconnectRcaFromScene()
{
    scene->startServer();
    QTest::qWait(waitTime);
    rcaProcess.start(pathToRcaExec + "/System", QStringList());
    QTest::qWait(waitTime);
    rcaProcess.kill();
    rcaProcess.waitForFinished();
    QTest::qWait(waitTime);

    QCOMPARE(scene->isRcaDisconnected(), true);
}

void ConnectionTests::disconnectUnitFromRca()
{
    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);
    unit.disconnectFromServer();
    QTest::qWait(waitTime);

    QCOMPARE(unit.isDisconnected(), true);
}

void ConnectionTests::disconnectPlannerFromRca()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    QTest::qWait(waitTime);
    planner.disconnectFromServer();
    QTest::qWait(waitTime);

    QCOMPARE(planner.isDisconnected(), true);
}
