#ifndef CSERVER_H
#define CSERVER_H

#include <QObject>
#include <QtNetwork>
#include <QFile>
#include <QTextSTream>
#include <Windows.h>

class CServer : public QObject
{
    Q_OBJECT
public:
    explicit CServer(QObject *parent = nullptr);

    ~CServer();

    void RunServer();

    bool mpTryToConnectToServer();

private slots:
    void serverNewConnectionHandler();

    void socketReadyReadHandler();

private:
    QLocalServer *m_server;
    QFile *outFile{nullptr};
    QTextStream *ts{nullptr};
};

#endif // CSERVER_H
