#include "itemsmodel.h"
#include <algorithm>
#include <QRandomGenerator>

ItemsModel::ItemsModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

int ItemsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.size();
}

QVariant ItemsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto &ref = m_data.at(index.row());

    switch (role) {
    case ItemRole:
        return QVariant::fromValue(ref.first);
    case QualityRole:
        return QVariant::fromValue(ref.second);
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> ItemsModel::roleNames() const
{
    static const QHash<int, QByteArray> ROLE_NAMES{
        {ItemRole, "item"},
        {QualityRole, "quality"}
    };
    return ROLE_NAMES;
}

bool ItemsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    this->beginInsertRows(parent, row, row + count - 1);
    m_data.insert(row, count, std::pair<DndItem*, int>(nullptr, 0));
    this->endInsertRows();
    return true;
}

bool ItemsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    this->beginRemoveRows(parent, row, row + count - 1);
    m_data.remove(row, count);
    this->endRemoveRows();
    return true;
}

void ItemsModel::append(DndItem* item, int quality)
{
    insertRows(this->rowCount(), 1);
    auto &ref = m_data.back();
    ref.first = item;
    ref.second = quality;

    auto index = this->index(this->rowCount() - 1);
    emit dataChanged(index, index);
}

void ItemsModel::append(DndItem* item)
{
    insertRows(this->rowCount(), 1);
    auto &ref = m_data.back();
    ref.first = item;
    ref.second = item->qualityRand()->rand();

    auto index = this->index(this->rowCount() - 1);
    emit dataChanged(index, index);
}

void ItemsModel::remove(int row)
{
    removeRows(row, 1);
}

/*int ItemsModel::getQuality(DndItem* item)
{
    qDebug() << item->id();
    auto it = std::find_if(m_data.begin(), m_data.end(), [item](const std::pair<DndItem*,int> &r){
        return r.first == item;
    });
    qDebug() << it->first->id();
    qDebug() << it->second;
    return it->second;
}*/
