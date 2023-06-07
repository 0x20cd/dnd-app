#ifndef ABSTRACTDNDCHARACTER_H
#define ABSTRACTDNDCHARACTER_H

#include "abstractfiledata.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>
#include "dndsettingdata.h"
#include "dndsettingmodel.h"

class AbstractDndCharacter : public QObject, public AbstractFileData
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString desc READ desc WRITE setDesc NOTIFY descChanged)
    Q_PROPERTY(DndSettingData* setting READ setting NOTIFY settingChanged)
    Q_PROPERTY(DndRace* race READ race WRITE setRace NOTIFY raceChanged)
    Q_PROPERTY(DndClass* cls READ cls WRITE setCls NOTIFY clsChanged)
    Q_PROPERTY(QString settingHash READ settingHash NOTIFY settingChanged)
    Q_PROPERTY(bool isValid READ isValid NOTIFY validChanged)
    Q_PROPERTY(int xp READ xp WRITE setXp NOTIFY xpChanged)

public:
    AbstractDndCharacter(QObject *parent = 0);

    DndSettingData* setting();
    QString settingHash();
    QString name();
    QString desc();
    DndRace* race();
    DndClass* cls();
    int xp();
    bool isValid();

    void setName(QString name);
    void setDesc(QString desc);
    void setXp(int xp);
    void setSetting(DndSettingData* setting);
    void setRace(DndRace* race);
    void setCls(DndClass* cls);
    void setSettingById(QString settingHash);
    void setRaceById(QString raceId);
    void setClsById(QString clsId);

    void setSettingModel(DndSettingModel* model);

signals:
    void settingChanged();
    void nameChanged();
    void descChanged();
    void raceChanged();
    void clsChanged();
    void xpChanged();
    void validChanged();

protected:
    DndSettingModel* m_settingModel;
    DndSettingData* m_setting;
    QString m_name, m_desc, m_settingHash, m_raceId, m_clsId;
    DndRace *m_race;
    DndClass *m_cls;
    int m_xp;
};

#endif // ABSTRACTDNDCHARACTER_H
