#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <QtTest>

#include "Objects/scene.h"
#include "Objects/controlunit.h"
#include "Objects/planner.h"

class Test : public QObject
{
    Q_OBJECT
private:
    QString rcaIp;
    QString sceneIp;
    quint16 rcaPort;
    quint16 scenePort;
public:
    explicit Test(QObject *parent = nullptr);
signals:

public slots:

private slots:
    void connectToRcaUnitT();
    void connectToRcaUnitF();
    void connectPlannerToRca();
    void sendMsgFromPlannerToUnitT();
    void sendMsgFromPlannerToUnitF();
    void sendExitCmd();
    void disconnectFromRcaUnitT();
    void disconnectFromRcaUnitF();
};

#endif // TEST_H
