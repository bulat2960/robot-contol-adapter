#ifndef SCENE_H
#define SCENE_H

#include <QTcpServer>
#include <QTcpSocket>

/**
 * This is a mock-object of 3DScene
 * It can only wait for connecting RCA-module and
 * reading its messages.
**/

class Scene : public QTcpServer // Simple tcp server
{
    Q_OBJECT
private:
    // Socket for connect to RCA
    QTcpSocket* socket;
    quint16 port;

    QVector<QString> receivedMessages;
public:
    // Basic constructor
    Scene(quint16 port);
    int messagesCount() const;
public slots:
    // Slots for necessary actions
    void readyRead();
    void startServer();
    void closeServer();
    QString getLastMessage() const;
    bool isRcaConnected() const;
    bool isRcaDisconnected() const;
protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // SCENE_H
