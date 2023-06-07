#include "imodel.h"
#include <QDebug>

IModel::IModel(bool takeOwnership, bool manageCounter)
    : m_takeOwnership(takeOwnership)
    , m_manageCounter(manageCounter)
{
    //qDebug() << "IMODEL" << this;
}

void IModel::beforeRemove(QObject* ptr) { Q_UNUSED(ptr) }

void IModel::beforeAppend(QObject* ptr) { Q_UNUSED(ptr) }
