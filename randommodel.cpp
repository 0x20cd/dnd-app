#include "randommodel.h"
#include <algorithm>
#include <QRandomGenerator>

RandomModel::RandomModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

QByteArray RandomModel::hash()
{
    QList<QByteArray> hashes;
    for (const QJsonObject &val : m_data)
        hashes.append(AbstractHashable::hashJsonIntObject(QJsonObject{{"value",val["value"].toInt()},{"weight",val["weight"].toInt()}}));
    std::sort(hashes.begin(), hashes.end());

    QByteArray bytes;
    for (const QByteArray& h : hashes)
        bytes += h;

    return AbstractHashable::hashBytes(bytes);
}

int RandomModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.size();
}

QVariant RandomModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const QJsonObject &obj = m_data.at(index.row());

    switch (role) {
    case ValueRole:
        return QVariant::fromValue(obj["value"].toInt());
    case WeightRole:
        return QVariant::fromValue(obj["weight"].toInt());
    default:
        return QVariant();
    }
}

bool RandomModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    QJsonObject &ref = m_data[index.row()];
    int val = value.toInt();

    switch (role) {
    case ValueRole:
        ref["value"] = val;
        break;
    case WeightRole:
        ref["weight"] = val;
        break;
    default:
        return false;
    }

    updateData();
    return true;
}

QHash<int, QByteArray> RandomModel::roleNames() const
{
    static const QHash<int, QByteArray> ROLE_NAMES{
        {ValueRole, "value"},
        {WeightRole, "weight"}
    };
    return ROLE_NAMES;
}

bool RandomModel::insertRows(int row, int count, const QModelIndex &parent)
{
    this->beginInsertRows(parent, row, row + count - 1);
    m_data.insert(row, count, QJsonObject());
    this->endInsertRows();
    return true;
}

bool RandomModel::removeRows(int row, int count, const QModelIndex &parent)
{
    this->beginRemoveRows(parent, row, row + count - 1);
    m_data.remove(row, count);
    this->endRemoveRows();
    return true;
}

void RandomModel::append(int value, int weight)
{
    insertRows(this->rowCount(), 1);
    QJsonObject &obj = m_data.back();
    obj["value"] = value;
    obj["weight"] = weight;
    updateData();
}

void RandomModel::remove(int row)
{
    removeRows(row, 1);
    updateData();
}

void RandomModel::updateData()
{
    int bound = 0;
    for (QJsonObject &ref : m_data)
        ref["bound"] = (bound += ref["weight"].toInt());
    emit dataChanged(this->index(0), this->index(this->rowCount() - 1));
}

int RandomModel::rand() const
{
    if (m_data.empty())
        return 0;

    int n = QRandomGenerator::global()->bounded(m_data.last()["bound"].toInt());

    auto it = std::upper_bound(m_data.begin(), m_data.end(), n,
        [](int lhs, const QJsonValue &rhs) -> bool {
            return lhs < rhs["bound"].toInt();
        }
    );

    return it->value("value").toInt();
}
