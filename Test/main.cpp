#include <QCoreApplication>
#include <QFile>
#include <QSettings>
#include <QProcess>
#include <QDir>

#include "Objects/controlunit.h"
#include "Objects/planner.h"
#include "Objects/scene.h"

#include "test.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    auto res = QTest::qExec(new Test, argc, argv);
    return res;
}
