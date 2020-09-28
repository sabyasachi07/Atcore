#pragma once

#include <QString>
#include <QObject>
#include <QSettings>
#include <QObject>
#include <QQmlEngine>
#include <QVariant>
#include "atcore_export.h"



/**
 * A class that stores configurations for Server.
 */
class ATCORE_EXPORT AtcoreNetworkClientConfig : public QObject
{
    Q_OBJECT

public:

    explicit AtcoreNetworkClientConfig(QObject *parent = nullptr);

    static AtcoreNetworkClientConfig *instance();


    enum class USER {
         PROFILE = 0,
         NAME ,        //!< User Name
         PASSWORD,        //!< User Password
         HOSTADDRESS,     //!< Remote Host Address
         PORT,            //!< Port Value
     };
     Q_ENUM(USER)

    bool storeRemoteUserKey(const QString &remoteUserName, const AtcoreNetworkClientConfig::USER key, const QVariant &value) const;

    QVariant readRemoteUserKey(const QString &profileName, AtcoreNetworkClientConfig::USER key) const;

    void StoreRemoteUserInfo(const QMap<AtcoreNetworkClientConfig::USER , QVariant> &profile);



    void storeProfile(const QVariantMap &profile) const;

    QStringList UserName();




private:
     AtcoreNetworkClientConfig *operator =(AtcoreNetworkClientConfig &other) = delete;
     AtcoreNetworkClientConfig(const AtcoreNetworkClientConfig &other) = delete;
     ~AtcoreNetworkClientConfig() =default;
     QSettings *m_AtCoreNetworkSettings = nullptr;

    struct userKeyInfo {
         QString name;
         QVariant defaultValue;


    };

   static const QMap<AtcoreNetworkClientConfig::USER, AtcoreNetworkClientConfig::userKeyInfo> clientDecoderMap;
   QString m_UserName;

};
