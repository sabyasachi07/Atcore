#include "serverconfig.h"


ServerConfig::ServerConfig(QObject *parent):QObject(parent)
{

}
ServerConfig::~ServerConfig()
{

}
QString ServerConfig::ServerPrivateKey()
{
   m_ServerPrivateKey = QStringLiteral(SERVER_KEY_PATH);
   return m_ServerPrivateKey;
}
QString ServerConfig::ServerPublicKey()
{

    m_ServerPublicKey = QStringLiteral(SERVER_CRT_PATH);
    return m_ServerPublicKey;

}
QString ServerConfig::ClientPublicKey()
{
    m_ClientPublicKey = QStringLiteral(CLIENT_CRT_PATH);
    return m_ClientPublicKey;
}

uint16_t ServerConfig::getPort()
{
    m_port = 14552;
    return m_port;

}
