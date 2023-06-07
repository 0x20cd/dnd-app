#ifndef COMMONMODEL_H
#define COMMONMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QHash>
#include <QtQml/qqmlregistration.h>
#include "imodel.h"
#include "abstracthashable.h"
#include "entitydata.h"

class CommonModel : public QAbstractListModel, public virtual IModel, public virtual AbstractHashable
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum Roles {
        PtrRole = Qt::UserRole
    };

    explicit CommonModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Q_INVOKABLE virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Q_INVOKABLE virtual bool append(QObject* ptr);
    Q_INVOKABLE virtual bool remove(qsizetype i);
    Q_INVOKABLE int getIndex(QObject* ptr);
    Q_INVOKABLE QVariant getByIndex(int i);
    virtual void clear();
    virtual QByteArray hash();

protected:
    QList<QObject*> m_data;
};

#endif // COMMONMODEL_H
