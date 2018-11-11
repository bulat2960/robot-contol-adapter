#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H

#include <QTcpSocket>
#include <QtWidgets>

class ControlUnit : public QWidget
{
    Q_OBJECT
private:
    QTcpSocket* socket;
    QPushButton* connectButton;
    QPushButton* disconnectButton;

    QByteArray name;
public:
    ControlUnit(QString name);
public slots:
    void connectToServer();
    void sendName();
    void readyRead();
    void disconnectFromServer();
};

#endif // CONTROLUNIT_H
