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

ServerPart::ServerPart(AtCore *core, QObject *parent):QObject(parent),m_nNextBlockSize(0), m_core(core)
{



}

void ServerPart::closeConnection()
{
   if(server->isListening())
   {
      disconnect(server, &QTcpServer::newConnection , this , &ServerPart::link);
       server->close();

   }
   qCDebug(ATCORE_SERVER) << "Server stopped, port is closed";
}


void ServerPart::link()
{


   QTcpSocket *clientSocket = server->nextPendingConnection();
   connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
   connect(clientSocket, &QTcpSocket::readyRead, this, [this,clientSocket](){readClient(clientSocket);});
   connect(clientSocket, &QTcpSocket::disconnected, this, [this,clientSocket](){disconnectfromClient(clientSocket);});
   qCDebug(ATCORE_SERVER) << "connection established";
   client = clientSocket;


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
        m_nNextBlockSize=0;
        //qCDebug(ATCORE_SERVER) << str;
        emit gotNewCommand(str);



   }
}

void ServerPart::disconnectfromClient(QTcpSocket* m_clientSocket)
{
   //here it will be disconnect
   qCDebug(ATCORE_SERVER) << "client disconnected";
   m_clientSocket->deleteLater();
}

void ServerPart::sendToClient(QTcpSocket* socket, const QString& str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << uint16_t(0) << str;

    out.device()->seek(0);
    out << uint16_t(arrBlock.size() - sizeof(uint16_t));

    socket->write(arrBlock);
}

void ServerPart::readyToreceiveCommand()
{

    server = new QTcpServer(this);



    //push the command to AtCore receive by client
   connect(this , &ServerPart::gotNewCommand , m_core ,&AtCore::pushCommand);

   //receiveing the commands from AtCore

   connect(m_core, &AtCore::atcoreMessage, this , [this](const QString &msg){
       sendToClient(client , msg);
 });

   connect(m_core, &AtCore::receivedMessage, this, [this](const QByteArray &message){

      const QString command = QString::fromUtf8(message);
      sendToClient(client , command);
 });
   connect(m_core, &AtCore::pushedCommand, this, [this](const QByteArray &message){

       const QString command = QString::fromUtf8(message);
       sendToClient(client , command);
  });

   //server will detect the port
   connect(m_core, &AtCore::portsChanged, this, [this](const QStringList &ports){
       sendToClient(client , ports.first());
   });


   /*connect(m_core, &AtCore::sdCardFileListChanged, this ,  [this](const QString &filelist){
       sendToClient(client , filelist);
});*/

}

void ServerPart::connectRemotePb()
{


}

