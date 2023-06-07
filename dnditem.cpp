#include "dnditem.h"

DndItem::DndItem(QObject *parent)
    : EntityData{parent}
    , m_chr(nullptr)
    , m_itemRand(nullptr)
    , m_qualityRand(nullptr)
{}

/*int DndItem::cost()
{
    return m_cost;
}*/

DndChar* DndItem::chr()
{
    return m_chr;
}

DndRandom* DndItem::itemRand()
{
    return m_itemRand;
}

DndRandom* DndItem::qualityRand()
{
    return m_qualityRand;
}

/*void DndItem::setCost(int cost)
{
    if (m_cost == cost)
        return;

    m_cost = cost;
    emit costChanged();
}*/

void DndItem::setChr(DndChar* chr)
{
    if (m_chr == chr)
        return;

    if (m_chr) m_chr->refCounter--;
    if (chr) chr->refCounter++;

    qDebug() << "setchr" << chr->display();

    m_chr = chr;
    emit chrChanged();
}

void DndItem::setItemRand(DndRandom* itemRand)
{
    if (m_itemRand == itemRand)
        return;

    if (m_itemRand) m_itemRand->refCounter--;
    if (itemRand) itemRand->refCounter++;

    m_itemRand = itemRand;
    emit itemRandChanged();
}

void DndItem::setQualityRand(DndRandom* qualityRand)
{
    if (m_qualityRand == qualityRand)
        return;

    if (m_qualityRand) m_qualityRand->refCounter--;
    if (qualityRand) qualityRand->refCounter++;

    m_qualityRand = qualityRand;
    emit qualityRandChanged();
}

QByteArray DndItem::hash()
{
    return AbstractHashable::hashBytes(
        EntityData::hash()
        //+ AbstractHashable::hashBytes(QByteArray::number(m_cost))
        + m_chr->hash()
        + m_itemRand->hash()
        + m_qualityRand->hash()
    );
}
