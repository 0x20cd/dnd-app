#include "charbonus.h"
#include <algorithm>

CharBonus::CharBonus(QObject *parent)
    : QObject{parent}
{

}

int CharBonus::get(DndChar *chr)
{
    return m_map.value(chr, 0);
}

void CharBonus::set(DndChar *chr, int val)
{
    if (val == 0) {
        if (m_map.contains(chr)) {
            chr->refCounter--;
            m_map.remove(chr);
        }
        return;
    }

    if (this->get(chr) == val)
        return;

    if (m_map.contains(chr))
        m_map[chr] = val;
    else {
        m_map.insert(chr, val);
        chr->refCounter++;
    }

    emit this->dataChanged();
}

QHash<DndChar*,int> CharBonus::data()
{
    return m_map;
}

QByteArray CharBonus::hash()
{
    QList<QByteArray> hashes;

    for (DndChar *p : m_map.keys())
        hashes.append(p->hash() + AbstractHashable::hashBytes(QByteArray::number(m_map[p])));

    std::sort(hashes.begin(), hashes.end());

    QByteArray bytes;
    for (const QByteArray &h : hashes)
        bytes += h;

    return AbstractHashable::hashBytes(bytes);
}
