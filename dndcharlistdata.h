#ifndef DNDCHARLISTDATA_H
#define DNDCHARLISTDATA_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include "lockedfilesset.h"
#include "abstractdndcharacter.h"
#include "itemsmodel.h"

class DndCharlistData : public AbstractDndCharacter
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QJsonObject chars READ charsObj CONSTANT)
    Q_PROPERTY(ItemsModel* items READ items CONSTANT)
    Q_PROPERTY(int money READ money WRITE setMoney NOTIFY moneyChanged)
    Q_PROPERTY(int xp READ xp WRITE setXp NOTIFY xpChanged)
    Q_PROPERTY(int pb READ pb NOTIFY pbChanged)
    Q_PROPERTY(int level READ level NOTIFY levelChanged)
    Q_PROPERTY(bool isLocked READ isLocked WRITE setLocked NOTIFY lockedChanged)
public:
    explicit DndCharlistData(QObject *parent = nullptr);
    void setLockedFilesSet(LockedFilesSet *lockedFiles);

    QJsonObject* chars();
    QJsonObject charsObj();
    ItemsModel* items();
    int money();
    int pb();
    int level();
    bool isLocked();
    void setXp(int xp);
    void setLocked(bool value);
    void setMoney(int money);
    virtual bool removeFile();
    virtual void setFilePath(QString filePath);

    Q_INVOKABLE int testChar(QString charId, DndSkill* skill = nullptr);
    Q_INVOKABLE int useItem(DndItem* item);

    virtual bool readFile();
    Q_INVOKABLE virtual bool saveFile();

signals:
    void pbChanged();
    void levelChanged();
    void lockedChanged();
    void moneyChanged();

private:
    int m_level, m_pb, m_money;
    bool m_isLocked;
    QJsonObject m_chars, m_itemsRaw;
    ItemsModel m_items;
    LockedFilesSet *m_lockedFiles;
    static const QString M_HEAD;
};

#endif // DNDCHARLISTDATA_H
