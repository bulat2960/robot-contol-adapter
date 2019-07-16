#include "testwithoutscene.h"

TestWithoutScene::TestWithoutScene(QString rcaIp, QString sceneIp, quint16 rcaPort, quint16 scenePort,
                                   QString pathToRcaExec, int waitTime, QObject *parent) : QObject(parent)
{
    this->rcaIp = rcaIp;
    this->sceneIp = sceneIp;
    this->rcaPort = rcaPort;
    this->scenePort = scenePort;

    this->waitTime = waitTime;

    this->pathToRcaExec = pathToRcaExec;
}
