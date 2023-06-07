#include "levelmodel.h"
#include <algorithm>

LevelModel::LevelModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

QByteArray LevelModel::hash()
{
    QByteArray bytes;
    for (const Lvl &lvl : m_data)
        bytes += AbstractHashable::hashBytes(QByteArray::number(lvl.xpDiff) + ';' + QByteArray::number(lvl.pbDiff)); //AbstractHashable::hashJsonIntObject(lvl.toObject());

    return AbstractHashable::hashBytes(bytes);
}

int LevelModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.size();
}

QVariant LevelModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const Lvl &obj = m_data[index.row()];

    switch (role) {
    case XpRole:
        return QVariant::fromValue(obj.xp);
    case PbRole:
        return QVariant::fromValue(obj.pb);
    case XpDiffRole:
        return QVariant::fromValue(obj.xpDiff);
    case PbDiffRole:
        return QVariant::fromValue(obj.pbDiff);
    default:
        return QVariant();
    }
}

bool LevelModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    Lvl &ref = m_data[index.row()];
    int val = value.toInt();

    switch (role) {
    case XpDiffRole:
        ref.xpDiff = val;
        break;
    case PbDiffRole:
        ref.pbDiff = val;
        break;
    default:
        return false;
    }

    updateData();
    emit dataChanged(index, this->index(this->rowCount() - 1));
    return true;
}

QHash<int, QByteArray> LevelModel::roleNames() const
{
    static const QHash<int, QByteArray> ROLE_NAMES{
        {XpRole, "xp"},
        {PbRole, "pb"},
        {XpDiffRole, "xpDiff"},
        {PbDiffRole, "pbDiff"}
    };
    return ROLE_NAMES;
}

bool LevelModel::insertRows(int row, int count, const QModelIndex &parent)
{
    this->beginInsertRows(parent, row, row + count - 1);
    m_data.insert(m_data.begin() + row, count, Lvl{});
    this->endInsertRows();
    return true;
}

bool LevelModel::removeRows(int row, int count, const QModelIndex &parent)
{
    this->beginRemoveRows(parent, row, row + count - 1);
    m_data.erase(m_data.begin() + row, m_data.begin() + row + count);
    this->endRemoveRows();
    return true;
}

void LevelModel::insert(int row, int xpDiff, int pbDiff)
{
    if (row < 0)
        row = this->rowCount();

    insertRows(row, 1);
    Lvl &ref = m_data.back();
    ref.xpDiff = xpDiff;
    ref.pbDiff = pbDiff;
    updateData();
}

void LevelModel::remove(int row)
{
    removeRows(row, 1);
    updateData();
}

void LevelModel::clear()
{
    m_data.clear();
}

void LevelModel::updateData()
{
    int xp = 0, pb = 0;
    for (Lvl &ref : m_data) {
        ref.xp = (xp += ref.xpDiff);
        ref.pb = (pb += ref.pbDiff);
    }
}

QJsonObject LevelModel::getLvlByXp(int xp)
{
    //qDebug() << (m_data.begin() == m_data.end());
    //return QJsonObject{{"level", 0}, {"pb", 0}};

    auto it = std::upper_bound(
        m_data.begin(), m_data.end(), xp,
        [](int lhs, const Lvl &rhs) -> bool {
            return lhs < rhs.xp;
        }
    );

    int level = it - m_data.begin() + 1;

    int pb = level > 1 ? (--it)->pb : 0;

    return QJsonObject{{"level", level}, {"pb", pb}};
}
