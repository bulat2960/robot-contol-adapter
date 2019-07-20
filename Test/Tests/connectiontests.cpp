#include "connectiontests.h"

ConnectionTests::ConnectionTests(QString rcaIp, QString sceneIp, quint16 rcaPort, quint16 scenePort,
                                 QString pathToRcaExec, int waitTime, QObject *parent) : QObject(parent)
{
    this->rcaIp = rcaIp;
    this->sceneIp = sceneIp;
    this->rcaPort = rcaPort;
    this->scenePort = scenePort;

    this->waitTime = waitTime;

    this->pathToRcaExec = pathToRcaExec;

    this->scene = nullptr;
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

void ConnectionTests::connectToRcaUnitT()
{
    rcaProcess.start(pathToRcaExec + "/System", QStringList());
    QTest::qWait(waitTime);

    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);

    QCOMPARE(unit.isConnected(), true);
}

void ConnectionTests::connectToRcaUnitF()
{
    rcaProcess.start(pathToRcaExec + "/System", QStringList());
    QTest::qWait(waitTime);

    ControlUnit unit("f", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);

    QCOMPARE(unit.isConnected(), true);
}

void ConnectionTests::connectToRcaPlanner()
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

    QCOMPARE(scene->isRcaDisconnected(), true);
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

void ConnectionTests::disconnectFromRcaUnitT()
{
    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);
    unit.disconnectFromServer();
    QTest::qWait(waitTime);

    QCOMPARE(unit.isDisconnected(), true);
}

void ConnectionTests::disconnectFromRcaUnitF()
{
    ControlUnit unit("f", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);
    unit.disconnectFromServer();
    QTest::qWait(waitTime);

    QCOMPARE(unit.isDisconnected(), true);
}

void ConnectionTests::disconnectFromRcaPlanner()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    QTest::qWait(waitTime);
    planner.disconnectFromServer();
    QTest::qWait(waitTime);

    QCOMPARE(planner.isDisconnected(), true);
}
