#ifndef ENTITYDATA_H
#define ENTITYDATA_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include "abstracthashable.h"

class EntityData : public QObject, public virtual AbstractHashable
{
    Q_OBJECT
    Q_PROPERTY(QString eid READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString display READ display WRITE setDisplay NOTIFY displayChanged)
    Q_PROPERTY(QString about READ about WRITE setAbout NOTIFY aboutChanged)

public:
    explicit EntityData(QObject *parent = nullptr);
    QString id() const;
    QString display() const;
    QString about() const;
    void setId(QString id);
    void setDisplay(QString display);
    void setAbout(QString about);
    virtual QByteArray hash();

    int refCounter;

signals:
    void idChanged();
    void idChangedFrom(QString oldId);
    void displayChanged();
    void aboutChanged();

protected:
    QString m_id, m_display, m_about;
};

#endif // ENTITYDATA_H
