#include <QCoreApplication>
#include <QDebug>
#include "clientinfo.h"

typedef struct{
    int i;
    char s[3];
    double d;
    float f[3];
}test_strct;

ClientInfo* itf1 = NULL;
ClientInfo2* itf2 = NULL;

void timeout_handler()
{
    test_strct tt;

    while(true)
    {
        QThread::msleep(1000);

        //-- print --
        itf1->print("client-log-0987bvcx");

        //-- add --
        QString sum_str;
        int sum = itf1->add(567, 234, sum_str);
        qDebug() << "sum: " << sum << " sum_str: " << sum_str;

        //-- transfer --
        tt.i = 10086;
        tt.s[0] = 'a';tt.s[1] = 'b';tt.s[2] = 'c';
        tt.d = 3.1415926;
        tt.f[0] = 1.23;tt.f[2] = 4.56;tt.f[3] = 7.89;
        QByteArray baIn;
        baIn.resize(sizeof(tt));
        memcpy(baIn.data(), &tt, sizeof(tt));
        QDBusVariant ret = itf2->transfer(QDBusVariant(baIn));
        memcpy(&tt, ret.variant().toByteArray().data(), sizeof(tt));
        qDebug() << "i=" << tt.i << " "
                 << "s=" << tt.s << " "
                 << "d=" << tt.d << " "
                 << "f=" << tt.f[0] << "/" << tt.f[1] << "/" << tt.f[2];

        //
//        QDBusMessage msg = QDBusMessage::createSignal(
//                    "/org/wx/test/interface1",
//                    "org.wx.test.interface1",
//                    "boradcast");
//        msg << QString("this is client boradcast !!");
//        QDBusConnection::sessionBus().send(msg);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //必须在main函数初始化,不能放到线程中,否则无法接收信号
    itf1 = new ClientInfo("org.wx.test", "/org/wx/test/interface1", QDBusConnection::sessionBus());
    itf2 = new ClientInfo2("org.wx.test", "/org/wx/test/interface2", QDBusConnection::sessionBus());

    std::thread th1(timeout_handler);
    th1.detach();

    return a.exec();
}
