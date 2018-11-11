#ifndef PLANNER_H
#define PLANNER_H

#include <QTcpSocket>
#include <QtWidgets>

class Planner : public QWidget
{
    Q_OBJECT
private:
    QTcpSocket* socket;
    QPushButton* connectButton;
    QPushButton* disconnectButton;

    QTextEdit* textEdit;
    QPushButton* sendMsgButton;

    QByteArray name;
public:
    Planner(QString name);
public slots:
    void connectToServer();
    void sendName();
    void sendMsg();
    void readyRead();
    void disconnectFromServer();
};

#endif // PLANNER_H
