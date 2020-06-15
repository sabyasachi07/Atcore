#include <QLoggingCategory>
#include <QSslSocket>
#include <QFile>
#include "serverpart.h"

Q_LOGGING_CATEGORY(ATCORE_SERVER, "org.kde.atelier.core.server")

ServerPart::ServerPart(QObject *parent):QTcpServer(parent),m_nNextBlockSize(0)
{

        QFile keyFile(QLatin1String("../../certificates/red_local.key"));
        keyFile.open(QIODevice::ReadOnly);
        key = QSslKey(keyFile.readAll(), QSsl::Rsa , QSsl::Pem);
        keyFile.close();

        QFile certFile(QLatin1String("../../certificates/red_local.pem"));
        certFile.open(QIODevice::ReadOnly);
        cert = QSslCertificate(certFile.readAll());
        certFile.close();

        if (!listen(QHostAddress(tr("127.0.0.1")), 12345)) { // FQDN in red_local.pem is set to 127.0.0.1.  If you change this, it will not authenticate.
            qCritical() << "Unable to start the TCP server";
            exit(0);
        }
        connect(this, &ServerPart::newConnection , this , &ServerPart::newConnection);

}


void ServerPart::incomingConnection(qintptr sslSocketDescriptor)
{
    QSslSocket *sslSocket = new QSslSocket(this);

    connect(sslSocket,QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),this,&ServerPart::sslErrors);
    sslSocket->setSocketDescriptor(sslSocketDescriptor);
    sslSocket->setLocalCertificate(cert.toText(),QSsl::Pem);
    sslSocket->setPrivateKey(key);
    sslSocket->addCaCertificates(QLatin1String("../../certificates/blue_ca.pem"));
    sslSocket->setPeerVerifyMode(QSslSocket::VerifyNone);
    sslSocket->setProtocol(QSsl::TlsV1SslV3);

    addPendingConnection(sslSocket);
}




/*void ServerPart::startserver()
{
      if (!m_tcpServer->listen(QHostAddress::Any, 38373)){
      qCDebug(ATCORE_SERVER) <<(tr("Error! The port is taken by some other service."));
      return;
      }
      connect(m_tcpServer, &QTcpServer::newConnection, this, &ServerPart::newConnection);
      qCDebug(ATCORE_SERVER) << (tr("Server started, port is openned."));
}*/

void ServerPart::sslErrors(const QList<QSslError> &errors)
{

    foreach (const QSslError &error, errors)
     qCDebug(ATCORE_SERVER) << error.errorString();
}
void ServerPart::newConnection()
{


       QTcpSocket *m_clientSocket = nextPendingConnection();

       connect(m_clientSocket, &QTcpSocket::readyRead, this, [this,m_clientSocket](){readClient(m_clientSocket);});
       connect(m_clientSocket, &QTcpSocket::disconnected, this, [this,m_clientSocket](){disconnect(m_clientSocket);});
       qCDebug(ATCORE_SERVER) << tr("connection established");

}

void ServerPart::readClient(QTcpSocket *m_clientSocket)
{
    //here ssslSocket willl read from the client
      QDataStream in(m_clientSocket);
       for (;;){
           if(!m_nNextBlockSize){
                if (m_clientSocket->bytesAvailable() <  static_cast<qint64>(sizeof(quint16))) { break; }
            in >> m_nNextBlockSize;
            }
            if (m_clientSocket->bytesAvailable() < m_nNextBlockSize) { break; }
            QString str;
            in >> str;
            m_nNextBlockSize = 0;
            qDebug()<< str;
       }
}

void ServerPart::disconnect(QTcpSocket* m_clientSocket)
{
       //here it will be disconnected
       qCDebug(ATCORE_SERVER) <<tr("disconnect");
       m_clientSocket->deleteLater();
}

