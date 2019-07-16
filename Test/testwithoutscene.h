#ifndef TESTWITHOUTSCENE_H
#define TESTWITHOUTSCENE_H

#include <QObject>
#include <QtTest>

#include <QProcess>

#include "Objects/scene.h"
#include "Objects/controlunit.h"
#include "Objects/planner.h"

class TestWithoutScene : public QObject
{
    Q_OBJECT
private:
    QString rcaIp;
    QString sceneIp;
    quint16 rcaPort;
    quint16 scenePort;

    QString pathToRcaExec;
    QProcess rcaProcess;

    int waitTime;
public:
    explicit TestWithoutScene(QString rcaIp, QString sceneIp, quint16 rcaPort, quint16 scenePort,
                           QString pathToRcaExec, int waitTime, QObject *parent = nullptr);
signals:

public slots:

};

#endif // TESTWITHOUTSCENE_H
