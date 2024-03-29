#-------------------------------------------------
#
# Project created by QtCreator 2015-05-31T18:04:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = photo-restoration
TEMPLATE = app

INCLUDEPATH += "/usr/include/opencv/include"

CONFIG += c++11

LIBS +=    -L/usr/lib64/opencv \
-lopencv_core \
-lopencv_highgui \
-lopencv_imgproc

SOURCES += main.cpp\
        mainwindow.cpp \
    processedimagescollection.cpp \
    processorutils.cpp \
    ramponiprocessor.cpp \
    imagedisplayer.cpp \
    processorconfiggenerator.cpp

HEADERS  += mainwindow.h \
    iprocessor.h \
    processedimagescollection.h \
    processorutils.h \
    rationalfilter.h \
    ramponiprocessor.h \
    imagedisplayer.h \
    processorconfiggenerator.h

FORMS    += mainwindow.ui
