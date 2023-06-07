#include "abstractfiledata.h"

AbstractFileData::AbstractFileData(QString filePath)
    : m_file(filePath) {}

bool AbstractFileData::removeFile()
{
    return m_file.remove();
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
