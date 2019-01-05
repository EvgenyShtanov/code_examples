#ifndef SIMPLETCPSERVER_H
#define SIMPLETCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>

class SimpleTCPserver : public QObject
{
    Q_OBJECT
public:
    SimpleTCPserver(quint16 port);
    ~SimpleTCPserver();
    void setMaxClients(quint32 maxCount);
    void startServer();
    void sendToClient(QTcpSocket * pSocket, const QString& str);

public slots:
    virtual void slotNewConnection();
    void slotReadClient();
    void slotDisconnected();
    void slotCloseServer();

private:
    QTcpServer         * m_server;
    QList<QTcpSocket*>   m_clients_list;
    quint16              m_port;
    quint32              m_maxClients;
    QByteArray      m_readData;
    qint32          m_dataLen;
};

#endif // SIMPLETCPSERVER_H
