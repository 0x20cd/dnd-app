#include "lockedfilesset.h"
#include <QDebug>

LockedFilesSet::LockedFilesSet(QObject *parent)
    : QObject{parent}
{}

QSet<QString>* LockedFilesSet::data()
{
    return &m_data;
}

void LockedFilesSet::insert(QString filename)
{
    qDebug() << "111!!!" << filename;
    if (m_data.contains(filename))
        return;

    m_data.insert(filename);
    emit dataChanged();
}

void LockedFilesSet::remove(QString filename)
{
    qDebug() << "111!!!" << filename;
    if (!m_data.contains(filename))
        return;

    m_data.remove(filename);
    emit dataChanged();
}
