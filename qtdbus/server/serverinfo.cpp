/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -c ServerInfo -a serverinfo.h:serverinfo.cpp test.xml
 *
 * qdbusxml2cpp is Copyright (C) 2017 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "serverinfo.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class ServerInfo
 */

ServerInfo::ServerInfo(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

ServerInfo::~ServerInfo()
{
    // destructor
}

#include <QDebug>

typedef struct{
    int i;
    char s[3];
    double d;
    float f[3];
}test_strct;

int ServerInfo::add(int va, int vb, QString &sum_str)
{
    int sum = va+vb;
    sum_str = QString::number(sum);
    qDebug() << "add: " << va << "+" << vb << "=" << sum;
    return sum;
}

void ServerInfo::print(const QString &value)
{
    qDebug() << "print: " << value;
}

/*
 * Implementation of adaptor class ServerInfo
 */

ServerInfo2::ServerInfo2(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

ServerInfo2::~ServerInfo2()
{
    // destructor
}

QDBusVariant ServerInfo2::transfer(const QDBusVariant &in_data)
{
    test_strct tt;
    //
    memcpy(&tt, in_data.variant().toByteArray().data(), sizeof(tt));
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

