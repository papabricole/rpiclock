TEMPLATE = app
TARGET = rpiclock
DEPENDPATH += .
INCLUDEPATH += .

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG -= app_bundle

# Input
SOURCES += RpiClock.cpp \
           dht11/common_dht_read.c \
           dht11/pi_2_dht_read.c \
           dht11/pi_2_mmio.c \
           main.cpp

HEADERS += RpiClock.h \
           dht11/pi_2_dht_read.h \
           dht11/common_dht_read.h \
           dht11/pi_2_mmio.h

