#-------------------------------------------------
#
# Project created by QtCreator 2016-06-15T17:00:56
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = magazyn
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dictionary.cpp

HEADERS  += mainwindow.h \
    dictionary.h \
    settings.h

FORMS    += mainwindow.ui \
    dictionary.ui

QTPLUGIN += QSQLMYSQL
