#ifndef CONTROLUNITCONNECTOR_H
#define CONTROLUNITCONNECTOR_H

#include <QTcpSocket>
#include <QTime>

class ControlUnitConnector : public QObject
{
    Q_OBJECT
private:
    QTcpSocket* socket;
    QByteArray name;

public:
    ControlUnitConnector(QTcpSocket* socket, QByteArray name);

    ControlUnitConnector(const ControlUnitConnector&) = delete;
    ControlUnitConnector& operator=(const ControlUnitConnector&) = delete;
    ControlUnitConnector(ControlUnitConnector&&) = delete;
    ControlUnitConnector& operator=(ControlUnitConnector&&) = delete;

    ~ControlUnitConnector();

public:
    QByteArray getName() const;

public slots:
    void slotSend(QByteArray msg);
    void slotRead();

signals:
    void signalMsgReceived(QByteArray msg);
    void signalDisconnected();
};

#endif // CONTROLUNITCONNECTOR_H
