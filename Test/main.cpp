#include <QApplication>

#include "Objects/controlunit.h"
#include "Objects/planner.h"
#include "Objects/scene.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString serverIp = "localhost";
    quint16 serverPort = 5555;

    quint16 scenePort = 1111;

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
