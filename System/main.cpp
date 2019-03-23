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
    QTextStream* fileStream;
    QTextStream* consoleStream;
public:
    static LoggerSingleton& instance()
    {
        static LoggerSingleton obj;
        return obj;
    }

    void initStreams(QString name)
    {
        QFile* file = new QFile(name);
        if (!file->isOpen())
        {
            file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
        }

        fileStream = new QTextStream(file);
        consoleStream = new QTextStream(stdout);
    }

    QTextStream& getFileStream() const
    {
        return *fileStream;
    }

    QTextStream& getConsoleStream() const
    {
        return *consoleStream;
    }
private:
    LoggerSingleton() {}

    LoggerSingleton(const LoggerSingleton& obj) = delete;
    LoggerSingleton(LoggerSingleton&& obj) = delete;
    LoggerSingleton& operator=(const LoggerSingleton& obj) = delete;
    LoggerSingleton& operator=(LoggerSingleton&& obj) = delete;

    ~LoggerSingleton()
    {
        delete fileStream;
        delete consoleStream;
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
    LoggerSingleton::instance().getFileStream() << QString("%1\nFunction \"%2\", Line %3\n" + msgType + " %4\n\n").
              arg(currentDate).arg(context.function).arg(context.line).arg(msg);

    // Write to console
    LoggerSingleton::instance().getConsoleStream() << QString("%1, Function \"%2\", Line %3, " + msgType + ": %4\n").
                     arg(currentDate).arg(context.function).arg(context.line).arg(msg);

    // Flush
    LoggerSingleton::instance().getFileStream().flush();
    LoggerSingleton::instance().getConsoleStream().flush();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSettings settings("config.ini", QSettings::IniFormat);
    QString sceneIp = settings.value("HOSTS/Scene").toString();
    quint16 rcaPort = static_cast<quint16>(settings.value("PORTS/Rca").toInt());
    quint16 scenePort  = static_cast<quint16>(settings.value("PORTS/Scene").toInt());

    QString log = QDir::homePath() + "/" + settings.value("FILES/Log").toString();
    LoggerSingleton::instance().initStreams(log);

    qInstallMessageHandler(messageHandler);

    RobotControlAdapter* RCA = new RobotControlAdapter(rcaPort, sceneIp, scenePort);
    Q_UNUSED(RCA);

    return a.exec();
}
