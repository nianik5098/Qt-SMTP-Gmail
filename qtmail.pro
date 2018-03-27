#-------------------------------------------------
#
# Project created by QtCreator 2018-03-23T20:41:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtmail
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mailnotification.cpp

HEADERS  += mainwindow.h \
    mailnotification.h

FORMS    += mainwindow.ui

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libcurl
