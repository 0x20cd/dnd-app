#ifndef LEVELMODEL_H
#define LEVELMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QList>
#include <QJsonObject>
#include <vector>
#include "abstracthashable.h"

class LevelModel : public QAbstractListModel, public AbstractHashable
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit LevelModel(QObject *parent = nullptr);
    enum Roles {
        XpRole = Qt::UserRole,
        PbRole,
        XpDiffRole,
        PbDiffRole
    };

    struct Lvl {
        int xpDiff, pbDiff, xp, pb;
    };

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Q_INVOKABLE virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Q_INVOKABLE void insert(int row = -1, int xpDiff = 1, int pbDiff = 0);
    Q_INVOKABLE void remove(int row);
    Q_INVOKABLE void clear();
    virtual QByteArray hash();
    QJsonObject getLvlByXp(int xp); // {"level" : 0, "pb" : 0}

private:
    void updateData();
    std::vector<Lvl> m_data;
};

#endif // LEVELMODEL_H
