#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QStringList>
#include <QCoreApplication>

class QTcpServer;
class QNetworkSession;

class Server: public QObject
{
    Q_OBJECT

public:
    Server();

private slots:
    void sessionOpened();
    void sendDatas();

private:
    QTcpServer *tcpServer = NULL;
    QStringList fortunes;
    QNetworkSession *networkSession = NULL;
};

#endif // SERVER_H
