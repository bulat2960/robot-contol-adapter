#include <QApplication>
#include <QFile>
#include <QSettings>

#include "Objects/controlunit.h"
#include "Objects/planner.h"
#include "Objects/scene.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSettings settings("D:\\Qt Programs New\\RCA\\config.ini", QSettings::IniFormat);
    QString serverIp = settings.value("HOSTS/Host").toString();
    quint16 serverPort = static_cast<quint16>(settings.value("PORTS/Rca").toInt());
    quint16 scenePort  = static_cast<quint16>(settings.value("PORTS/Scene").toInt());

    Scene* scene = new Scene(scenePort);
    Q_UNUSED(scene);

    Planner* planner = new Planner("p", serverIp, serverPort);
    planner->show();

    ControlUnit* unit1 = new ControlUnit("t", serverIp, serverPort);
    unit1->show();

    ControlUnit* unit2 = new ControlUnit("f", serverIp, serverPort);
    unit2->show();

    return a.exec();
}
