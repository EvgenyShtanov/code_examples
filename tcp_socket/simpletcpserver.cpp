#include "simpletcpserver.h"
#include <iostream>

SimpleTCPserver::SimpleTCPserver(quint16 port)
{
    m_port = port;
    m_server = new QTcpServer(this);

    connect(m_server, SIGNAL(newConnection()),
            this, SLOT(slotNewConnection()));

    qDebug() << "SimpleTCPserver - constructor";
}

SimpleTCPserver::~SimpleTCPserver()
{
   // delete m_tableConn;
}

void
SimpleTCPserver::startServer()
{
    if (! m_server->listen(QHostAddress::Any, m_port))
    {
        qDebug() << "Server Error";
        qDebug() << "Unable to start the server: " << m_server->errorString();
        m_server->close();

        return;
    }

    qDebug() << "SimpleTCPserver - startServer";
}

void
SimpleTCPserver::slotNewConnection()
{
    QTcpSocket * clientSocket = m_server->nextPendingConnection();
    clientSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

    connect(clientSocket, SIGNAL(disconnected()),
            this, SLOT(slotDisconnected()));
    connect(clientSocket, SIGNAL(readyRead()),
            this, SLOT(slotReadClient()));

    clientSocket->waitForBytesWritten(0);
    clientSocket->waitForConnected(0);

    qDebug() << "New TCP connection";

    if ((m_maxClients) && (m_clients_list.count() >= int(m_maxClients)) && (clientSocket))
    {
        qCritical() << tr("Connection limit (%1) reached, client rejected").arg(m_maxClients);
        if (clientSocket->isOpen())
            clientSocket->close();
    }

    qDebug() << "SimpleTCPserver - start slotNewConnectionserver";
}

void
SimpleTCPserver::setMaxClients(quint32 maxCount)
{
    m_maxClients = maxCount;
}

void
SimpleTCPserver::slotDisconnected()
{
    QTcpSocket * clientSocket = static_cast<QTcpSocket*>(sender());

    if (! clientSocket)
        return;

    // QString findStr(tr("%1").arg(int(clientSocket)));

    QListIterator<QTcpSocket *> iter(m_clients_list);
    QTcpSocket  *sendClient;
    int i = 0;
    while(iter.hasNext())
    {
        sendClient = iter.next();
        if (sendClient == clientSocket)
        {
            m_clients_list.removeAt(i);
        }
        i++;
    }

    qDebug() << "TCP client disconnected";
}

void
SimpleTCPserver::slotReadClient()
{
    qDebug () << "SimpleTCPserver::slotReadClient";

    QTcpSocket * clientSocket = qobject_cast<QTcpSocket*> (sender());

    if(! clientSocket)
        return;

    m_readData.append(clientSocket->readAll());

    qDebug() << "Server received data: " << m_readData;
    std::cout << m_readData.data();
}

void
SimpleTCPserver::slotCloseServer() {

}
