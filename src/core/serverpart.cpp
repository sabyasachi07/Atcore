#include <QLoggingCategory>

#include "serverpart.h"

Q_LOGGING_CATEGORY(ATCORE_SERVER, "org.kde.atelier.core.server")

ServerPart::ServerPart(QObject *parent):QObject(parent)
{
      m_tcpServer=new QTcpServer(this);
}

void ServerPart::startserver()
{
     if (!m_tcpServer->listen(QHostAddress::Any, 37369)){
      qCDebug(ATCORE_SERVER) <<   (tr("Error! The port is taken by some other service."));
      return;
      }
      connect(m_tcpServer, &QTcpServer::newConnection, this, &ServerPart::newConnection);
      qCDebug(ATCORE_SERVER) << (tr("Server started, port is openned."));
}

void ServerPart::newConnection()
{
        QTcpSocket *m_clientSocket = m_tcpServer->nextPendingConnection();

       connect(m_clientSocket, &QTcpSocket::readyRead, this, [this,m_clientSocket](){readClient(m_clientSocket);});
       connect(m_clientSocket, &QTcpSocket::disconnected, this, [this,m_clientSocket](){disconnect(m_clientSocket);});
       qCDebug(ATCORE_SERVER) << tr("connection established");
}

void ServerPart::readClient(QTcpSocket *clientSocket)
{
    //here socket willl read from the client
      QDataStream in(m_clientSocket);
       for (;;){
           if(!m_nNextBlockSize){
                if (m_clientSocket->bytesAvailable() <  static_cast<qint64>(sizeof(quint16))) { break; }
            in >> m_nNextBlockSize;
            }
            if (m_clientSocket->bytesAvailable() < m_nNextBlockSize) { break; }
            QString str;
            in >> str;
            emit gotNewCommand(str);
            m_nNextBlockSize = 0;
       }
}

void ServerPart::disconnect(QTcpSocket* clientSocket)
{
       //here it will be disconnected
       qCDebug(ATCORE_SERVER) <<tr("disconnect");
       m_clientSocket->deleteLater();
}

