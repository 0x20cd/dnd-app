#include "dndsettingmodel.h"

DndSettingModel::DndSettingModel(QObject *parent)
    : IModel(true, false)
    , EntityModel{parent}
    //, AbstractFileModel(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/settings")
{}

QObject* DndSettingModel::newObject(QObject* parent)
{
    DndSettingData *obj = new DndSettingData(parent);
    obj->setFilePath(m_dir.absoluteFilePath(".UNNAMED"));

    //qDebug() << obj->filePath();

    return obj;
}

bool DndSettingModel::importFile(QString path)
{
    return AbstractFileModel::importFile(path);
}

void DndSettingModel::beforeAppend(QObject* ptr)
{
    static_cast<DndSettingData*>(ptr)->updateHash();
}
