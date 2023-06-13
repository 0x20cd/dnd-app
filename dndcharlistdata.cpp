#include "dndcharlistdata.h"
#include <algorithm>

const QString DndCharlistData::M_HEAD("dnd-charlist");

DndCharlistData::DndCharlistData(QObject *parent)
    : AbstractDndCharacter{parent}
    , m_level(1)
    , m_pb(0)
    , m_money(0)
    , m_isLocked(false)
    , m_lockedFiles(nullptr)
{}

bool DndCharlistData::exportFile(QString dest)
{
    return AbstractFileData::exportFile(dest);
}

void DndCharlistData::setLockedFilesSet(LockedFilesSet *lockedFiles)
{
    m_lockedFiles = lockedFiles;

//    bool locked = m_lockedFiles->data()->contains(QFileInfo(m_file.fileName()).fileName());

//    if (m_isLocked == locked)
//        return;

//    m_isLocked = locked;
//    emit lockedChanged();
}

bool DndCharlistData::removeFile()
{
    QString filename = QFileInfo(this->filePath()).fileName();
    if (AbstractFileData::removeFile()) {
        m_lockedFiles->remove(filename);
        return true;
    }
    return false;
}

void DndCharlistData::setFilePath(QString filePath)
{
    AbstractFileData::setFilePath(filePath);
    bool locked = m_lockedFiles->data()->contains(QFileInfo(m_file.fileName()).fileName());

    if (m_isLocked == locked)
        return;

    m_isLocked = locked;
    emit lockedChanged();
}

void DndCharlistData::setXp(int xp)
{
    if (xp == m_xp)
        return;

    m_xp = xp;
    emit xpChanged();

    if (!m_setting)
        return;

    QJsonObject obj = m_setting->levels()->getLvlByXp(xp);
    int level = obj["level"].toInt(), pb = obj["pb"].toInt();

    if (m_pb != pb) {
        m_pb = pb;
        emit pbChanged();
    }

    if (m_level != level) {
        m_level = level;
        emit levelChanged();
    }
}

int DndCharlistData::money()
{
    return m_money;
}

void DndCharlistData::setMoney(int money)
{
    if (m_money == money)
        return;
    m_money = money;
    emit moneyChanged();
}

int DndCharlistData::pb()
{
    return m_pb;
}

int DndCharlistData::level()
{
    return m_level;
}

bool DndCharlistData::isLocked()
{
    return m_isLocked;
}

void DndCharlistData::setLocked(bool isLocked)
{
    if (m_isLocked == isLocked)
        return;

    m_isLocked = isLocked;

    qDebug() << m_file.fileName();

    if (m_isLocked)
        m_lockedFiles->insert(QFileInfo(m_file.fileName()).fileName());
    else m_lockedFiles->remove(QFileInfo(m_file.fileName()).fileName());

    emit lockedChanged();
}

int DndCharlistData::testChar(QString charId, DndSkill* skill)
{
    int res = m_chars[charId].toInt() + m_setting->testRand()->rand();
    if (skill)
        res += m_pb * skill->koeff() / 100;

    return res;
}

int DndCharlistData::useItem(int i)
{
    DndItem *item = m_items.data(m_items.index(i), ItemsModel::ItemRole).value<DndItem*>();
    int quality = m_items.data(m_items.index(i), ItemsModel::QualityRole).value<int>();
    return (m_chars[item->chr()->id()].toInt() + item->itemRand()->rand() + m_pb) * quality / 100;
}

QJsonObject* DndCharlistData::chars()
{
    return &m_chars;
}

QJsonObject DndCharlistData::charsObj()
{
    return m_chars;
}

ItemsModel* DndCharlistData::items()
{
    return &m_items;
}

bool DndCharlistData::readFile()
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

    this->setSettingById(body["settingHash"].toString());

    this->setName(body["name"].toString());
    this->setDesc(body["description"].toString());
    this->setRaceById(body["race"].toString());//(static_cast<DndRace*>(m_setting->races()->getById(body["race"].toString())));
    this->setClsById(body["class"].toString());//(static_cast<DndClass*>(m_setting->classes()->getById(body["class"].toString())));
    this->setXp(body["xp"].toInt());
    this->setMoney(body["money"].toInt());
    m_chars = body["chars"].toObject();

    QJsonObject items = body["items"].toObject();
    if (m_setting) {
        for (const QString &itemId : items.keys()) {
            DndItem* item = static_cast<DndItem*>(m_setting->items()->getById(itemId));
            int quality = items[itemId].toInt();
            m_items.append(item, quality);
        }
    } else {
        m_itemsRaw = items;
    }

    return true;
}

bool DndCharlistData::saveFile()
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
    body["race"] = m_raceId;//m_race->id();
    body["class"] = m_clsId;//m_cls->id();
    body["xp"] = m_xp;
    body["money"] = m_money;
    body["chars"] = m_chars;

    QJsonObject items;
    if (m_setting) {
        for (int i = 0; i < m_items.rowCount(); ++i) {
            DndItem *item = m_items.data(m_items.index(i), ItemsModel::ItemRole).value<DndItem*>();
            int quality = m_items.data(m_items.index(i), ItemsModel::QualityRole).toInt();
            items[item->id()] = quality;
        }
    } else {
        items = m_itemsRaw;
    }
    body["items"] = items;

    /*QJsonArray items;
    for (int i = 0, n = m_items.rowCount(); i < n; ++i)
        items.append(m_items.data(m_items.index(i)).value<DndItem*>()->id());*/


    QJsonDocument doc(QJsonObject{{"head", M_HEAD}, {"body", body}});
    QByteArray text = doc.toJson();

    auto n = m_file.write(text, text.size());
    m_file.close();
    return n >= 0;
}
