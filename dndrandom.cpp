#include "dndrandom.h"
#include <algorithm>

DndRandom::DndRandom(QObject *parent) : EntityData(parent) {}

RandomModel* DndRandom::dist()
{
    return &m_dist;
}

int DndRandom::rand() const
{
    return m_dist.rand();
}

QByteArray DndRandom::hash()
{
    return m_dist.hash();
}
