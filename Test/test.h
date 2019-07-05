#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <QtTest>

#include "../System/RobotControlAdapter/robotcontroladapter.h"
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

    bool connectUnitToRca();
    // еще набор таких функций

signals:

public slots:

private slots:
    void test_data();
    void test();
};

#endif // TEST_H
