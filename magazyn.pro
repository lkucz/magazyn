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
    dictionary.cpp \
    product.cpp \
    users.cpp \
    workers.cpp \
    documents.cpp \
    production.cpp \
    productlist.cpp \
    settings.cpp

HEADERS  += mainwindow.h \
    dictionary.h \
    settings.h \
    product.h \
    users.h \
    workers.h \
    documents.h \
    production.h \
    productlist.h

FORMS    += mainwindow.ui \
    dictionary.ui \
    product.ui \
    users.ui \
    workers.ui \
    documents.ui \
    production.ui \
    productlist.ui

QTPLUGIN += QSQLMYSQL

OTHER_FILES += \
    DB.txt
