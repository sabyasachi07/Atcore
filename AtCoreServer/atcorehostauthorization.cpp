#include <atcorehostauthorization.h>
#include "atcorenetworkclientconfig.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(ATCORE_HOST_AUTHORIZATION, "org.kde.atelier.core.host.authoriztion")


AtCoreHostAuthorization::AtCoreHostAuthorization(QObject *parent):QObject(parent)
{
    m_server = new ServerPart(core,this);
    connect(m_server, &ServerPart::gotNewCommand, this, &AtCoreHostAuthorization::gotPassword);
    startserver();
}

void AtCoreHostAuthorization::startserver()
{

    if (!m_server->server->listen(QHostAddress::Any, AtcoreNetworkClientConfig::instance()->readRemoteUserKey(AtcoreNetworkClientConfig::instance()->getCurrentUsername(), AtcoreNetworkClientConfig::USER::PORT).toInt() )){
        qCDebug(ATCORE_HOST_AUTHORIZATION) << "Unable to start the TCP server";
      } else {
      connect(m_server->server, &QTcpServer::newConnection , m_server , &ServerPart::link);
       qCDebug(ATCORE_HOST_AUTHORIZATION) << "Server started, Port is opened";
    }
}

void AtCoreHostAuthorization::gotPassword(const QString &password)
{

    m_password = password;

    if(AtcoreNetworkClientConfig::instance()->readRemoteUserKey(AtcoreNetworkClientConfig::instance()->getCurrentUsername(), AtcoreNetworkClientConfig::USER::PASSWORD).toString().isEmpty() )
    {
       AtcoreNetworkClientConfig::instance()->storeRemoteUserKey(AtcoreNetworkClientConfig::instance()->getCurrentUsername(), AtcoreNetworkClientConfig::USER::PASSWORD, m_password);
       qCDebug(ATCORE_HOST_AUTHORIZATION) << "Login Succeded";
       m_server->readyToreceiveCommand();
       return;
    }
    if(checkPassword()){
        qCDebug(ATCORE_HOST_AUTHORIZATION) << "Login Succeded";
        m_server->readyToreceiveCommand();

    }
   else{
     qCDebug(ATCORE_HOST_AUTHORIZATION) << "Login Failed";
     m_server->closeConnection();
    //m_server->sendToClient(m_server->client, tr("Incorrect Password"));
   }
}
bool AtCoreHostAuthorization::checkPassword()
{
    if(AtcoreNetworkClientConfig::instance()->readRemoteUserKey(AtcoreNetworkClientConfig::instance()->getCurrentUsername(), AtcoreNetworkClientConfig::USER::PASSWORD).toString().contains(m_password) )
    {
        return true;
    }


    return false;
}
