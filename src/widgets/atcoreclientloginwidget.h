#pragma once

#include "atcorewidgets_export.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>

#include <QSpinBox>
#include <QWidget>


class ATCOREWIDGETS_EXPORT AtCoreClientLoginWidget: public QWidget
{

    Q_OBJECT

signals:

 void sendInfo();

public:
    AtCoreClientLoginWidget(QWidget *parent = nullptr);

    void RemoteUserLoginEditing(const QString &text);

    void loadProfile(const QString &profileName);



    QString m_password;

 public slots:
   bool isVerified();
   void verifyPassword(bool value);


 private:
    QComboBox *ComboUserName = nullptr;
    QLineEdit *lineName = nullptr;
    QLineEdit *LineUserPassword = nullptr;
    QLineEdit *LineHostName = nullptr;
    QLineEdit *LinePort = nullptr;
    QPushButton *LogInbutton = nullptr;
    QGroupBox *groupBox = nullptr;
    bool m_verified;



};
