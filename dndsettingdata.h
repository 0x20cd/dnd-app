#ifndef DNDSETTINGDATA_H
#define DNDSETTINGDATA_H

#include <QObject>
#include <QList>
#include <QJsonArray>
#include <QString>
#include <QtQml/qqmlregistration.h>
#include "commonmodel.h"
#include "entitymodel.h"
#include "dndrandom.h"
#include "abstractfiledata.h"
#include "dndchar.h"
#include "dndskill.h"
#include "dnditem.h"
#include "dndrace.h"
#include "dndclass.h"
#include "levelmodel.h"

class DndSettingData : public EntityData, public AbstractFileData
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString eid READ id NOTIFY idChanged) // updateHash must be called manually when needed!
    Q_PROPERTY(EntityModel* randoms READ randoms CONSTANT)
    Q_PROPERTY(EntityModel* chars READ chars CONSTANT)
    Q_PROPERTY(EntityModel* skills READ skills CONSTANT)
    Q_PROPERTY(EntityModel* races READ races CONSTANT)
    Q_PROPERTY(EntityModel* classes READ classes CONSTANT)
    Q_PROPERTY(EntityModel* items READ items CONSTANT)
    Q_PROPERTY(LevelModel* levels READ levels CONSTANT)
    Q_PROPERTY(DndRandom* testRand READ testRand WRITE setTestRand NOTIFY testRandChanged)
    Q_PROPERTY(DndRandom* genRand READ genRand WRITE setGenRand NOTIFY genRandChanged)

public:
    explicit DndSettingData(QObject *parent = nullptr);
    EntityModel* randoms();
    EntityModel* chars();
    EntityModel* skills();
    EntityModel* races();
    EntityModel* classes();
    EntityModel* items();
    LevelModel* levels();

    DndRandom* testRand();
    DndRandom* genRand();

    void setTestRand(DndRandom* rand);
    void setGenRand(DndRandom* rand);

    virtual bool readFile();
    Q_INVOKABLE virtual bool saveFile();
    Q_INVOKABLE virtual bool removeFile();
    Q_INVOKABLE bool exportFile(QString dest);

    Q_INVOKABLE DndSettingData* createCopy();

    void updateHash();

signals:
    void hashChanged();
    void nameChanged();
    void testRandChanged();
    void genRandChanged();

private:
    EntityModel m_randoms, m_chars, m_skills, m_races, m_classes, m_items;
    LevelModel m_levels;
    DndRandom *m_testRand, *m_genRand;
    static const QString M_HEAD;
};

#endif // DNDSETTINGDATA_H
