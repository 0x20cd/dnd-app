#ifndef CHARBONUS_H
#define CHARBONUS_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include "dndchar.h"

class CharBonus : public QObject, public virtual AbstractHashable
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit CharBonus(QObject *parent = nullptr);

    Q_INVOKABLE int get(DndChar *chr);
    Q_INVOKABLE void set(DndChar *chr, int val);
    QHash<DndChar*,int> data();

    virtual QByteArray hash();

signals:
    void dataChanged();

private:
    QHash<DndChar*,int> m_map;
};

#endif // CHARBONUS_H
