#ifndef RANDOMMODEL_H
#define RANDOMMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QList>
#include <QJsonObject>
#include "abstracthashable.h"

class RandomModel : public QAbstractListModel, public AbstractHashable
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit RandomModel(QObject *parent = nullptr);
    enum Roles {
        ValueRole = Qt::UserRole,
        WeightRole
    };

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Q_INVOKABLE virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Q_INVOKABLE void append(int value = 0, int weight = 0);
    Q_INVOKABLE void remove(int row);
    virtual QByteArray hash();
    int rand() const;

private:
    void updateData();
    QList<QJsonObject> m_data;
};

#endif // RANDOMMODEL_H
