#ifndef ITEMSMODEL_H
#define ITEMSMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QList>
#include <utility>
#include "abstracthashable.h"
#include "dnditem.h"

class ItemsModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ItemsModel(QObject *parent = nullptr);
    enum Roles {
        ItemRole = Qt::UserRole,
        QualityRole
    };

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Q_INVOKABLE virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Q_INVOKABLE void append(DndItem* item);
    Q_INVOKABLE void append(DndItem* item, int quality);
    Q_INVOKABLE void remove(int row);
    Q_INVOKABLE int getQuality(DndItem* item);

private:
    QList<std::pair<DndItem*, int>> m_data;
};

#endif // ITEMSMODEL_H
