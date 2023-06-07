#ifndef DNDCLASS_H
#define DNDCLASS_H

#include "entitydata.h"
#include <QObject>
#include "commonmodel.h"
#include "dndrandom.h"

class DndClass : public EntityData, public virtual AbstractHashable
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(CommonModel* skills READ skills CONSTANT)
    Q_PROPERTY(CommonModel* items READ items CONSTANT)
    Q_PROPERTY(DndRandom* moneyRand READ moneyRand WRITE setMoneyRand NOTIFY moneyRandChanged)
    /*Q_PROPERTY(DndRandom* moneyBaseRand READ moneyBaseRand WRITE setMoneyBaseRand NOTIFY moneyBaseRandChanged)
    Q_PROPERTY(int moneyAlt READ moneyAlt WRITE setMoneyAlt NOTIFY moneyAltChanged)*/
public:
    explicit DndClass(QObject *parent = nullptr);
    CommonModel* skills();
    CommonModel* items();
    DndRandom* moneyRand();
    //int moneyAlt();
    //void setMoneyAlt(int amount);
    void setMoneyRand(DndRandom *rand);
    virtual QByteArray hash();
signals:
    //void moneyAltChanged();
    void moneyRandChanged();
private:
    CommonModel m_skills, m_items;
    DndRandom *m_moneyRand;
    //int m_moneyAlt;
};

#endif // DNDCLASS_H
