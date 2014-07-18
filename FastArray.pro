#-------------------------------------------------
#
# Project created by QtCreator 2014-07-14T20:19:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FastArray
TEMPLATE = app


SOURCES += main.cpp\
        editor.cpp \
    editorview.cpp \
    plugincentral.cpp

HEADERS  += editor.h \
    editorview.h \
    plugininterface.h \
    importerplugininterface.h \
    plugincentral.h

FORMS    += editor.ui
