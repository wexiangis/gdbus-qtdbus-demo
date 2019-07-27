#include <QCoreApplication>
#include <QDebug>
#include "info.h"

void timeout_handler()
{
    while(true)
    {
        QThread::msleep(1000);

        //
        QDBusMessage msg = QDBusMessage::createSignal(
                    QString(),
                    "org.wx.test.interface1",
                    "signalTest");
        msg << QString("here is server\n");
        QDBusConnection::sessionBus().send(msg);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Info* info = new Info;
    new ServerInfo(info);
    new ServerInfo2(info);

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerService("org.wx.test");
    connection.registerObject(QString(), info);

    std::thread th1(timeout_handler);
    th1.detach();

    return a.exec();
}
