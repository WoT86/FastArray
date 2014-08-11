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
    plugincentral.cpp \
    logger.cpp \
    loggermessage.cpp \
    projectmanager.cpp \
    array.cpp \
    projectsettings.cpp \
    arraysettings.cpp \
    projecttabwidget.cpp

HEADERS  += editor.h \
    editorview.h \
    plugininterface.h \
    importerplugininterface.h \
    plugincentral.h \
    loggerinterface.h \
    logger.h \
    loggermessage.h \
    projectmanager.h \
    array.h \
    projectsettings.h \
    arraysettings.h \
    projecttabwidget.h

FORMS    += editor.ui

CONFIG(debug, release|debug):DEFINES += _DEBUG
