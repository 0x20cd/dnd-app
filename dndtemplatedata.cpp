#include "dndtemplatedata.h"

const QString DndTemplateData::M_HEAD("dnd-template");

DndTemplateData::DndTemplateData(QObject *parent)
    : AbstractDndCharacter{parent}
{}

bool DndTemplateData::populateCharlist(DndCharlistData *charlist)
{
    if (!m_race || !m_cls || !m_setting || !m_settingModel)
        return false;

    charlist->setSettingModel(m_settingModel);
    charlist->setSetting(m_setting);
    charlist->setName(m_name);
    charlist->setDesc(m_desc);
    charlist->setRace(m_race);
    charlist->setCls(m_cls);
    charlist->setXp(m_xp);

    QJsonObject &chars = *charlist->chars();
    for (int i = 0, n = m_setting->chars()->rowCount(); i < n; ++i) {
        DndChar *chr = m_setting->chars()->data(m_setting->chars()->index(i), EntityModel::PtrRole).value<DndChar*>();
        chars[chr->id()] = m_setting->genRand()->rand() + m_race->charBonus()->get(chr);
        qDebug() << chr->display();
    }

    qDebug() << *charlist->chars();

    int money = m_cls->moneyRand()->rand();
    charlist->setMoney(money);

    for (int i = 0; i < m_cls->items()->rowCount(); ++i) {
        charlist->items()->append(m_cls->items()->data(m_cls->items()->index(i), CommonModel::PtrRole).value<DndItem*>());
    }

    return true;
}

bool DndTemplateData::readFile()
{
    if (!m_file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QJsonDocument doc = QJsonDocument::fromJson(m_file.readAll());
    m_file.close();

    if (doc.object()["head"].toString() != M_HEAD)
        return false;

    QJsonObject body = doc.object()["body"].toObject();

    /*m_setting = static_cast<DndSettingData*>(m_settingModel->getById(body["settingHash"].toString()));
    if (!m_setting)
        return false;
    emit settingChanged();*/

    //DndSettingData *setting = static_cast<DndSettingData*>(m_settingModel->getById(body["settingHash"].toString()));
    this->setSettingById(body["settingHash"].toString());

    this->setName(body["name"].toString());
    this->setDesc(body["description"].toString());
    this->setRaceById(body["race"].toString());
    this->setClsById(body["class"].toString());
    this->setXp(body["initialXp"].toInt());
    /*if (this->m_setting) {
        this->setRace(static_cast<DndRace*>(m_setting->races()->getById(body["race"].toString())));
        this->setCls(static_cast<DndClass*>(m_setting->classes()->getById(body["class"].toString())));
    }*/

    return true;
}

bool DndTemplateData::saveFile()
{
    if (m_file.fileName().endsWith(".UNNAMED")) {
        QString filename = m_file.fileName().chopped(8) + "NEWFILE";// + QFile::encodeName(this->name());

        int i = 0;
        while (QFileInfo::exists(filename + QString::number(i) + ".json")) ++i;

        filename = filename + QString::number(i) + ".json";
        m_file.setFileName(filename);
    }

    if (!m_race || !m_cls)
        return false;

    if (!this->m_file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QJsonObject body;
    body["settingHash"] = m_settingHash; //m_setting->id();
    body["name"] = m_name;
    body["description"] = m_desc;
    body["race"] = m_raceId; //m_race->id();
    body["class"] = m_clsId; //m_cls->id();
    body["initialXp"] = m_xp;

    QJsonDocument doc(QJsonObject{{"head", M_HEAD}, {"body", body}});
    QByteArray text = doc.toJson();

    auto n = m_file.write(text, text.size());
    m_file.close();
    return n >= 0;
}
