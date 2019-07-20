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
}

void ConnectionTests::init()
{

}

void ConnectionTests::cleanup()
{

}

void ConnectionTests::connectToRcaUnitT()
{

}

void ConnectionTests::connectToRcaUnitF()
{

}

void ConnectionTests::connectToRcaPlanner()
{

}

void ConnectionTests::connectRcaToScene()
{

}

void ConnectionTests::connectRcaToNotRunningScene()
{

}

void ConnectionTests::disconnectFromRcaUnitT()
{

}

void ConnectionTests::disconnectFromRcaUnitF()
{

}

void ConnectionTests::disconnectFromRcaPlanner()
{

}
