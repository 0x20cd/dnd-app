TEMPLATE = app

QT += qml quick widgets
SOURCES += main.cpp \
    abstractdndcharacter.cpp \
    abstractfiledata.cpp \
    abstractfilemodel.cpp \
    abstracthashable.cpp \
    appengine.cpp \
    charbonus.cpp \
    commonmodel.cpp \
    dndchar.cpp \
    dndcharlistdata.cpp \
    dndcharlistmodel.cpp \
    dndrandom.cpp \
    dndsettingdata.cpp \
    dndsettingmodel.cpp \
    dndskill.cpp \
    dndtemplatedata.cpp \
    dndtemplatemodel.cpp \
    entitydata.cpp \
    entitymodel.cpp \
    imodel.cpp \
    itemsmodel.cpp \
    levelmodel.cpp \
    lockedfilesset.cpp \
    dnditem.cpp \
    dndrace.cpp \
    dndclass.cpp \
    randommodel.cpp
RESOURCES += res.qrc

HEADERS += \
    abstractdndcharacter.h \
    abstractfiledata.h \
    abstractfilemodel.h \
    abstracthashable.h \
    appengine.h \
    charbonus.h \
    commonmodel.h \
    dndchar.h \
    dndcharlistdata.h \
    dndcharlistmodel.h \
    dndrandom.h \
    dndsettingdata.h \
    dndsettingmodel.h \
    dndskill.h \
    dndtemplatedata.h \
    dndtemplatemodel.h \
    entitydata.h \
    entitymodel.h \
    imodel.h \
    itemsmodel.h \
    levelmodel.h \
    lockedfilesset.h \
    dnditem.h \
    dndrace.h \
    dndclass.h \
    randommodel.h

CONFIG += qmltypes

QML_IMPORT_NAME = AppEngineModule
QML_IMPORT_MAJOR_VERSION = 1
QML_IMPORT_MINOR_VERSION = 0
