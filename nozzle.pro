#-------------------------------------------------
#
# Project created by QtCreator 2017-07-04T17:36:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG  += silent

TARGET   = nozzle
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = .
OBJECTS_DIR = build/obj
MOC_DIR     = build/moc
UI_DIR      = build/ui 
RCC_DIR     = build/rcc

SOURCES += \
    sources/main.cpp \
    sources/mainwindow.cpp \
    sources/backendscript.cpp \
    sources/backendworker.cpp \
    sources/entry.cpp \
    sources/elidedlabel.cpp

HEADERS += \
    includes/mainwindow.hh \
    includes/backendscript.hh \
    includes/backendworker.hh \
    includes/entry.hh \
    includes/elidedlabel.hh

FORMS += \
    forms/mainwindow.ui \
    forms/entry.ui

INCLUDEPATH += \
    includes/
