#include "abstractdndcharacter.h"

AbstractDndCharacter::AbstractDndCharacter(QObject *parent)
    : QObject(parent)
    , m_settingModel(nullptr)
    , m_setting(nullptr)
    , m_race(nullptr)
    , m_cls(nullptr)
    , m_xp(0)
{}

QString AbstractDndCharacter::settingHash()
{
    return m_settingHash;
}

DndSettingData* AbstractDndCharacter::setting()
{
    return m_setting;
}

QString AbstractDndCharacter::name()
{
    return m_name;
}

QString AbstractDndCharacter::desc()
{
    return m_desc;
}

DndRace* AbstractDndCharacter::race()
{
    return m_race;
}

DndClass* AbstractDndCharacter::cls()
{
    return m_cls;
}

int AbstractDndCharacter::xp()
{
    return m_xp;
}

void AbstractDndCharacter::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged();
}

void AbstractDndCharacter::setDesc(QString desc)
{
    if (m_desc == desc)
        return;

    m_desc = desc;
    emit descChanged();
}

void AbstractDndCharacter::setRace(DndRace* race)
{
    if (m_race == race)
        return;

    m_race = race;
    if (race)
        m_raceId = race->id();
    emit raceChanged();
}

void AbstractDndCharacter::setCls(DndClass* cls)
{
    if (m_cls == cls)
        return;

    m_cls = cls;
    if (cls)
        m_clsId = cls->id();
    emit clsChanged();
}

void AbstractDndCharacter::setXp(int xp)
{
    if (m_xp == xp)
        return;

    m_xp = xp;
    emit xpChanged();
}

void AbstractDndCharacter::setSetting(DndSettingData* setting)
{
    qDebug() << "setSetting" << setting;
    m_setting = setting;

    if (setting) {
        setting->updateHash();
        m_settingHash = setting->id();
        setRaceById(m_raceId);
        setClsById(m_clsId);
        QObject::connect(setting, &QObject::destroyed, this, [this](QObject*){ this->setSetting(nullptr); });
    }

    emit this->settingChanged();
    emit this->validChanged();
}

void AbstractDndCharacter::setSettingById(QString settingHash)
{
    m_settingHash = settingHash;
    DndSettingData *setting = static_cast<DndSettingData*>(m_settingModel->getById(settingHash));
    this->setSetting(setting);
}

void AbstractDndCharacter::setRaceById(QString raceId)
{
    m_raceId = raceId;
    if (!m_setting)
        return;
    DndRace *race = static_cast<DndRace*>(m_setting->races()->getById(raceId));
    this->setRace(race);
}

void AbstractDndCharacter::setClsById(QString clsId)
{
    m_clsId = clsId;
    if (!m_setting)
        return;
    DndClass *cls = static_cast<DndClass*>(m_setting->classes()->getById(clsId));
    this->setCls(cls);
}

void AbstractDndCharacter::setSettingModel(DndSettingModel* model)
{
    m_settingModel = model;
    QObject::connect(m_settingModel, &EntityModel::dataChanged, this,
        [this](const QModelIndex&, const QModelIndex&, const QList<int>&)
        {
            if (m_setting) return;

            DndSettingData *ptr = static_cast<DndSettingData*>(m_settingModel->getById(m_settingHash));
            if (ptr) this->setSetting(ptr);
        }
    );
}

bool AbstractDndCharacter::isValid()
{
    return m_setting;
}
