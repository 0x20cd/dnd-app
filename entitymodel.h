#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QHash>
#include <QtQml/qqmlregistration.h>
#include "entitydata.h"
#include "imodel.h"
#include "abstracthashable.h"

class EntityModel : public QAbstractListModel, public virtual IModel, public virtual AbstractHashable
{
    Q_OBJECT
    QML_ELEMENT
public:
    enum Roles {
        PtrRole = Qt::UserRole
    };

    explicit EntityModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Q_INVOKABLE virtual bool append(QObject* ptr);
    Q_INVOKABLE virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Q_INVOKABLE bool removeById(QString id);
    bool remove(qsizetype i);
    virtual void clear();
    virtual QByteArray hash();
    EntityData* getById(QString id);

    Q_INVOKABLE int getIndex(EntityData* ptr);

protected:
    QHash<QString, EntityData*> m_idMap;
    QList<QString> m_ids;

//private slots:
//    void updateId(QString oldId);
};

#endif // ENTITYMODEL_H
