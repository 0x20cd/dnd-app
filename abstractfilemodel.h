#ifndef ABSTRACTFILEMODEL_H
#define ABSTRACTFILEMODEL_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include "imodel.h"
#include "abstractfiledata.h"

class AbstractFileModel : public virtual IModel
{
public:
    AbstractFileModel();
    virtual QObject* newObject(QObject* parent = 0) = 0;
    void readDir();
    void setDir(QString dir);
    virtual bool importFile(QString path);
    virtual void beforeRemove(QObject* ptr);
protected:
    QDir m_dir;
    //QString m_fileDir;
};

#endif // ABSTRACTFILEMODEL_H
