#-------------------------------------------------
#
# Project created by QtCreator 2021-08-11T00:04:00
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = generator
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(qt-json/qt-json.pri)

SOURCES += main.cpp \
    shared.cpp \
    method.cpp \
    constructor.cpp \
    schema.cpp

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    mtproto.json \
    telegram.json

HEADERS += \
    shared.h \
    method.h \
    constructor.h \
    schema.h
