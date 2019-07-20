#include "transfertests.h"

TransferTests::TransferTests(QString rcaIp, QString sceneIp, quint16 rcaPort, quint16 scenePort,
                             QString pathToRcaExec, int waitTime, QObject *parent) : QObject(parent)
{
    this->rcaIp = rcaIp;
    this->sceneIp = sceneIp;
    this->rcaPort = rcaPort;
    this->scenePort = scenePort;

    this->waitTime = waitTime;

    this->pathToRcaExec = pathToRcaExec;
}

void TransferTests::init()
{

}

void TransferTests::cleanup()
{

}

void TransferTests::sendFromPlannerWithoutResponseToUnitT()
{

}

void TransferTests::sendFromPlannerWithoutResponseToUnitF()
{

}

void TransferTests::sendToSceneFromUnitT()
{

}

void TransferTests::sendToSceneFromUnitF()
{

}

void TransferTests::sendFromPlannerWithResponseToUnitT()
{

}

void TransferTests::sendFromPlannerWithResponseToUnitF()
{

}

void TransferTests::sendFromPlannerToUnconnectedUnitT()
{

}

void TransferTests::sendFromPlannerToUnconnectedUnitF()
{

}

void TransferTests::sendToUnconnectedSceneFromUnitT()
{

}

void TransferTests::sendToUnconnectedSceneFromUnitF()
{

}

void TransferTests::sendExitCmd()
{

}
