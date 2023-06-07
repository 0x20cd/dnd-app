#ifndef DNDRANDOM_H
#define DNDRANDOM_H

#include "entitydata.h"
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QtQml/qqmlregistration.h>
#include <QtAlgorithms>
#include <QRandomGenerator>
#include <algorithm>
#include "abstracthashable.h"
#include "randommodel.h"

class DndRandom : public EntityData, public virtual AbstractHashable
{
    Q_OBJECT
    QML_ELEMENT
    //Q_PROPERTY(QJsonArray dist READ dist WRITE setDist NOTIFY distChanged)
    Q_PROPERTY(RandomModel* dist READ dist CONSTANT)
public:
    explicit DndRandom(QObject *parent = nullptr);
    RandomModel* dist();
    //void setDist(QJsonArray dist);
    Q_INVOKABLE int rand() const;
    virtual QByteArray hash();
private:
    RandomModel m_dist;
};

#endif // DNDRANDOM_H
