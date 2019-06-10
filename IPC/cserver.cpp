#include "cserver.h"

CServer::CServer(QObject *parent) : QObject(parent), m_server(nullptr)
{
    m_server = new QLocalServer(this);
    QObject::connect(m_server, SIGNAL(newConnection()), this, SLOT(serverNewConnectionHandler()));
    outFile = new QFile("serverlogger" + (QTime::currentTime()).toString("hh_mm_ss") + ".log", this);
    outFile->open(QIODevice::WriteOnly | QIODevice::Append);
    ts = new QTextStream(outFile);
}

CServer::~CServer()
{
  m_server->close();
  delete m_server;
}
void CServer::RunServer()
{
    qDebug() << "Run Server ok";
    //QLocalServer::removeServer("clickshareclient");
    bool ok = m_server->listen("clickshareclient");
    if (!ok)
    {
             // TODO:
        *ts << "Run Server fail" <<endl;
     }
    else{
            // TODO:
        *ts << "Run Server success" <<endl;
    }
}

bool CServer::mpTryToConnectToServer()
{
    auto pSocket = new QLocalSocket;
    pSocket->connectToServer("clickshareclient");
    if (pSocket->waitForConnected()) {
        *ts << "mpTryToConnectToServer success";
        return true;
    }
    *ts << "[Bean]mpTryToConnectToServer fail "<<pSocket->error()<<endl;
    // Client socket can be discarded.
    delete (pSocket);
    return false;
}
void CServer::serverNewConnectionHandler()
{
    qDebug() << "New Connection";

    QLocalSocket* socket = m_server->nextPendingConnection();
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyReadHandler()));
    QObject::connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
}

void CServer::socketReadyReadHandler()
{
    QLocalSocket* socket = static_cast<QLocalSocket*>(sender());
    if (socket)
    {
        QTextStream stream(socket);
        qDebug() << "Read Data From Client:" << stream.readAll();

        QString response = "Hello Client";
        socket->write(response.toUtf8());
        socket->flush();
    }

    // 返回到客户端的void sendMessage方法，m_socket->waitForReadyRead()之后的操作
}
