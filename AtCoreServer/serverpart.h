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
#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include "atcore_export.h"
#include "atcore.h"


class  ATCORE_EXPORT ServerPart :public QObject
{
    Q_OBJECT

public:
  ServerPart(AtCore *core, QObject *parent = nullptr);
  bool connectionEstablished();
  void closeConnection();
  QTcpSocket *client;
  QTcpServer *server;


signals :
    void gotNewCommand(const QString& str);

public slots:
    void link();
    void readClient(QTcpSocket* m_clientSocket);
    void disconnectfromClient(QTcpSocket* m_clientSocket);
    void sendToClient(QTcpSocket* socket, const QString& str);
    void readyToreceiveCommand();
    void connectRemotePb();

private:
    uint16_t m_nNextBlockSize;
    AtCore *m_core;


};
