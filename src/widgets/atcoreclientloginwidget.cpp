#include <atcoreclientloginwidget.h>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QMessageBox>
#include <QCompleter>
#include "machineinfo.h"
#include "atcorenetworkclientconfig.h"

AtCoreClientLoginWidget::AtCoreClientLoginWidget(QWidget *parent):QWidget(parent)
{

    auto newHLayout = new QHBoxLayout();

    auto loginwidgetlayout = new QVBoxLayout();
    auto boxLayout = new QVBoxLayout();


    auto newLabel = new QLabel(tr("Profile"));
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
        }
    });
    connect(ComboUserName ,static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated),[this](const QString &text){ RemoteUserLoginEditing(text); });
    newHLayout->addWidget(newLabel);
    newHLayout->addWidget(ComboUserName);
    loginwidgetlayout->addLayout(newHLayout);

    newHLayout = new QHBoxLayout();
    newLabel = new QLabel(tr("Username"));
    lineName = new QLineEdit();
    connect(lineName, &QLineEdit::editingFinished, this , [this] {

         QString name = lineName->text();
        AtcoreNetworkClientConfig::instance()->storeRemoteUserKey(ComboUserName->currentText(), AtcoreNetworkClientConfig::USER::NAME, name);
    });
    newHLayout->addWidget(newLabel);
    newHLayout->addWidget(lineName,75);
    boxLayout->addLayout(newHLayout);


    newHLayout = new QHBoxLayout();
    newLabel = new QLabel(tr("Password"));
    LineUserPassword = new QLineEdit();
    LineUserPassword->setEchoMode(QLineEdit::Password);
    connect(LineUserPassword, &QLineEdit::editingFinished, this , [this] { AtcoreNetworkClientConfig::instance()->storeRemoteUserKey(ComboUserName->currentText(), AtcoreNetworkClientConfig::USER::PASSWORD, LineUserPassword->text()); });


    newHLayout->addWidget(newLabel);
    newHLayout->addWidget(LineUserPassword,75);
    boxLayout->addLayout(newHLayout);


    newHLayout = new QHBoxLayout();
    newLabel = new QLabel(tr("Host Name"));
    LineHostName = new QLineEdit();

    connect(LineHostName, &QLineEdit::editingFinished, this , [this] {

        QString  hostAddressValue = LineHostName->text();
        AtcoreNetworkClientConfig::instance()->storeRemoteUserKey(ComboUserName->currentText(), AtcoreNetworkClientConfig::USER::HOSTADDRESS, hostAddressValue);
    });

    newHLayout->addWidget(newLabel);
    newHLayout->addWidget(LineHostName,75);
    boxLayout->addLayout(newHLayout);

    newHLayout = new QHBoxLayout();
    newLabel = new QLabel(tr("Port"));
    LinePort = new QLineEdit();
    LinePort->setValidator(new QIntValidator(22,32767,this));
    connect(LinePort, &QLineEdit::editingFinished, this , [this] {

        qint16 portValue = LinePort->text().toUShort();
        AtcoreNetworkClientConfig::instance()->storeRemoteUserKey(ComboUserName->currentText(), AtcoreNetworkClientConfig::USER::PORT, portValue);

    });

    newHLayout->addWidget(newLabel);
    newHLayout->addWidget(LinePort,75);
    boxLayout->addLayout(newHLayout);


    newHLayout = new QHBoxLayout();
    LogInbutton = new QPushButton(tr("Log In"));
    newHLayout->addWidget(LogInbutton);


    connect(LogInbutton, &QPushButton::clicked,this, [this]{
        if(LineHostName->text().isEmpty()||LinePort->text().isEmpty()||LineUserPassword->text().isEmpty()){
            qDebug()<< tr("fill first");
            LogInbutton->setEnabled(false);
        }
        else{

            groupBox->hide();
            sendInfo();
        }

    });

    groupBox = new QGroupBox(tr("Profile"));
    groupBox->setLayout(boxLayout);
    loginwidgetlayout->addWidget(groupBox);
    loginwidgetlayout->addLayout(newHLayout);

    setLayout(loginwidgetlayout);
    loadProfile(ComboUserName->currentText());


}

void AtCoreClientLoginWidget::RemoteUserLoginEditing(const QString &text)
{
    if (AtcoreNetworkClientConfig::instance()->UserName().contains(text)) {
        loadProfile(text);
        return;
    }
    QMap< AtcoreNetworkClientConfig::USER, QVariant> newRemoteuserProfile = {{AtcoreNetworkClientConfig::USER::PROFILE, ComboUserName->currentText()},

                                                                             {AtcoreNetworkClientConfig::USER::NAME , lineName->text()},

                                                                             {AtcoreNetworkClientConfig::USER::PASSWORD , LineUserPassword->text()},

                                                                             {AtcoreNetworkClientConfig::USER::HOSTADDRESS , LineHostName->text()},

                                                                             {AtcoreNetworkClientConfig::USER::PORT , LinePort->text()}};


    AtcoreNetworkClientConfig::instance()->StoreRemoteUserInfo(newRemoteuserProfile);

    loadProfile(newRemoteuserProfile[AtcoreNetworkClientConfig::USER::PROFILE].toString());

}

void AtCoreClientLoginWidget::loadProfile(const QString &profileName)
{
    if (profileName.isEmpty()) {
        return;
    }


    blockSignals(true);
    lineName->setText(AtcoreNetworkClientConfig::instance()->readRemoteUserKey(profileName, AtcoreNetworkClientConfig::USER::NAME).toString());
    LineUserPassword->setText(AtcoreNetworkClientConfig::instance()->readRemoteUserKey(profileName, AtcoreNetworkClientConfig::USER::PASSWORD).toString());
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







