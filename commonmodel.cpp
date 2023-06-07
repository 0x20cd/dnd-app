#include "commonmodel.h"
#include <algorithm>

CommonModel::CommonModel(QObject *parent)
    : IModel(false, true)
    , QAbstractListModel(parent)
{}

QByteArray CommonModel::hash()
{
    QList<QByteArray> hashes;

    for(QObject *obj : m_data)
        hashes.append(dynamic_cast<AbstractHashable*>(obj)->hash());

    std::sort(hashes.begin(), hashes.end()); // order of elements is not important

    QByteArray bytes;
    for (const QByteArray &h : hashes)
        bytes += h;

    return AbstractHashable::hashBytes(bytes);
}

int CommonModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.size();
}

QVariant CommonModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        qDebug() << "111";
        return QVariant();
    }

    auto *ptr = m_data.at(index.row());

    switch (role) {
    case PtrRole:
        return QVariant::fromValue(ptr);
    default:
        qDebug() << "222";
        return QVariant();
    }
}

QVariant CommonModel::getByIndex(int i)
{
    return this->data(this->index(i), PtrRole);
}

QHash<int, QByteArray> CommonModel::roleNames() const
{
    static const QHash<int, QByteArray> ROLE_NAMES{{PtrRole, "ptr"}};
    return ROLE_NAMES;
}

bool CommonModel::insertRows(int row, int count, const QModelIndex &parent)
{
    this->beginInsertRows(parent, row, row + count - 1);
    m_data.insert(row, count, nullptr);
    this->endInsertRows();
    return true;
}

bool CommonModel::removeRows(int row, int count, const QModelIndex &parent)
{
    this->beginRemoveRows(parent, row, row + count - 1);
    m_data.remove(row, count);
    this->endRemoveRows();
    return true;
}

bool CommonModel::append(QObject* ptr)
{
    if (this->getIndex(ptr) >= 0)
        return false;

    beforeAppend(ptr);

    if (m_takeOwnership)
        ptr->setParent(this);
    else if (m_manageCounter)
        static_cast<EntityData*>(ptr)->refCounter++;

    int startRow = m_data.size();
    auto startIndex = this->index(startRow, 0);
    this->insertRows(startRow, 1);
    m_data.back() = ptr;

    emit dataChanged(startIndex, startIndex);
    return true;
}

int CommonModel::getIndex(QObject* ptr)
{
    auto it = std::find(m_data.begin(), m_data.end(), ptr);
    if (it == m_data.end())
        return -1;
    return it - m_data.begin();
}

bool CommonModel::remove(qsizetype i)
{
    QObject *ptr = m_data[i];
    /*if (m_manageCounter)
        static_cast<EntityData*>(ptr)->refCounter--;

    beforeRemove(ptr);

    if (m_takeOwnership)
        delete ptr;

    this->removeRows(i, 1);*/

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

void CommonModel::clear()
{
    this->beginResetModel();
    m_data.clear();
    this->endResetModel();
}


