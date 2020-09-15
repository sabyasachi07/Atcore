#include <atcoreclientloginwidget.h>
#include "machineinfo.h"
#include "atcorenetworkclientconfig.h"

#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QMessageBox>


AtCoreClientLoginWidget::AtCoreClientLoginWidget(QWidget *parent):QWidget(parent)
{

    auto newHLayout = new QHBoxLayout();

    auto LoginWidgetLayout = new QVBoxLayout();


    auto newLabel = new QLabel(tr("ComboUserName:"));
    ComboUserName = new QComboBox();

    ComboUserName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ComboUserName->addItems(MachineInfo::instance()->profileNames());


       connect(MachineInfo::instance(), &MachineInfo::profilesChanged, this, [this] {
        int index = ComboUserName->currentIndex();
        ComboUserName->clear();
        ComboUserName->addItems(MachineInfo::instance()->profileNames());
        ComboUserName->setCurrentIndex(std::min<int>(index, ComboUserName->count() - 1));
    });

       connect(ComboUserName, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this] {
           if (AtcoreNetworkClientConfig::instance()->UserName().contains(ComboUserName->currentText())) {
               loadProfile(ComboUserName->currentText());
               AtcoreNetworkClientConfig::instance()->setCurrentUserName(ComboUserName->currentText());
           }
       });

    newHLayout->addWidget(newLabel);
    newHLayout->addWidget(ComboUserName);
    LoginWidgetLayout->addLayout(newHLayout);


    newHLayout = new QHBoxLayout();
    newLabel = new QLabel(tr("Password"));
    LineUserPassword = new QLineEdit();
    connect(LineUserPassword, &QLineEdit::editingFinished, this , [this] {
         m_password = LineUserPassword->text();
    });


    newHLayout->addWidget(newLabel);
    newHLayout->addWidget(LineUserPassword,75);
    LoginWidgetLayout->addLayout(newHLayout);


    newHLayout = new QHBoxLayout();
    newLabel = new QLabel(tr("Host Name"));
    LineHostName = new QLineEdit();

    connect(LineHostName, &QLineEdit::editingFinished, this , [this] {

        QString  hostAddressValue = LineHostName->text();
        AtcoreNetworkClientConfig::instance()->storeRemoteUserKey(ComboUserName->currentText(), AtcoreNetworkClientConfig::USER::HOSTADDRESS, hostAddressValue);
    });

    newHLayout->addWidget(newLabel);
    newHLayout->addWidget(LineHostName,75);
    LoginWidgetLayout->addLayout(newHLayout);

    newHLayout = new QHBoxLayout();
    newLabel = new QLabel(tr("Port"));
    LinePort = new QLineEdit();
    LinePort->setValidator(new QIntValidator(22,64345,this));
    connect(LinePort, &QLineEdit::editingFinished, this , [this] {

         quint16  portValue = LineHostName->text().toInt();
         AtcoreNetworkClientConfig::instance()->storeRemoteUserKey(ComboUserName->currentText(), AtcoreNetworkClientConfig::USER::PORT, portValue);

    });

    newHLayout->addWidget(newLabel);
    newHLayout->addWidget(LinePort,75);
    LoginWidgetLayout->addLayout(newHLayout);


    newHLayout = new QHBoxLayout();
    LogInbutton = new QPushButton(tr("Log In"));
    newHLayout->addWidget(LogInbutton);
    LoginWidgetLayout->addLayout(newHLayout);

    connect(LogInbutton, &QPushButton::clicked,this, [this]{
    if(LineHostName->text().isEmpty()||LinePort->text().isEmpty()||LineUserPassword->text().isEmpty()){
      qDebug()<< tr("fill first");
      LogInbutton->setEnabled(false);
     }
    else{

     sendInfo();
    }

    });

    setLayout(LoginWidgetLayout);
    RemoteUserLoginEditing();
    loadProfile(ComboUserName->currentText());
    AtcoreNetworkClientConfig::instance()->setCurrentUserName(ComboUserName->currentText());

}

void AtCoreClientLoginWidget::RemoteUserLoginEditing()
{
    if (MachineInfo::instance()->profileNames().contains(ComboUserName->currentText())) {
        loadProfile(ComboUserName->currentText());


        return;
    }
    QMap< AtcoreNetworkClientConfig::USER, QVariant> newRemoteuserProfile = {{AtcoreNetworkClientConfig::USER::NAME , ComboUserName->currentText()},

    {AtcoreNetworkClientConfig::USER::PASSWORD , LineUserPassword->text()},

    {AtcoreNetworkClientConfig::USER::HOSTADDRESS , LineHostName->text()},

    {AtcoreNetworkClientConfig::USER::PORT , LinePort->text()}};


    AtcoreNetworkClientConfig::instance()->StoreRemoteUserInfo(newRemoteuserProfile);


   loadProfile(newRemoteuserProfile[AtcoreNetworkClientConfig::USER::NAME].toString());

}

void AtCoreClientLoginWidget::loadProfile(const QString &profileName)
{
    if (profileName.isEmpty()) {
        return;
    }


    blockSignals(true);
    LineHostName->setText(AtcoreNetworkClientConfig::instance()->readRemoteUserKey(profileName, AtcoreNetworkClientConfig::USER::HOSTADDRESS).toString());
    LinePort->setText(AtcoreNetworkClientConfig::instance()->readRemoteUserKey(profileName, AtcoreNetworkClientConfig::USER::PORT).toString());
    blockSignals(false);

}

bool AtCoreClientLoginWidget::isVerified()
{
    return m_verified;
}

void AtCoreClientLoginWidget::verifyPassword(bool value)
{
    m_verified = value;
}







