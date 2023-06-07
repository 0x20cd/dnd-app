#include "abstracthashable.h"
#include <QCryptographicHash>
#include <algorithm>

QByteArray AbstractHashable::hashBytes(QByteArray bytes)
{
    return QCryptographicHash::hash(bytes, QCryptographicHash::Md5);
}

QByteArray AbstractHashable::hashJsonIntObject(QJsonObject obj)
{
    QStringList keys = obj.keys();

    std::sort(keys.begin(), keys.end());

    QByteArray bytes;
    for (const QString &k : keys) {
        bytes += hashBytes(k.toUtf8());
        bytes += hashBytes(QByteArray::number(obj[k].toInt()));
    }

    return hashBytes(bytes);
}
