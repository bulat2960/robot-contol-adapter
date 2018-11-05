#ifndef PLANNER_H
#define PLANNER_H

#include <QTcpSocket>
#include <QtWidgets>

class Planner : public QWidget
{
    Q_OBJECT
private:
    QTcpSocket* socket;
    QPushButton* b;

    QTextEdit* textEdit;
    QPushButton* sendMsgButton;

    QByteArray name;
public:
    Planner(QString name);
public slots:
    void connectToServer();
    void readyRead();
    void deleteSocket();

    void sendMsg();
};

#endif // PLANNER_H
