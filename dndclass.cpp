#include "dndclass.h"

DndClass::DndClass(QObject *parent)
    : EntityData{parent}
    , m_moneyRand(nullptr)
{}

CommonModel* DndClass::skills()
{
    return &m_skills;
}

CommonModel* DndClass::items()
{
    return &m_items;
}

DndRandom* DndClass::moneyRand()
{
    return m_moneyRand;
}

void DndClass::setMoneyRand(DndRandom *rand)
{
    if (m_moneyRand == rand)
        return;

    if (m_moneyRand) m_moneyRand->refCounter--;
    if (rand) rand->refCounter++;

    m_moneyRand = rand;
    emit moneyRandChanged();
}

/*int DndClass::moneyAlt()
{
    return m_moneyAlt;
}

void DndClass::setMoneyAlt(int amount)
{
    if (m_moneyAlt == amount)
        return;

    m_moneyAlt = amount;
    emit moneyAltChanged();
}*/

QByteArray DndClass::hash()
{
    return AbstractHashable::hashBytes(
        EntityData::hash()
        + m_skills.hash()
        + m_items.hash()
        + m_moneyRand->hash()
        //+ AbstractHashable::hashBytes(QByteArray::number(m_moneyAlt))
    );
}
