#include <QApplication>

#include "robotcontroladapter.h"
#include "controlunit.h"
#include "planner.h"
#include "scene.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Scene* scene = new Scene;

    RobotControlAdapter* RCA = new RobotControlAdapter;

    Planner* planner = new Planner("p");
    planner->show();

    ControlUnit* unit1 = new ControlUnit("t");
    unit1->show();

    ControlUnit* unit2 = new ControlUnit("f");
    unit2->show();

    return a.exec();
}
