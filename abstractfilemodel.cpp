#include "abstractfilemodel.h"

AbstractFileModel::AbstractFileModel()
{}

void AbstractFileModel::setDir(QString dir)
{
    m_dir = dir;
    m_dir.mkpath(".");
}

void AbstractFileModel::readDir()
{
    this->clear();

    m_dir.refresh();
    for (const QString &filename : m_dir.entryList())
    {
        if (filename.startsWith("."))
            continue;

        QObject *obj = this->newObject();
        AbstractFileData *objfd = dynamic_cast<AbstractFileData*>(obj);

        objfd->setFilePath(m_dir.absoluteFilePath(filename));
        if (!objfd->readFile()) {
            delete obj;
            continue;
        }

        if (!this->append(obj))
            delete obj;
    }
}

bool AbstractFileModel::importFile(QString path)
{
    QObject *obj = this->newObject();
    AbstractFileData *objfd = dynamic_cast<AbstractFileData*>(obj);

    QUrl url(path);
    if (url.isLocalFile())
        path = url.toLocalFile();

    objfd->setFilePath(path);
    if (!objfd->readFile()) {
        delete obj;
        return false;
    }

    QString filename = QFileInfo(path).fileName();
    if (filename.endsWith(".json"))
        filename.chop(5);

    while(QFileInfo::exists(m_dir.absoluteFilePath(filename + ".json")))
        filename.append('1');
    filename.append(".json");

    objfd->setFilePath(m_dir.absoluteFilePath(filename));
    if (!objfd->saveFile()) {
        delete obj;
        return false;
    }

    if (!this->append(obj)) {
        objfd->removeFile();
        delete obj;
        return false;
    }

    return true;
}

void AbstractFileModel::beforeRemove(QObject* ptr)
{
    AbstractFileData *fd = dynamic_cast<AbstractFileData*>(ptr);
    fd->removeFile();
}
