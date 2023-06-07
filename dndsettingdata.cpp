#include "dndsettingdata.h"
#include <QFileInfo>

const QString DndSettingData::M_HEAD("dnd-setting");

DndSettingData::DndSettingData(QObject *parent)
    : EntityData(parent)
    , m_testRand(nullptr)
    , m_genRand(nullptr)
{}

DndSettingData* DndSettingData::createCopy()
{
    //qDebug() << "P?" << this << this->parent();
    DndSettingData *obj = new DndSettingData(this->parent());


    obj->setFilePath(m_file.fileName());
    if (!obj->readFile()) {
        delete obj;
        return nullptr;
    }

    QString filename = m_file.fileName();//QFileInfo(path).fileName();
    if (filename.endsWith(".json"))
        filename.chop(5);

    int i = 1;
    while (QFileInfo::exists(filename + QString::number(i) + ".json")) ++i;

    filename.append(QString::number(i) + ".json");

    obj->setFilePath(filename);

    qDebug() << "parent" << obj->parent();
    return obj;
}

void DndSettingData::updateHash()
{
    QByteArray bytes;

    bytes = AbstractHashable::hashBytes(m_display.toUtf8());
    bytes += AbstractHashable::hashBytes(m_about.toUtf8());
    bytes += m_randoms.hash();
    bytes += m_chars.hash();
    bytes += m_skills.hash();
    bytes += m_races.hash();
    bytes += m_classes.hash();
    bytes += m_items.hash();
    bytes += m_levels.hash();
    bytes += m_testRand ? m_testRand->hash() : AbstractHashable::hashBytes(QByteArray());
    bytes += m_genRand ? m_genRand->hash() : AbstractHashable::hashBytes(QByteArray());

    QString id = QString::fromLatin1(AbstractHashable::hashBytes(bytes).toHex().toLower());
    if (m_id == id)
        return;

    QString oldId = m_id;
    m_id = id;

    emit idChangedFrom(oldId);
    emit idChanged();
}

bool DndSettingData::readFile()
{

    if (!m_file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QJsonDocument doc = QJsonDocument::fromJson(m_file.readAll());
    m_file.close();

    if (doc.object()["head"].toString() != M_HEAD)
        return false;

    QJsonObject body = doc.object()["body"].toObject();
    this->setDisplay(body["display"].toString());
    this->setAbout(body["about"].toString());

    m_randoms.clear();
    for (QJsonValueRef rand : body["randoms"].toArray())
    {
        const QJsonObject &obj = rand.toObject();
        DndRandom *entity = new DndRandom();
        entity->setId(obj["id"].toString());
        entity->setDisplay(obj["display"].toString());
        entity->setAbout(obj["about"].toString());
        QJsonArray dist = obj["dist"].toArray();

        for (const QJsonValue& value : dist) {
            entity->dist()->append(value["value"].toInt(), value["weight"].toInt());
        }
        m_randoms.append(entity);
    }

    m_chars.clear();
    for (QJsonValueRef chr : body["chars"].toArray())
    {
        DndChar *entity = new DndChar();
        QJsonObject obj = chr.toObject();
        entity->setId(obj["id"].toString());
        entity->setDisplay(obj["display"].toString());
        entity->setAbout(obj["about"].toString());
        m_chars.append(entity);
    }

    m_skills.clear();
    for (QJsonValueRef skill : body["skills"].toArray())
    {
        DndSkill *entity = new DndSkill();
        QJsonObject obj = skill.toObject();
        entity->setId(obj["id"].toString());
        entity->setDisplay(obj["display"].toString());
        entity->setAbout(obj["about"].toString());
        entity->setKoeff(obj["koeff"].toInt());
        entity->setChr(static_cast<DndChar*>(m_chars.getById(obj["char"].toString())));
        m_skills.append(entity);
    }

    m_races.clear();
    for (QJsonValueRef race : body["races"].toArray())
    {
        DndRace *entity = new DndRace();
        QJsonObject obj = race.toObject();
        entity->setId(obj["id"].toString());
        entity->setDisplay(obj["display"].toString());
        entity->setAbout(obj["about"].toString());

        QJsonObject charBonus = obj["charBonus"].toObject();
        for (const QString &k : charBonus.keys())
            entity->charBonus()->set(static_cast<DndChar*>(m_chars.getById(k)), charBonus[k].toInt());

        m_races.append(entity);
    }

    m_items.clear();
    for (QJsonValueRef item : body["items"].toArray())
    {
        DndItem *entity = new DndItem();
        QJsonObject obj = item.toObject();
        entity->setId(obj["id"].toString());
        entity->setDisplay(obj["display"].toString());
        entity->setAbout(obj["about"].toString());
        //entity->setCost(obj["cost"].toInt());
        entity->setChr(static_cast<DndChar*>(m_chars.getById(obj["char"].toString())));
        entity->setItemRand(static_cast<DndRandom*>(m_randoms.getById(obj["itemRand"].toString())));
        entity->setQualityRand(static_cast<DndRandom*>(m_randoms.getById(obj["qualityRand"].toString())));
        m_items.append(entity);
    }

    m_classes.clear();
    for (QJsonValueRef cls : body["classes"].toArray())
    {
        DndClass *entity = new DndClass();
        QJsonObject obj = cls.toObject();
        entity->setId(obj["id"].toString());
        entity->setDisplay(obj["display"].toString());
        entity->setAbout(obj["about"].toString());

        for (QJsonValueRef skill : obj["skills"].toArray())
            entity->skills()->append(static_cast<QObject*>(m_skills.getById(skill.toString())));

        for (QJsonValueRef item : obj["items"].toArray()) {
            QObject *p = static_cast<QObject*>(m_items.getById(item.toString()));
            if (!p) {
                qDebug() << "!-!-!" << item.toString();
            }
            entity->items()->append(p);
        }

        entity->setMoneyRand(static_cast<DndRandom*>(m_randoms.getById(obj["moneyRand"].toString())));
        //entity->setMoneyAlt(obj["moneyAlt"].toInt());

        m_classes.append(entity);
    }

    m_levels.clear();

    for (QJsonValueRef level : body["levels"].toArray())
    {
        QJsonObject obj;
        int xpDiff = level.toObject()["xpDiff"].toInt();
        int pbDiff = level.toObject()["pbDiff"].toInt();
        m_levels.insert(m_levels.rowCount(), xpDiff, pbDiff);
    }

    this->setTestRand(static_cast<DndRandom*>(m_randoms.getById(body["charTestRand"].toString())));
    this->setGenRand(static_cast<DndRandom*>(m_randoms.getById(body["charGenRand"].toString())));

    updateHash();

    return true;
}

bool DndSettingData::saveFile()
{
    if (m_file.fileName().endsWith(".UNNAMED")) {
        QString filename = m_file.fileName().chopped(8) + "NEWFILE";// + QFile::encodeName(this->name());

        int i = 0;
        while (QFileInfo::exists(filename + QString::number(i) + ".json")) ++i;

        filename = filename + QString::number(i) + ".json";
        m_file.setFileName(filename);
    }

    QJsonObject body;
    body["display"] = this->display();
    body["about"] = this->about();

    if (!m_testRand || !m_genRand) {
        return false;
    }
    body["charTestRand"] = m_testRand->id();
    body["charGenRand"] = m_genRand->id();

    QJsonArray randoms;
    for (int i = 0, n = m_randoms.rowCount(); i < n; ++i) {
        DndRandom* e = m_randoms.data(m_randoms.index(i), EntityModel::PtrRole).value<DndRandom*>();
        QJsonObject obj;
        obj["id"] = e->id();
        obj["display"] = e->display();
        obj["about"] = e->about();

        QJsonArray dist;
        for (int j = 0, m = e->dist()->rowCount(); j < m; ++j) {
            dist.append(QJsonObject{
                {"value", e->dist()->data(e->dist()->index(j), RandomModel::ValueRole).toInt()},
                {"weight", e->dist()->data(e->dist()->index(j), RandomModel::WeightRole).toInt()}
            });
        }
        obj["dist"] = dist;
        randoms.append(obj);
    }
    body["randoms"] = randoms;

    QJsonArray chars;
    for (int i = 0, n = m_chars.rowCount(); i < n; ++i) {
        DndChar* e = m_chars.data(m_chars.index(i), EntityModel::PtrRole).value<DndChar*>();
        QJsonObject obj;
        obj["id"] = e->id();
        obj["display"] = e->display();
        obj["about"] = e->about();
        chars.append(obj);
    }
    body["chars"] = chars;

    QJsonArray skills;
    for (int i = 0, n = m_skills.rowCount(); i < n; ++i) {
        DndSkill* e = m_skills.data(m_skills.index(i), EntityModel::PtrRole).value<DndSkill*>();
        QJsonObject obj;
        obj["id"] = e->id();
        obj["display"] = e->display();
        obj["about"] = e->about();
        if (!e->chr()) {
            return false;
        }
        obj["char"] = e->chr()->id();
        obj["koeff"] = e->koeff();
        skills.append(obj);
    }
    body["skills"] = skills;

    QJsonArray races;
    for (int i = 0, n = m_races.rowCount(); i < n; ++i) {
        DndRace* e = m_races.data(m_races.index(i), EntityModel::PtrRole).value<DndRace*>();
        QJsonObject obj;
        obj["id"] = e->id();
        obj["display"] = e->display();
        obj["about"] = e->about();

        QJsonObject charBonus;
        auto map = e->charBonus()->data();

        for (DndChar *k : map.keys())
            charBonus[k->id()] = map[k];

        obj["charBonus"] = charBonus;
        races.append(obj);
    }
    body["races"] = races;

    QJsonArray classes;
    for (int i = 0, n = m_classes.rowCount(); i < n; ++i) {
        DndClass* e = m_classes.data(m_classes.index(i), EntityModel::PtrRole).value<DndClass*>();
        QJsonObject obj;
        obj["id"] = e->id();
        obj["display"] = e->display();
        obj["about"] = e->about();
        if (!e->moneyRand()) {
            return false;
        }
        obj["moneyRand"] = e->moneyRand()->id();

        QJsonArray skills;
        for (int j = 0, m = e->skills()->rowCount(); j < m; ++j) {
            DndSkill* skill = e->skills()->data(e->skills()->index(j), CommonModel::PtrRole).value<DndSkill*>();
            skills.append(skill->id());
        }
        obj["skills"] = skills;

        QJsonArray items;
        for (int j = 0, m = e->items()->rowCount(); j < m; ++j) {
            DndItem* item = e->items()->data(e->items()->index(j), CommonModel::PtrRole).value<DndItem*>();
            items.append(item->id());
        }
        obj["items"] = items;

        //obj["moneyAlt"] = e->moneyAlt();

        classes.append(obj);
    }
    body["classes"] = classes;

    QJsonArray items;
    for (int i = 0, n = m_items.rowCount(); i < n; ++i) {
        DndItem* e = m_items.data(m_items.index(i), EntityModel::PtrRole).value<DndItem*>();
        QJsonObject obj;
        obj["id"] = e->id();
        obj["display"] = e->display();
        obj["about"] = e->about();
        //obj["cost"] = e->cost();
        if (!e->chr() || !e->itemRand() || !e->qualityRand()) {
            return false;
        }
        obj["char"] = e->chr()->id();
        obj["itemRand"] = e->itemRand()->id();
        obj["qualityRand"] = e->qualityRand()->id();
        items.append(obj);
    }
    body["items"] = items;

    QJsonArray levels;
    for (int i = 0, n = m_levels.rowCount(); i < n; ++i) {
        int xpDiff = m_levels.data(m_levels.index(i), LevelModel::XpDiffRole).toInt();
        int pbDiff = m_levels.data(m_levels.index(i), LevelModel::PbDiffRole).toInt();

        levels.append(QJsonObject{
            {"xpDiff", xpDiff},
            {"pbDiff", pbDiff}
        });
    }
    body["levels"] = levels;

    QJsonDocument doc(QJsonObject{{"head", M_HEAD}, {"body", body}});
    QByteArray text = doc.toJson();

    if (!this->m_file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    updateHash();

    auto n = m_file.write(text, text.size());
    m_file.close();
    return n >= 0;
}

bool DndSettingData::removeFile()
{
    return AbstractFileData::removeFile();
}

DndRandom* DndSettingData::testRand()
{
    return m_testRand;
}

DndRandom* DndSettingData::genRand()
{
    return m_genRand;
}

void DndSettingData::setTestRand(DndRandom* testRand)
{
    if (m_testRand == testRand)
        return;

    if (m_testRand) m_testRand->refCounter--;
    if (testRand) testRand->refCounter++;

    m_testRand = testRand;
    emit testRandChanged();
}

void DndSettingData::setGenRand(DndRandom* genRand)
{
    if (m_genRand == genRand)
        return;

    if (m_genRand) m_genRand->refCounter--;
    if (genRand) genRand->refCounter++;

    m_genRand = genRand;
    emit genRandChanged();
}

EntityModel* DndSettingData::randoms()
{
    return &m_randoms;
}

EntityModel* DndSettingData::chars()
{
    return &m_chars;
}

EntityModel* DndSettingData::skills()
{
    return &m_skills;
}

EntityModel* DndSettingData::races()
{
    return &m_races;
}

EntityModel* DndSettingData::classes()
{
    return &m_classes;
}

EntityModel* DndSettingData::items()
{
    return &m_items;
}

LevelModel* DndSettingData::levels()
{
    return &m_levels;
}

