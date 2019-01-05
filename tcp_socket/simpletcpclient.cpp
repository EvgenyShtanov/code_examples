#include "simpletcpclient.h"


SimpleTCPclient::SimpleTCPclient(QString hostAddr, quint16 port, QObject *parent) : QObject(parent)
{
    m_port = port;
    m_hostAddr = hostAddr;

    m_socket = new QTcpSocket(this);

    connect(m_socket, SIGNAL(connected()),
            this, SLOT(slotConnected()));
    connect(m_socket, SIGNAL(readyRead()),
            this, SLOT(slotReadyRead()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotError(QAbstractSocket::SocketError)));
    connect(m_socket, SIGNAL(disconnected()),
            this, SLOT(slotDisconnected()));

}

SimpleTCPclient::~SimpleTCPclient() {
    if ((m_socket) && (m_socket->isOpen()))
        m_socket->close();
}

void
SimpleTCPclient::start()
{
    m_socket->connectToHost(m_hostAddr, m_port);
    m_socket->waitForConnected();
}

void
SimpleTCPclient::slotReadyRead()
{
    QTcpSocket * clientSocket = m_socket;

    m_readData.append(clientSocket->readAll());

    qDebug() << m_readData;
}

void
SimpleTCPclient::slotConnected() {
    qDebug() << "SimpleTCPclient - slotConnected";

    m_socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

    m_socket->waitForConnected(0);

    /*struct ucs_app_info_t   appInfo;
    memset((char*) &appInfo, 0x00, sizeof(struct ucs_app_info_t));

    appInfo.version = UNICAN_APPLICATION_VERSION;
    strncpy(appInfo.desc, QString("Unican control center").toStdString().data(), 32);*/

    // sendInfoToServer(appInfo);
}

void
SimpleTCPclient::sendToServer(QByteArray * arr)
{
    if (! arr)
        return;

    qDebug() << *arr << " is almost send." ;

    QByteArray sendData = std::move(*arr);

    m_socket->write(sendData);
    m_socket->waitForBytesWritten(0);
    m_socket->flush();

    qDebug() << sendData << " is send." ;
}

void
SimpleTCPclient::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
            "TCP error: " + (err == QAbstractSocket::HostNotFoundError ?
                             "host not found" :
                             err == QAbstractSocket::RemoteHostClosedError ?
                             "remote host is closed" :
                             err == QAbstractSocket::ConnectionRefusedError ?
                             "connection refused" :
                             QString(m_socket->errorString()));

    // changeStatus(UcdTcpClient::ConnError);
    // changeHwStatus(UcdTcpClient::HwConnUnknown);
    // emit connChanged(m_connState);

    if ((err == QAbstractSocket::HostNotFoundError) ||
        (err == QAbstractSocket::RemoteHostClosedError))
        qCritical() << strError;
}

void
SimpleTCPclient::slotDisconnected() {
    emit clientDisconnected();
    m_socket->deleteLater();
}
