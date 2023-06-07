#include "dndtemplatemodel.h"

DndTemplateModel::DndTemplateModel(QObject *parent)
    : IModel(true, false)
    , CommonModel{parent}
    //, AbstractFileModel(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/templates")
{
//    m_takeOwnership = true;
//    m_manageCounter = false;
}

QObject* DndTemplateModel::newObject(QObject *parent)
{
    DndTemplateData *obj = new DndTemplateData(parent);
    obj->setFilePath(m_dir.absoluteFilePath(".UNNAMED"));
    obj->setSettingModel(m_settingModel);
    return obj;
}

void DndTemplateModel::setSettingModel(DndSettingModel* model)
{
    m_settingModel = model;
}

bool DndTemplateModel::importFile(QString path)
{
    return AbstractFileModel::importFile(path);
}
