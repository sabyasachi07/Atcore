/* AtCore
    Copyright (C) <2020>

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

#include <QDataStream>
#include <QHostAddress>
#include "atcorenetworkclient.h"
#include "atcorenetworkclientconfig.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(ATCORE_CLIENT, "org.kde.atelier.core.client")


AtCoreNetworkClient::AtCoreNetworkClient(const QString hostAddress, int portVal, QObject *parent ):QObject(parent),m_nNextBlockSize(0),m_host(hostAddress),m_port(portVal)
{

}
void AtCoreNetworkClient::connectToHost()
{
    m_client.connectToHost(m_host, m_port);
    if (m_client.waitForConnected(5000)) {

       qCDebug(ATCORE_CLIENT) << "connected to server";
       connected = true;

    } else {
        qCDebug(ATCORE_CLIENT) << "Unable to connect to server";
        connected = false;

        return;
    }
}

bool AtCoreNetworkClient::isConnected()
{
    return connected;
}

//here command to be expected a string, to send it to server and the server will send it tot atcore lib
void AtCoreNetworkClient::sendCommand(const QString &comm)
{


    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << uint16_t(0) << comm;

    out.device()->seek(0);
    out << uint16_t(arrBlock.size() - sizeof(uint16_t));

    m_client.write(arrBlock);



}

void AtCoreNetworkClient::closeConnection()
{

  if(m_client.state()==QAbstractSocket::UnconnectedState) {
       m_client.disconnectFromHost();
  }

  else if (m_client.state()==QAbstractSocket::ConnectedState) {
         m_client.abort();
  }
  else {
      m_client.abort();
}
  m_client.deleteLater();

}

//get commands,messages from server
void AtCoreNetworkClient::read()
{


    QDataStream in(&m_client);
    while (m_client.bytesAvailable())
   {
        if (!m_nNextBlockSize)
        {
            if (m_client.bytesAvailable() < static_cast<qint64>(sizeof(quint16))) { break; }
            in >> m_nNextBlockSize;
        }

        if (m_client.bytesAvailable() < m_nNextBlockSize) { break; }

        QString str;
        in >> str;

        /*if(str == tr("Incorrect Password"))
           {
             connected = false;
             emit wrongPassword(true);
             closeConnection();


           }*/


        m_nNextBlockSize = 0;
    }
}

