#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include "serverinfo.h"

void timeout_handler()
{
    while(true)
    {
        QThread::msleep(1000);
        //
        QDBusMessage msg = QDBusMessage::createSignal(
                    "/org/wx/test/interface1",
                    "org.wx.test.interface1",
                    "boradcast");
        msg << QString("this is server boradcast !!");
        QDBusConnection::sessionBus().send(msg);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //必须在main函数初始化,不能放到线程中
    QObject* info = new QObject;
    new ServerInfo(info);
    new ServerInfo2(info);

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerService("org.wx.test");
    connection.registerObject("/org/wx/test/interface1", info);
    connection.registerObject("/org/wx/test/interface2", info);

    std::thread th1(timeout_handler);
    th1.detach();

    return a.exec();
}
