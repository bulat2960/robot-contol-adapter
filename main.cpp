#include <QApplication>

#include <stdio.h>
#include <stdlib.h>

#include "System/robotcontroladapter.h"
#include "Test/ControlUnit/controlunit.h"
#include "Test/Planner/planner.h"
#include "Test/Scene/scene.h"

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QFile file("D:/Qt Programs New/RCA/log.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text))
    {
        return;
    }

    QTextStream stream(&file);
    QString currentDate = "[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "]";

    switch (type)
    {
        case QtDebugMsg:
            stream << QString("Date: %1 \n\tDebug: File \"%2\", Function \"%3\", Line %4 \n\tMessage %5 \n\n").
                      arg(currentDate).arg(context.file).arg(context.function).arg(context.line).arg(msg);
            break;
        case QtWarningMsg:
            stream << QString("Date: %1 \n\tWarning: File \"%2\", Function \"%3\", Line %4 \n\tMessage %5 \n\n").
                      arg(currentDate).arg(context.file).arg(context.function).arg(context.line).arg(msg);
            break;
        case QtCriticalMsg:
            stream << QString("Date: %1 \n\tCritical: File \"%2\", Function \"%3\", Line %4 \n\tMessage %5 \n\n").
                      arg(currentDate).arg(context.file).arg(context.function).arg(context.line).arg(msg);
            break;
        case QtFatalMsg:
            stream << QString("Date: %1 \n\tFatal: File \"%2\", Function \"%3\", Line %4 \n\tMessage %5 \n\n").
                      arg(currentDate).arg(context.file).arg(context.function).arg(context.line).arg(msg);
            break;
        case QtInfoMsg:
            stream << QString("Date: %1 \n\tInfo: File \"%2\", Function \"%3\", Line %4 \n\tMessage %5 \n\n").
                      arg(currentDate).arg(context.file).arg(context.function).arg(context.line).arg(msg);
            break;
    }

    stream.flush();
    file.flush();
    file.close();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler(messageHandler);

    Scene* scene = new Scene;
    Q_UNUSED(scene);

    RobotControlAdapter* RCA = new RobotControlAdapter;
    Q_UNUSED(RCA);

    Planner* planner = new Planner("p");
    planner->show();

    ControlUnit* unit1 = new ControlUnit("t");
    unit1->show();

    ControlUnit* unit2 = new ControlUnit("f");
    unit2->show();

    return a.exec();
}
