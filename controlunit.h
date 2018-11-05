#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H

#include <QTcpSocket>
#include <QtWidgets>

class ControlUnit : public QWidget
{
    Q_OBJECT
private:
    QTcpSocket* socket;
    QPushButton* b;

    QByteArray name;
public:
    ControlUnit(QString name);
public slots:
    void connectToServer();
    void readyRead();
    void deleteSocket();
};

#endif // CONTROLUNIT_H
