#include "dndcharlistmodel.h"

DndCharlistModel::DndCharlistModel(QObject *parent)
    : IModel(true, false)
    , CommonModel{parent}
{}

/*bool DndCharlistModel::append(QObject* ptr)
{
    if (CommonModel::append(ptr)) {
        static_cast<DndCharlistData*>(ptr)->setLockedFilesSet(m_lockedFiles);
        return true;
    }
    return false;
}*/

void DndCharlistModel::setLockedFilesSet(LockedFilesSet *lockedFiles)
{
    m_lockedFiles = lockedFiles;
}

QObject* DndCharlistModel::newObject(QObject *parent)
{
    //return new DndCharlistModel(parent);
    DndCharlistData *obj = new DndCharlistData(parent);
    obj->setLockedFilesSet(m_lockedFiles);
    obj->setFilePath(m_dir.absoluteFilePath(".UNNAMED"));
    obj->setSettingModel(m_settingModel);
    return obj;
}

void DndCharlistModel::setSettingModel(DndSettingModel* model)
{
    m_settingModel = model;
}

bool DndCharlistModel::importFile(QString path)
{
    return AbstractFileModel::importFile(path);
}
