#include <QCoreApplication>
#include <QFile>
#include <QDateTime>
#include <QSettings>
#include <QDir>

#include <iostream>

#include "RobotControlAdapter/robotcontroladapter.h"

class LoggerSingleton
{
private:
    QList<QTextStream*> fileStreams;
    QTextStream consoleStream;

    const int maxNumberOfStreams = 8;
public:
    static LoggerSingleton& instance()
    {
        static LoggerSingleton obj;
        return obj;
    }

    void addFile(QString name)
    {
        if (fileStreams.size() <= maxNumberOfStreams)
        {
            QFile* file = new QFile(name);
            if (!file->isOpen())
            {
                file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
            }
            fileStreams.append(new QTextStream(file));
        }
    }

    void logMsg(QString msg)
    {
        for (auto& fileStream : fileStreams)
        {
            (*fileStream) << msg;
            fileStream->flush();
        }
        consoleStream << msg;
        consoleStream.flush();
    }
private:
    LoggerSingleton() : consoleStream(stdout) {}

    LoggerSingleton(const LoggerSingleton& obj) = delete;
    LoggerSingleton(LoggerSingleton&& obj) = delete;
    LoggerSingleton& operator=(const LoggerSingleton& obj) = delete;
    LoggerSingleton& operator=(LoggerSingleton&& obj) = delete;

    ~LoggerSingleton()
    {
        for (auto& fileStream : fileStreams)
        {
            delete fileStream->device();
            delete fileStream;
        }
    }
};

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    // Set current date and time
    QString currentDate = "[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz") + "]";

    // Choose msg type
    QString msgType;
    switch (type)
    {
        case QtDebugMsg:    msgType = "Debug";    break;
        case QtInfoMsg:     msgType = "Info";     break;
        case QtWarningMsg:  msgType = "Warning";  break;
        case QtCriticalMsg: msgType = "Critical"; break;
        case QtFatalMsg:    msgType = "Fatal";    break;
    }

    // Write to file
    LoggerSingleton::instance().logMsg( QString("%1, Function \"%2\", Line %3, " + msgType + ": %4\n").
              arg(currentDate).arg(context.function).arg(context.line).arg(msg));
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    const QString defaultSceneIp   = "localhost";
    const quint16 defaultRcaPort   = 8000;
    const quint16 defaultScenePort = 8080;
	
    QSettings settings("../config.ini", QSettings::IniFormat);
    QString sceneIp = settings.value("HOSTS/Scene", defaultSceneIp).toString();
    quint16 rcaPort = static_cast<quint16>(settings.value("PORTS/Rca", defaultRcaPort).toInt());
    quint16 scenePort = static_cast<quint16>(settings.value("PORTS/Scene", defaultScenePort).toInt());

    QString log = QDir::homePath() + "/" + settings.value("FILES/Log").toString();
    LoggerSingleton::instance().addFile(log);

    qInstallMessageHandler(messageHandler);

    RobotControlAdapter RCA(rcaPort, sceneIp, scenePort);
    QObject::connect(&RCA, &RobotControlAdapter::signalShutdown, &a, QCoreApplication::quit);

    return a.exec();
}
