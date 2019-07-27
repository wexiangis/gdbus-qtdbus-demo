#include <QCoreApplication>
#include <QDebug>
#include "clientinfo.h"

void boradcast(const QString data)
{
    qDebug() << "boradcast: " << data;
}

typedef struct{
    int i;
    char s[3];
    double d;
    float f[3];
}test_strct;

void timeout_handler()
{
    test_strct tt;
    ClientInfo* itf1 = new ClientInfo("org.wx.test", "/org/wx/test/interface1", QDBusConnection::sessionBus());
    ClientInfo2* itf2 = new ClientInfo2("org.wx.test", "/org/wx/test/interface2", QDBusConnection::sessionBus());

    QDBusConnection::sessionBus().connect(
                "org.wx.test",
                "/org/wx/test/interface1",
                "org.wx.test.interface1",
                "boradcast",
                itf1,
                SLOT(boradcast(const QString data)));

    while(true)
    {
        QThread::msleep(1000);

        //-- print --
        itf1->print("here is qt5");

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
//        QDBusMessage msg = QDBusMessage::createSignal("/", "com.vnmp.client.infos", "signalTest");
//        msg << QString("here is client\n");
//        QDBusConnection::sessionBus().send(msg);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::thread th1(timeout_handler);
    th1.detach();

    return a.exec();
}
