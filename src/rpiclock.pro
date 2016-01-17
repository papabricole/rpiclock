TEMPLATE = app
TARGET = rpiclock
DEPENDPATH += .
INCLUDEPATH += .

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG -= app_bundle

# Input
SOURCES += RpiClock.cpp \
           main.cpp

HEADERS += RpiClock.h
