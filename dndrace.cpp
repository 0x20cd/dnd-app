#include "dndrace.h"

DndRace::DndRace(QObject *parent)
    : EntityData{parent}
{

}

CharBonus* DndRace::charBonus()
{
    return &m_charBonus;
}

QByteArray DndRace::hash()
{
    return AbstractHashable::hashBytes(
        EntityData::hash()
        + m_charBonus.hash()
    );
}
