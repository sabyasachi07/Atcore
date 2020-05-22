#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QObject>

class  ServerPart :public QObject
{
    Q_OBJECT

public:
  ServerPart(QObject *parent = nullptr);
  QTcpServer *m_tcpServer;


signals :
    void gotNewCommand(QString str);

public slots:
    void startserver();
    void newConnection();
    void readClient(QTcpSocket* m_clientSocket);
    void disconnect(QTcpSocket* m_clientSocket);

private:
    quint16 m_nNextBlockSize;
};

