#ifndef CONTROLUNITCONNECTOR_H
#define CONTROLUNITCONNECTOR_H

#include <QTcpSocket>

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

public slots:
    void slotSend(QByteArray msg);
    void slotRead();

signals:
    void signalMsgReceived(QByteArray msg);
    void signalUnitClosed();
};

#endif // CONTROLUNITCONNECTOR_H
