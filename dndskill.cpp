#include "dndskill.h"

DndSkill::DndSkill(QObject *parent)
    : EntityData{parent}
    , m_chr(nullptr)
{}

DndChar* DndSkill::chr()
{
    return m_chr;
}

void DndSkill::setChr(DndChar* chr)
{
    if (m_chr == chr)
        return;

    if (m_chr) m_chr->refCounter--;
    if (chr) chr->refCounter++;

    m_chr = chr;
    emit chrChanged();
}

int DndSkill::koeff()
{
    return m_koeff;
}

void DndSkill::setKoeff(int koeff)
{
    if (m_koeff == koeff)
        return;

    m_koeff = koeff;
    emit koeffChanged();
}

QByteArray DndSkill::hash()
{
    return AbstractHashable::hashBytes(
        EntityData::hash()
        + m_chr->hash()
        + AbstractHashable::hashBytes(QByteArray::number(m_koeff))
    );
}
