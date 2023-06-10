#include "abstractfiledata.h"

AbstractFileData::AbstractFileData(QString filePath)
    : m_file(filePath) {}

bool AbstractFileData::removeFile()
{
    return m_file.remove();
}

bool AbstractFileData::exportFile(QString dest)
{
    QUrl url(dest);
    if (url.isLocalFile())
        dest = url.toLocalFile();

    /*qDebug() << m_file.fileName();
    qDebug() << dest;
    return QFile::copy(m_file.fileName(), dest);*/

    QString filepath = m_file.fileName();
    m_file.setFileName(dest);
    bool res = this->saveFile();
    m_file.setFileName(filepath);
    return res;
}

QString AbstractFileData::filePath()
{
    return m_file.fileName();
}

void AbstractFileData::setFilePath(QString filePath)
{
    if (m_file.isOpen())
        m_file.close();

    m_file.setFileName(filePath);
}
