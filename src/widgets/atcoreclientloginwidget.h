#pragma once

#include "atcorewidgets_export.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>


class ATCOREWIDGETS_EXPORT AtCoreClientLoginWidget: public QWidget
{

    Q_OBJECT

signals:

 void sendInfo();

public:
    AtCoreClientLoginWidget(QWidget *parent = nullptr);

    void RemoteUserLoginEditing();

    void loadProfile(const QString &profileName);



    QString m_password;

 public slots:
   bool isVerified();
   void verifyPassword(bool value);


 private:
    QComboBox *ComboUserName = nullptr;
    QLineEdit *LineUserPassword = nullptr;
    QLineEdit *LineHostName = nullptr;
    QLineEdit *LinePort = nullptr;
    QPushButton *LogInbutton = nullptr;
    bool m_verified;



};
