#ifndef SIMPLETCPCLIENT_H
#define SIMPLETCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>


class SimpleTCPclient : public QObject
{
    Q_OBJECT
public:
    explicit SimpleTCPclient(QString hostAddr, quint16 port, QObject *parent = nullptr);
    virtual ~SimpleTCPclient();
    void start();
    void sendToServer(QByteArray * arr);

signals:
    void clientDisconnected();

public slots:
    void slotReadyRead();
    void slotConnected();
    void slotError(QAbstractSocket::SocketError err);
    void slotDisconnected();

private:
    QTcpSocket    * m_socket;
    QString         m_hostAddr;
    quint16         m_port;
    quint64         m_stampId;
    QByteArray      m_readData;
    qint32          m_dataLen;
};

#endif // SIMPLETCPCLIENT_H
