#include "entitymodel.h"
#include <algorithm>

EntityModel::EntityModel(QObject *parent)
    : IModel(true, true)
    , QAbstractListModel{parent}
{
    m_idMap.insert(QString(), nullptr);
}

QByteArray EntityModel::hash()
{
    QList<QByteArray> hashes;

    for(const QString &s : m_ids)
        hashes.append(m_idMap[s]->hash());

    std::sort(hashes.begin(), hashes.end()); // order of elements is not important

    QByteArray bytes;
    for (const QByteArray &h : hashes)
        bytes += h;

    return AbstractHashable::hashBytes(bytes);
}

int EntityModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_ids.size();
}

QVariant EntityModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case PtrRole:
        return QVariant::fromValue(m_idMap[m_ids.at(index.row())]);
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> EntityModel::roleNames() const
{
    static const QHash<int, QByteArray> ROLE_NAMES{{PtrRole, "ptr"}};
    return ROLE_NAMES;
}

bool EntityModel::insertRows(int row, int count, const QModelIndex &parent)
{
    this->beginInsertRows(parent, row, row + count - 1);
    m_ids.insert(row, count, QString());
    this->endInsertRows();
    return true;
}

bool EntityModel::append(QObject* ptr)
{
    beforeAppend(ptr);
    EntityData *entity = static_cast<EntityData*>(ptr);

    qDebug() << entity->id();//this->rowCount();

    if (m_idMap.contains(entity->id()))
        return false;

    if (m_takeOwnership) {
        ptr->setParent(this);
        //qDebug() << "APPEND" << this << ptr << ptr->parent();
    } else if (m_manageCounter) {
        static_cast<EntityData*>(ptr)->refCounter++;
    }

    int startRow = m_ids.size();
    auto startIndex = this->index(startRow, 0);

    this->insertRows(startRow, 1);

    m_ids.back() = entity->id();
    //QObject::connect(entity, &EntityData::idChangedFrom, this, &EntityModel::updateId);
    m_idMap.insert(entity->id(), entity);

    emit dataChanged(startIndex, startIndex);
    return true;
}

bool EntityModel::removeRows(int row, int count, const QModelIndex &parent)
{
    this->beginRemoveRows(parent, row, row + count - 1);

    for (int i = row, c = 0; c < count; ++i, ++c) {
        QString curr_id = m_ids[i];
        if (!curr_id.isEmpty()) {
            //QObject::disconnect(m_idMap[curr_id], nullptr, this, nullptr);
            m_idMap.remove(curr_id);
            // delete ?
        }
    }
    m_ids.remove(row, count);

    this->endRemoveRows();
    return true;
}

bool EntityModel::remove(qsizetype i)
{
    QObject *ptr = m_idMap[m_ids[i]];
    EntityData *e = static_cast<EntityData*>(ptr);

    if (m_manageCounter) {
        if (m_takeOwnership && e->refCounter > 0)
            return false;

        if (!m_takeOwnership)
            e->refCounter--;
    }

    beforeRemove(ptr);

    if (m_takeOwnership) {
        delete ptr;
    }

    return this->removeRows(i, 1);
}

bool EntityModel::removeById(QString id)
{
    if (!m_idMap.contains(id))
        return false;

    auto it = std::find(m_ids.begin(), m_ids.end(), id);
    qDebug() << it - m_ids.begin();
    return this->remove(it - m_ids.begin());
}

void EntityModel::clear()
{
    this->beginResetModel();
    m_ids.clear();
    m_idMap.clear();
    m_idMap.insert(QString(), nullptr);
    this->endResetModel();
}

EntityData* EntityModel::getById(QString id)
{
    return m_idMap.value(id, nullptr);
}

int EntityModel::getIndex(EntityData* ptr)
{
    if (!ptr)
        return -1;

    auto it = std::find(m_ids.begin(), m_ids.end(), ptr->id());

    if (it == m_ids.end())
        return -1;
    return it - m_ids.begin();
}

/*void EntityModel::updateId(QString oldId)
{
    EntityData *ptr = m_idMap[oldId];

    auto it = std::find(m_ids.begin(), m_ids.end(), oldId);
    *it = ptr->id();

    m_idMap.remove(oldId);
    m_idMap.insert(ptr->id(), ptr);
}*/
