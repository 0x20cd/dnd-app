#include "entitydata.h"

EntityData::EntityData(QObject *parent)
    : QObject{parent}
    , refCounter(0)
{}

QByteArray EntityData::hash()
{
    return AbstractHashable::hashBytes(
        AbstractHashable::hashBytes(m_id.toUtf8())
        + AbstractHashable::hashBytes(m_display.toUtf8())
        + AbstractHashable::hashBytes(m_about.toUtf8())
    );
}

QString EntityData::id() const
{
    return m_id;
}

QString EntityData::display() const
{
    return m_display;
}

QString EntityData::about() const
{
    return m_about;
}

void EntityData::setId(QString id)
{
    if (m_id == id)
        return;

    QString oldId = m_id;
    m_id = id;
    emit idChangedFrom(oldId);
    emit idChanged();
}

void EntityData::setDisplay(QString display)
{
    if (m_display == display)
        return;

    m_display = display;
    emit displayChanged();
}

void EntityData::setAbout(QString about)
{
    if (m_about == about)
        return;

    m_about = about;
    emit aboutChanged();
}
