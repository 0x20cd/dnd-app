#ifndef ABSTRACTHASHABLE_H
#define ABSTRACTHASHABLE_H

#include <QByteArray>
#include <QJsonObject>
#include <QJsonArray>

class AbstractHashable
{
public:
    //AbstractHashable();
    virtual QByteArray hash() = 0;
    static QByteArray hashBytes(QByteArray bytes);
    static QByteArray hashJsonIntObject(QJsonObject obj);
};

#endif // ABSTRACTHASHABLE_H
