#ifndef DNDCHARLISTMODEL_H
#define DNDCHARLISTMODEL_H

#include <QObject>
#include "commonmodel.h"
#include "abstractfilemodel.h"
#include "lockedfilesset.h"
#include "dndcharlistdata.h"
#include "dndsettingmodel.h"

class DndCharlistModel : public CommonModel, public AbstractFileModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit DndCharlistModel(QObject *parent = nullptr);
    Q_INVOKABLE virtual QObject* newObject(QObject* parent = 0);
    void setLockedFilesSet(LockedFilesSet *lockedFiles);
    //virtual bool append(QObject* ptr);

    void setSettingModel(DndSettingModel* model);

    Q_INVOKABLE virtual bool importFile(QString path);

private:
    LockedFilesSet *m_lockedFiles;
    DndSettingModel *m_settingModel;
};

#endif // DNDCHARLISTMODEL_H
