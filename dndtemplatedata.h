#ifndef DNDTEMPLATEDATA_H
#define DNDTEMPLATEDATA_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include "abstractdndcharacter.h"
#include "dndcharlistdata.h"

class DndTemplateData : public AbstractDndCharacter
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit DndTemplateData(QObject *parent = nullptr);

    Q_INVOKABLE virtual bool readFile();
    Q_INVOKABLE virtual bool saveFile();
    Q_INVOKABLE bool exportFile(QString dest);

    //Q_INVOKABLE DndCharlistData* generateCharlist(QObject *parent = nullptr);
    bool populateCharlist(DndCharlistData *charlist);

signals:

private:
    static const QString M_HEAD;
};

#endif // DNDTEMPLATEDATA_H
