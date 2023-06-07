#include "appengine.h"
#include <QRandomGenerator>

AppEngine::AppEngine(QObject *parent)
    : QObject{parent}
    , m_isSignedIn(false)
    , m_isMaster(false)
    , m_appDataPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))
{
    /*QDir dir(m_appDataPath);
    dir.mkpath("./charlists");
    dir.mkpath("./templates");
    dir.mkpath("./settings");*/
    QDir(m_appDataPath).mkpath(".");

    QFile configFile(m_appDataPath + "/config.json");

    QJsonObject config;
    if (configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        config = QJsonDocument::fromJson(configFile.readAll()).object();
        configFile.close();
    }

    m_password = config["masterPassword"].toString();
    m_isPasswordSet = !m_password.isEmpty();

    QJsonArray locked = config["lockedCharlists"].toArray();
    for (const QJsonValue &fname : locked) {
        m_lockedFiles.insert(fname.toString());
    }

    connect(&m_lockedFiles, &LockedFilesSet::dataChanged, this, &AppEngine::saveConfigFile);

    m_settingModel.setDir(m_appDataPath + "/settings");
    m_settingModel.readDir();

    m_templateModel.setSettingModel(&m_settingModel);
    m_templateModel.setDir(m_appDataPath + "/templates");
    m_templateModel.readDir();

    m_charlistModel.setSettingModel(&m_settingModel);
    m_charlistModel.setDir(m_appDataPath + "/charlists");
    m_charlistModel.setLockedFilesSet(&m_lockedFiles);
    m_charlistModel.readDir();
}

QString AppEngine::getUniqueId()
{
    QString id;
    for (int i = 0; i < 2; ++i)
        id += QString::number(QRandomGenerator::global()->generate64(), 16).rightJustified(16, '0');
    //qDebug() << "iqefq" << id;
    return id;
}

void AppEngine::saveConfigFile()
{
    QJsonObject config;
    QJsonArray locked;

    for (const QString &fname : *m_lockedFiles.data())
        locked.append(fname);

    config["masterPassword"] = m_password;
    config["lockedCharlists"] = locked;

    QFile configFile(m_appDataPath + "/config.json");

    if (configFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QByteArray text = QJsonDocument(config).toJson();
        configFile.write(text, text.size());
        configFile.close();
    }
}

bool AppEngine::isPasswordSet() const
{
    return m_isPasswordSet;
}

bool AppEngine::changePassword(QString newpass, QString oldpass)
{
    if (newpass.isEmpty() || !verifyPassword(oldpass))
        return false;

    m_password = newpass;
    if (oldpass.isEmpty()) {
        m_isPasswordSet = true;
        emit passwordSetChanged();
    }
    saveConfigFile();
    return true;
}

bool AppEngine::signIn()
{
    if (!m_isSignedIn) {
        m_isSignedIn = true;
        emit signedInChanged();
    }

    if (m_isMaster) {
        m_isMaster = false;
        emit masterChanged();
    }

    return true;
}

bool AppEngine::signIn(QString password)
{
    bool verif = verifyPassword(password);

    if (m_isSignedIn != verif) {
        m_isSignedIn = verif;
        emit signedInChanged();
    }

    if (m_isMaster != verif) {
        m_isMaster = verif;
        emit masterChanged();
    }

    return m_isSignedIn;
}

void AppEngine::signOut()
{
    if (m_isSignedIn) {
        m_isSignedIn = false;
        emit signedInChanged();
    }

    if (m_isMaster) {
        m_isMaster = false;
        emit masterChanged();
    }
}

bool AppEngine::verifyPassword(QString password)
{
    return m_password == password;
}

bool AppEngine::isSignedIn() const
{
    return m_isSignedIn;
}

bool AppEngine::isMaster() const
{
    return m_isMaster;
}

DndSettingModel* AppEngine::settingModel()
{
    return &m_settingModel;
}

DndTemplateModel* AppEngine::templateModel()
{
    return &m_templateModel;
}

DndCharlistModel* AppEngine::charlistModel()
{
    return &m_charlistModel;
}

DndTemplateData* AppEngine::createTemplate(DndSettingData *setting)
{
    DndTemplateData *obj = static_cast<DndTemplateData*>(m_templateModel.newObject(this));
    obj->setSetting(setting);

    return obj;
}

DndCharlistData* AppEngine::generateCharlist(DndTemplateData *templ)
{
    DndCharlistData *charlist = static_cast<DndCharlistData*>(m_charlistModel.newObject(this));

    templ->populateCharlist(charlist);

    //obj->setSetting(setting);

    return charlist;
}

/*void AppEngine::deleteObject(QObject *obj)
{
    delete obj;
}*/
