#ifndef DNDTEMPLATEMODEL_H
#define DNDTEMPLATEMODEL_H

#include <QObject>
#include "commonmodel.h"
#include "abstractfilemodel.h"
#include "dndtemplatedata.h"

class DndTemplateModel : public CommonModel, public AbstractFileModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit DndTemplateModel(QObject *parent = nullptr);
    Q_INVOKABLE virtual QObject* newObject(QObject* parent = 0);

    void setSettingModel(DndSettingModel* model);

    Q_INVOKABLE virtual bool importFile(QString path);

private:
    DndSettingModel *m_settingModel;
};

#endif // DNDTEMPLATEMODEL_H
