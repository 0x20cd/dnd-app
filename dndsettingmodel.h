#ifndef DNDSETTINGMODEL_H
#define DNDSETTINGMODEL_H

#include <QObject>
#include <QHash>
#include "entitymodel.h"
#include "dndsettingdata.h"
#include "abstractfilemodel.h"

class DndSettingModel : public EntityModel, public AbstractFileModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit DndSettingModel(QObject *parent = nullptr);
    Q_INVOKABLE virtual QObject* newObject(QObject* parent = 0);

    Q_INVOKABLE virtual bool importFile(QString path);
    virtual void beforeAppend(QObject* ptr);
};

#endif // DNDSETTINGMODEL_H
