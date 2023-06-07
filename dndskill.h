#ifndef DNDSKILL_H
#define DNDSKILL_H

#include <QObject>
#include "entitydata.h"
#include "dndchar.h"
#include "abstracthashable.h"

class DndSkill : public EntityData, public virtual AbstractHashable
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(DndChar* chr READ chr WRITE setChr NOTIFY chrChanged)
    Q_PROPERTY(int koeff READ koeff WRITE setKoeff NOTIFY koeffChanged)
public:
    explicit DndSkill(QObject *parent = nullptr);
    DndChar* chr();
    void setChr(DndChar* chr);
    int koeff();
    void setKoeff(int koeff);
    virtual QByteArray hash();
signals:
    void chrChanged();
    void koeffChanged();
private:
    DndChar* m_chr;
    int m_koeff;
};

#endif // DNDSKILL_H
