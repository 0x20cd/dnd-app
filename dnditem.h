#ifndef DNDITEM_H
#define DNDITEM_H

#include "entitydata.h"
#include <QObject>
#include "dndchar.h"
#include "dndrandom.h"

class DndItem : public EntityData, public virtual AbstractHashable
{
    Q_OBJECT
    QML_ELEMENT
    //Q_PROPERTY(int cost READ cost WRITE setCost NOTIFY costChanged)
    Q_PROPERTY(DndChar* chr READ chr WRITE setChr NOTIFY chrChanged)
    Q_PROPERTY(DndRandom* itemRand READ itemRand WRITE setItemRand NOTIFY itemRandChanged)
    Q_PROPERTY(DndRandom* qualityRand READ qualityRand WRITE setQualityRand NOTIFY qualityRandChanged)
public:
    explicit DndItem(QObject *parent = nullptr);
    //int cost();
    DndChar* chr();
    DndRandom* itemRand();
    DndRandom* qualityRand();
    //void setCost(int cost);
    void setChr(DndChar* chr);
    void setItemRand(DndRandom* itemRand);
    void setQualityRand(DndRandom* qualityRand);
    virtual QByteArray hash();
signals:
    //void costChanged();
    void chrChanged();
    void itemRandChanged();
    void qualityRandChanged();
private:
    //int m_cost;
    DndChar *m_chr;
    DndRandom *m_itemRand, *m_qualityRand;
};

#endif // DNDITEM_H
