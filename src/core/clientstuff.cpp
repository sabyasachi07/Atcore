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
#include <QSslKey>
#include <QSslCertificate>
#include <QDataStream>
#include "clientstuff.h"

Q_LOGGING_CATEGORY(ATCORE_CLIENT, "org.kde.atelier.core.client")


ClientStuff::ClientStuff(QObject *parent):QObject(parent)

{
    server = new QSslSocket;

    connect(server, &QSslSocket::disconnected, this, &ClientStuff::serverDisconnect);

    connect(server, &QSslSocket::encryptedBytesWritten,this, &ClientStuff::encryptedBytesWritten);
    connect(server,QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),this,&ClientStuff::sslErrors);
    server->addCaCertificates(QStringLiteral("/home/trex108/code/server/server.crt"));
    server->setPrivateKey(QStringLiteral("/home/trex108/code/client/client.key"));
    server->setLocalCertificate(QStringLiteral("/home/trex108/code/client/client.crt"));
    server->setPeerVerifyMode(QSslSocket::VerifyPeer);
    server->setProtocol(QSsl::TlsV1SslV3);

}
void ClientStuff::connectToHost()
{
    server->connectToHostEncrypted(tr("127.0.0.1"),  38917);
    if (server->waitForEncrypted(5000)) {

       qCDebug(ATCORE_CLIENT) << (tr(" connected to server"));
       status = true;

    } else {
        qCDebug(ATCORE_CLIENT) << (tr("Unable to connect to server"));
        status = false;

        exit(0);
    }
}

bool ClientStuff::getStatus()
{
    return status;
}

void ClientStuff ::encrypted()
{
    qCDebug(ATCORE_CLIENT) << tr("Encrypted") <<server;
    if (!server)
           return;
}

void ClientStuff ::encryptedBytesWritten(qint64 written)
{
    qCDebug(ATCORE_CLIENT) << tr("encryptedBytesWritten") << server << written;
}


void ClientStuff::sslErrors(const QList<QSslError> &errors)
{
    foreach (const QSslError &error, errors)
        qCDebug(ATCORE_CLIENT) << error.errorString();
}

void ClientStuff::serverDisconnect()
{

   qCDebug(ATCORE_CLIENT)<< (tr("Server disconnected"));
    server->deleteLater();
    exit(0);
}



void ClientStuff::sendCommand(const QString &comm)
{


    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16(0) << comm;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    server->write(arrBlock);



}

void ClientStuff::closeConnection()
{
    disconnect(server, &QSslSocket::connected,0,0);


    switch (server->state())
    {
        case 0:
            server->disconnectFromHost();

            break;
        case 2:
            server->abort();
           break;
           default:
            server->abort();
    }

 }

