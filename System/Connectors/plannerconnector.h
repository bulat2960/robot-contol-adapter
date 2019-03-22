#ifndef PLANNERCONNECTOR_H
#define PLANNERCONNECTOR_H

#include <QTcpSocket>

class PlannerConnector : public QObject
{
    Q_OBJECT
private:
    QTcpSocket* socket;

public:
    PlannerConnector(QTcpSocket* socket);

    PlannerConnector(const PlannerConnector&) = delete;
    PlannerConnector& operator=(const PlannerConnector&) = delete;
    PlannerConnector(PlannerConnector&&) = delete;
    PlannerConnector& operator=(PlannerConnector&&) = delete;

    ~PlannerConnector();

public slots:
    void slotRead();

signals:
    void signalMsgReceived(QByteArray msg);
    void signalClose(QByteArray msg);
};

#endif // PLANNERCONNECTOR_H
