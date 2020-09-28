#include "atcorenetworkclientconfig.h"
#include <QCoreApplication>
#include <QDebug>

const QMap<AtcoreNetworkClientConfig::USER, AtcoreNetworkClientConfig::userKeyInfo> AtcoreNetworkClientConfig::clientDecoderMap = {{USER::PROFILE, {QStringLiteral("profilename"), QStringLiteral("name")}},
                                                                                                                                          {USER::NAME, {QStringLiteral("username"), QString()}},
                                                                                                                                          {USER::PASSWORD, {QStringLiteral("Password"), QString()}},
                                                                                                                                          {USER::HOSTADDRESS, {QStringLiteral("Hostaddress"), QStringLiteral("0.0.0.0")}},
                                                                                                                                          {USER::PORT, {QStringLiteral("Port"), 38917}}};



AtcoreNetworkClientConfig *AtcoreNetworkClientConfig::instance()
{
   static AtcoreNetworkClientConfig a;
   return &a;

}

AtcoreNetworkClientConfig::AtcoreNetworkClientConfig(QObject *parent):QObject(parent),m_AtCoreNetworkSettings(new QSettings(QSettings::IniFormat, QSettings::UserScope, QStringLiteral("remoteAtcore"), QStringLiteral("remoteHostprofiles"), this))
{

}

bool AtcoreNetworkClientConfig::storeRemoteUserKey(const QString &remoteUserName, const AtcoreNetworkClientConfig::USER key, const QVariant &value) const
{

    if (readRemoteUserKey(remoteUserName, key) == value) {
        return false;
    }

    m_AtCoreNetworkSettings->beginGroup(remoteUserName);
    m_AtCoreNetworkSettings->setValue(clientDecoderMap[key].name, value);



    m_AtCoreNetworkSettings->endGroup();

    m_AtCoreNetworkSettings->sync();
    return true;

}

QVariant AtcoreNetworkClientConfig::readRemoteUserKey(const QString &profileName, AtcoreNetworkClientConfig::USER key) const
{
    if (profileName.isEmpty()) {
        return clientDecoderMap[key].defaultValue;
   }

    if (key == USER::PROFILE) {
        return profileName;
    }



    m_AtCoreNetworkSettings->sync();


    return m_AtCoreNetworkSettings->value(QStringLiteral("%1/%2").arg(profileName, clientDecoderMap[key].name), clientDecoderMap[key].defaultValue);


}

void AtcoreNetworkClientConfig::StoreRemoteUserInfo(const QMap<AtcoreNetworkClientConfig::USER, QVariant> &profile)
{


    m_AtCoreNetworkSettings->beginGroup(profile[USER::PROFILE].toString());



   for (int i = 0; i < clientDecoderMap.size(); i++) {
       m_AtCoreNetworkSettings->setValue(clientDecoderMap[AtcoreNetworkClientConfig::USER(i)].name, profile[AtcoreNetworkClientConfig::USER(i)]);

   }
   m_AtCoreNetworkSettings->endGroup();
   m_AtCoreNetworkSettings->sync();

}

void AtcoreNetworkClientConfig::storeProfile(const QVariantMap &profile) const
{
     m_AtCoreNetworkSettings->beginGroup(profile.first().toString());
     for (auto it = profile.begin(), end = profile.end(); it != end; ++it) {
         m_AtCoreNetworkSettings->setValue(it.key(), it.value());
     }

     m_AtCoreNetworkSettings->endGroup();
     m_AtCoreNetworkSettings->sync();

}



QStringList AtcoreNetworkClientConfig::UserName()
{
 m_AtCoreNetworkSettings->sync();
 return m_AtCoreNetworkSettings->childGroups();

}





