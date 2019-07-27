#include "info.h"

Info::Info()
{

}

#include <QDebug>

typedef struct{
    int i;
    char s[3];
    double d;
    float f[3];
}test_strct;

int Info::add(int va, int vb, QString &sum_str)
{
    int sum = va+vb;
    sum_str = QString::number(sum);
    return sum;
}

void Info::print(const QString &value)
{
    qDebug() << "print: " << value;
}

QDBusVariant Info::transfer(const QDBusVariant &in_data)
{
    test_strct tt;
    //
    memcpy(&tt, ret.variant().toByteArray().data(), sizeof(tt));
    qDebug() << "transfer: \n"
             << "i=" << tt.i << " "
             << "s=" << tt.s << " "
             << "d=" << tt.d << " "
             << "f=" << tt.f[0] << "/" << tt.f[1] << "/" << tt.f[2];
    //
    tt.i = 10086;
    tt.s[0] = 'a';tt.s[1] = 'b';tt.s[2] = 'c';
    tt.d = 3.1415926;
    tt.f[0] = 1.23;tt.f[2] = 4.56;tt.f[3] = 7.89;
    QByteArray baIn;
    baIn.resize(sizeof(tt));
    memcpy(baIn.data(), &tt, sizeof(tt));
    //
    return QDBusVariant(baIn);
}
