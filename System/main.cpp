#include <QApplication>
#include <QFile>
#include <QDateTime>
#include <QSettings>
#include <QDir>

#include <iostream>

#include "RobotControlAdapter/robotcontroladapter.h"

class LoggerSingleton
{
private:
    QFile file;
public:
    static LoggerSingleton& instance()
    {
        static LoggerSingleton obj;
        return obj;
    }

    void setFilename(QString filename)
    {
        file.setFileName(filename);

        if (!file.isOpen())
        {
            file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
        }
    }

    QFile& fileRef()
    {
        return file;
    }

    ~LoggerSingleton()
    {
        file.close();
    }
private:
    LoggerSingleton() {}
    LoggerSingleton(const LoggerSingleton& obj) = delete;
    LoggerSingleton(LoggerSingleton&& obj) = delete;
    LoggerSingleton& operator=(const LoggerSingleton& obj) = delete;
    LoggerSingleton& operator=(LoggerSingleton&& obj) = delete;
};

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    // Create file and console streams
    QTextStream fileStream(&LoggerSingleton::instance().fileRef());
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
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSettings settings("D:\\Qt Programs New\\RCA\\config.ini", QSettings::IniFormat);
    QString sceneIp = settings.value("HOSTS/Scene").toString();
    quint16 rcaPort = static_cast<quint16>(settings.value("PORTS/Rca").toInt());
    quint16 scenePort  = static_cast<quint16>(settings.value("PORTS/Scene").toInt());

    QString log = QDir::homePath() + "/" + settings.value("FILES/Log").toString();
    LoggerSingleton::instance().setFilename(log);

    qInstallMessageHandler(messageHandler);

    RobotControlAdapter* RCA = new RobotControlAdapter(rcaPort, sceneIp, scenePort);
    Q_UNUSED(RCA);

    return a.exec();
}
