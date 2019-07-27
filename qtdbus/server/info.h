#ifndef INFO_H
#define INFO_H

#include <QObject>

#include "serverinfo.h"

class Info
{
public:
    Info();
public Q_SLOTS: // METHODS
    int add(int va, int vb, QString &sum_str);
    void print(const QString &value);
    QDBusVariant transfer(const QDBusVariant &in_data);
Q_SIGNALS: // SIGNALS
    void boradcast(const QString &data);
};

#endif // INFO_H
