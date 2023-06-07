#ifndef ABSTRACTFILEDATA_H
#define ABSTRACTFILEDATA_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

class AbstractFileData
{
public:
    explicit AbstractFileData(QString filePath = QString());
    virtual bool readFile() = 0;
    virtual bool saveFile() = 0;
    virtual bool removeFile();
    QString filePath();
    virtual void setFilePath(QString filePath);

protected:
    QFile m_file;
};

#endif // ABSTRACTFILEDATA_H
