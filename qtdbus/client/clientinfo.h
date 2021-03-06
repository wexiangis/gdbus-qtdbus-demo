/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -c ClientInfo -p ./client/clientinfo.h:./client/clientinfo.cpp test.xml
 *
 * qdbusxml2cpp is Copyright (C) 2017 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface org.wx.test.interface1
 */
class ClientInfo: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.wx.test.interface1"; }

public:
    ClientInfo(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = nullptr);

    ~ClientInfo();
public slots:
    void boradcast_get(QString data);

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<int, QString> add(int va, int vb)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(va) << QVariant::fromValue(vb);
        return asyncCallWithArgumentList(QStringLiteral("add"), argumentList);
    }
    inline QDBusReply<int> add(int va, int vb, QString &sum_str)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(va) << QVariant::fromValue(vb);
        QDBusMessage reply = callWithArgumentList(QDBus::Block, QStringLiteral("add"), argumentList);
        if (reply.type() == QDBusMessage::ReplyMessage && reply.arguments().count() == 2) {
            sum_str = qdbus_cast<QString>(reply.arguments().at(1));
        }
        return reply;
    }

    inline QDBusPendingReply<> print(const QString &value)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(value);
        return asyncCallWithArgumentList(QStringLiteral("print"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void boradcast(const QString &data);
};

/*
 * Proxy class for interface org.wx.test.interface2
 */
class ClientInfo2: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.wx.test.interface2"; }

public:
    ClientInfo2(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = nullptr);

    ~ClientInfo2();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<QDBusVariant> transfer(const QDBusVariant &in_data)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in_data);
        return asyncCallWithArgumentList(QStringLiteral("transfer"), argumentList);
    }

Q_SIGNALS: // SIGNALS
};

namespace org {
  namespace wx {
    namespace test {
      typedef ::ClientInfo interface1;
      typedef ::ClientInfo2 interface2;
    }
  }
}
#endif
