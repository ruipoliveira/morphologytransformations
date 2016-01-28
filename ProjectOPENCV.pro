#-------------------------------------------------
#
# Project created by QtCreator 2015-12-20T18:02:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjectOPENCV
TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv

LIBS += `pkg-config opencv --libs --cflags`

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
