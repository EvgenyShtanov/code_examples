#include <iostream>
#include <QApplication>
#include <QTest>
#include <QByteArray>

#include "simpletcpserver.h"
#include "simpletcpclient.h"

static const quint16 PORT = 2323;


int main(int argc, char *argv[])
{
    QApplication *app = new QApplication(argc, argv);

    SimpleTCPserver *server = new SimpleTCPserver(PORT);

    server->setMaxClients(3);

    server->startServer();

    QTest::qSleep(3);
    std::cout << "Hi!" << std::endl;


    SimpleTCPclient * client = new SimpleTCPclient("127.0.0.1", PORT);

    // QObject::connect(client, SIGNAL(clientDisconnected()), app, SLOT(quit()));

    client->start();

    QByteArray arr = QByteArray("a");

    qDebug() << arr << " is ready to be send.";

    client->sendToServer(&arr);

    QTest::qSleep(3);
    std::cout << "Hi!" << std::endl;

    return app->exec();
}
