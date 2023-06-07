#ifndef LOCKEDFILESSET_H
#define LOCKEDFILESSET_H

#include <QObject>
#include <QSet>

class LockedFilesSet : public QObject
{
    Q_OBJECT
public:
    explicit LockedFilesSet(QObject *parent = nullptr);
    void insert(QString filename);
    void remove(QString filename);
    QSet<QString>* data();

signals:
    void dataChanged();

private:
    QSet<QString> m_data;
};

#endif // LOCKEDFILESSET_H
