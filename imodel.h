#ifndef IMODEL_H
#define IMODEL_H

#include <QObject>

class IModel
{
public:
    IModel(bool takeOwnership = false, bool manageCounter = false);
    virtual bool append(QObject* ptr) = 0;
    virtual void clear() = 0;
    virtual void beforeRemove(QObject* ptr);
    virtual void beforeAppend(QObject* ptr);
protected:
    bool m_takeOwnership, m_manageCounter;
};

#endif // IMODEL_H
