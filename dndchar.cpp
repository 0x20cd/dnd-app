#include "dndchar.h"

DndChar::DndChar(QObject *parent)
    : EntityData{parent}
{}

QByteArray DndChar::hash()
{
    return EntityData::hash();
}
