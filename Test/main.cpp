#include <QApplication>

#include "Objects/controlunit.h"
#include "Objects/planner.h"
#include "Objects/scene.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Scene* scene = new Scene;
    Q_UNUSED(scene);

    Planner* planner = new Planner("p");
    planner->show();

    ControlUnit* unit1 = new ControlUnit("t");
    unit1->show();

    ControlUnit* unit2 = new ControlUnit("f");
    unit2->show();

    return a.exec();
}
