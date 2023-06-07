#ifndef APPENGINE_H
#define APPENGINE_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QtQml/qqmlregistration.h>
#include <QHash>
#include <QFile>
#include <QStandardPaths>

#include "dndsettingmodel.h"
#include "dndtemplatemodel.h"
#include "dndcharlistmodel.h"
#include "lockedfilesset.h"
#include "dndtemplatedata.h"

class AppEngine : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(bool isSignedIn READ isSignedIn NOTIFY signedInChanged)
    Q_PROPERTY(bool isMaster READ isMaster NOTIFY masterChanged)
    Q_PROPERTY(bool isPasswordSet READ isPasswordSet NOTIFY passwordSetChanged)
    Q_PROPERTY(DndSettingModel* settingModel READ settingModel CONSTANT)
    Q_PROPERTY(DndTemplateModel* templateModel READ templateModel CONSTANT)
    Q_PROPERTY(DndCharlistModel* charlistModel READ charlistModel CONSTANT)

public:
    explicit AppEngine(QObject *parent = nullptr);
    bool isSignedIn() const;
    bool isMaster() const;
    bool isPasswordSet() const;

    DndSettingModel* settingModel();
    DndTemplateModel* templateModel();
    DndCharlistModel* charlistModel();

    Q_INVOKABLE bool signIn();
    Q_INVOKABLE bool signIn(QString password);
    Q_INVOKABLE void signOut();
    Q_INVOKABLE bool changePassword(QString newpass, QString oldpass = QString());

    Q_INVOKABLE DndTemplateData* createTemplate(DndSettingData *setting);
    Q_INVOKABLE DndCharlistData* generateCharlist(DndTemplateData *templ);

    Q_INVOKABLE QString getUniqueId();

signals:
    void passwordSetChanged();
    void masterChanged();
    void signedInChanged();

private slots:
    void saveConfigFile();

private:
    bool verifyPassword(QString password);

    bool m_isPasswordSet, m_isSignedIn, m_isMaster;
    QString m_password, m_appDataPath;
    DndSettingModel m_settingModel;
    DndTemplateModel m_templateModel;
    DndCharlistModel m_charlistModel;
    LockedFilesSet m_lockedFiles;
};

#endif // APPENGINE_H
