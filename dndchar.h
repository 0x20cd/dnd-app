#ifndef DNDCHAR_H
#define DNDCHAR_H

#include "entitydata.h"
#include <QObject>
#include "abstracthashable.h"

class DndChar : public EntityData, public virtual AbstractHashable
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit DndChar(QObject *parent = nullptr);
    virtual QByteArray hash();

};

#endif // DNDCHAR_H
