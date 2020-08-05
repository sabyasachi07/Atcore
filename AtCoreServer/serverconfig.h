#pragma once


#include <QString>
#include <QObject>
#include "atcore_export.h"


/**
 * A class that stores configurations for Server.
 */
class ATCORE_EXPORT ServerConfig : public QObject
{
    Q_OBJECT

public:
     ServerConfig(QObject *parent = nullptr);
    ~ServerConfig();

    QString ServerPrivateKey();
    QString ServerPublicKey();

    QString ClientPublicKey();

    uint16_t getPort();

private:

    uint16_t m_port;
    QString m_ServerPrivateKey;
    QString m_ServerPublicKey;
    QString m_ClientPublicKey;


};
