#include <serverpart.h>
#include <QObject>
#include <atcore.h>

class  ATCORE_EXPORT AtCoreHostAuthorization :public QObject
{
     Q_OBJECT


    public:
    AtCoreHostAuthorization(QObject *parent = nullptr);
    void startserver();
    void gotPassword(const QString &password);
    bool checkPassword();


private:

    ServerPart *m_server;
    QString m_password;
    AtCore *core;

};
