#include <QApplication>
#include <QFile>
#include <QSettings>

#include "Objects/controlunit.h"
#include "Objects/planner.h"
#include "Objects/scene.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSettings settings("config.ini", QSettings::IniFormat);
    QString rcaIp = settings.value("HOSTS/Rca").toString();
    quint16 rcaPort = static_cast<quint16>(settings.value("PORTS/Rca").toInt());
    quint16 scenePort  = static_cast<quint16>(settings.value("PORTS/Scene").toInt());

    Scene* scene = new Scene(scenePort);
    Q_UNUSED(scene);

    Planner* planner = new Planner("p", rcaIp, rcaPort);
    planner->show();

    ControlUnit* unit1 = new ControlUnit("t", rcaIp, rcaPort);
    unit1->show();

    ControlUnit* unit2 = new ControlUnit("f", rcaIp, rcaPort);
    unit2->show();

    return a.exec();
}
