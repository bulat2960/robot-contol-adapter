#include <QApplication>
#include <QFile>
#include <QDateTime>

#include <iostream>

#include "RobotControlAdapter/robotcontroladapter.h"

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    // Create and open file
    QFile file("D:/Qt Programs New/RCA/log.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text))
    {
        return;
    }

    // Create file and console streams
    QTextStream fileStream(&file);
    QTextStream consoleStream(stdout);

    // Set current date and time
    QString currentDate = "[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz") + "]";

    // Write to file
    switch (type)
    {
        case QtDebugMsg:
            fileStream << QString("%1\nFunction \"%2\", Line %3\nDebug: %4\n\n").
                      arg(currentDate).arg(context.function).arg(context.line).arg(msg);
            break;
        case QtInfoMsg:
            fileStream << QString("%1\nFunction \"%2\", Line %3\nInfo: %4\n\n").
                      arg(currentDate).arg(context.function).arg(context.line).arg(msg);
            break;
        case QtWarningMsg:
            fileStream << QString("%1\nFunction \"%2\", Line %3\nWarning: %4\n\n").
                      arg(currentDate).arg(context.function).arg(context.line).arg(msg);
            break;
        case QtCriticalMsg:
            fileStream << QString("%1\nFunction \"%2\", Line %3\nCritical: %4\n\n").
                      arg(currentDate).arg(context.function).arg(context.line).arg(msg);
            break;
        case QtFatalMsg:
            fileStream << QString("%1\nFunction \"%2\", Line %3\nFatal: %4\n\n").
                      arg(currentDate).arg(context.function).arg(context.line).arg(msg);
            break;
    }

    // Write to console
    consoleStream << QString("%1, Function \"%2\", Line %3, %4 \n").
                     arg(currentDate).arg(context.function).arg(context.line).arg(msg);

    fileStream.flush();
    consoleStream.flush();

    file.flush();
    file.close();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler(messageHandler);

    RobotControlAdapter* RCA = new RobotControlAdapter;
    Q_UNUSED(RCA);

    return a.exec();
}
