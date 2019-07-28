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
    // 如果需要,服务器可以接收自己发出的信号
//    QDBusConnection::sessionBus().connect(
//                "org.wx.test",
//                "/org/wx/test/interface1",
//                "org.wx.test.interface1",
//                "boradcast",
//                this,
//                SLOT(boradcast_get(QString)));
}

ServerInfo::~ServerInfo()
{
}

void ServerInfo::boradcast_get(QString data)
{
    qDebug() << "boradcast_get: " << data;
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
    qDebug() << "transfer: "
             << "i=" << tt.i << " "
             << "s=" << tt.s << " "
             << "d=" << tt.d << " "
             << "f=" << tt.f[0] << "/" << tt.f[1] << "/" << tt.f[2];
    //
    tt.i = 10010;
    tt.s[0] = '1';tt.s[1] = '2';tt.s[2] = '3';
    tt.d = 1.23456;
    tt.f[0] = 1.11;tt.f[1] = 2.22;tt.f[2] = 3.33;
    QByteArray baIn;
    baIn.resize(sizeof(tt));
    memcpy(baIn.data(), &tt, sizeof(tt));
    //
    return QDBusVariant(baIn);
}

