#ifndef DNDRACE_H
#define DNDRACE_H

#include "entitydata.h"
#include <QObject>
#include <QJsonObject>
#include "abstracthashable.h"
#include "charbonus.h"

class DndRace : public EntityData, public virtual AbstractHashable
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(CharBonus* charBonus READ charBonus CONSTANT)
public:
    explicit DndRace(QObject *parent = nullptr);
    CharBonus* charBonus();
    virtual QByteArray hash();
private:
    CharBonus m_charBonus;
};

#endif // DNDRACE_H
