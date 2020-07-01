/* AtCore
    Copyright (C) <2016>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
        Patrick José Pereira <patrickjp@kde.org>
        Chris Rizzitello <rizzitello@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QLoggingCategory>
#include <QSslSocket>
#include <QFile>
#include <QDataStream>
#include "serverpart.h"

Q_LOGGING_CATEGORY(ATCORE_SERVER, "org.kde.atelier.core.server")

ServerPart::ServerPart(QObject *parent):QTcpServer(parent),m_nNextBlockSize(0)
{

    QFile keyFile(QStringLiteral("/home/trex108/code/server/server.key"));
    keyFile.open(QIODevice::ReadOnly);
    key = QSslKey(keyFile.readAll(), QSsl::Rsa );
    keyFile.close();



    QFile certFile(QStringLiteral("/home/trex108/code/server/server.crt"));
    certFile.open(QIODevice::ReadOnly);
    cert = QSslCertificate(certFile.readAll());
    certFile.close();


}

void ServerPart::startserver()
{
    
    if (!listen(QHostAddress::Any, 38917)){
        qCDebug(ATCORE_SERVER) << "Unable to start the TCP server";
        status = false;
        exit(0);
    } else {
      connect(this, &ServerPart::newConnection , this , &ServerPart::link);
      status = true;
      qCDebug(ATCORE_SERVER) << "Listening on " << serverAddress() << ":" << serverPort();    
    }
}

void ServerPart::incomingConnection(qintptr sslSocketDescriptor)
{


QSslSocket *sslSocket = new QSslSocket(this);

connect(sslSocket,QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),this,&ServerPart::sslErrors);
sslSocket->setSocketDescriptor(sslSocketDescriptor);
sslSocket->setLocalCertificate(cert);
sslSocket->setPrivateKey(key);
sslSocket->addCaCertificates(QStringLiteral("/home/trex108/code/client/client.crt"));
sslSocket->setPeerVerifyMode(QSslSocket::VerifyPeer);
sslSocket->setProtocol(QSsl::TlsV1SslV3);
sslSocket->startServerEncryption();



addPendingConnection(sslSocket);
}


bool ServerPart::connectionEstablished()
{

    return status;
}

void ServerPart::closeConnection()
{
   if(isListening())
   {
      disconnect(this, &ServerPart::newConnection , this , &ServerPart::link);
       close();

   }
   qCDebug(ATCORE_SERVER) << (tr("Server stopped, port is closed"));
}


void ServerPart::sslErrors(const QList<QSslError> &errors)
{

foreach (const QSslError &error, errors)
qCDebug(ATCORE_SERVER)<< error.errorString();
}

void ServerPart::link()
{


   QTcpSocket *m_clientSocket = nextPendingConnection();

   connect(m_clientSocket, &QTcpSocket::readyRead, this, [this,m_clientSocket](){readClient(m_clientSocket);});
   connect(m_clientSocket, &QTcpSocket::disconnected, this, [this,m_clientSocket](){disconnectfromClient(m_clientSocket);});
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
        //emit gotNewCommand(str);
        qCDebug(ATCORE_SERVER) << str;
        emit gotNewCommand(str);



   }
}

void ServerPart::disconnectfromClient(QTcpSocket* m_clientSocket)
{
   //here it will be disconnected
   qCDebug(ATCORE_SERVER) <<tr("client disconnected");
   m_clientSocket->deleteLater();
}

