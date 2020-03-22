#-------------------------------------------------
#
# Project created by QtCreator 2016-07-29T18:51:52
#
#-------------------------------------------------

#Simple monitoring of sTGC DAQ.
#Siyuan Sun, based on VMM DAQ by Aimilianos Koulouris.

QT+= core gui network widgets

QMAKE_CXXFLAGS += -std=c++11
TARGET = COVID19_GUI
TEMPLATE = app

INCLUDEPATH += $(ROOTSYS)/include
INCLUDEPATH += include
INCLUDEPATH += include/root

LIBS += -L$(ROOTSYS)/lib \
    -lMathCore \
    -lCore \
    -lHist \
    -lGpad


SOURCES +=\
    src/main.cpp\
    src/mainwindow.cpp\
    src/root/canvas.cxx\
    src/Data_Per_Day.cxx\
    src/Data_Per_Province.cxx\
    src/Data_Per_Country.cxx\
    src/Data_Analyzer.cxx

HEADERS +=\
    include/mainwindow.h \
    include/root/canvas.h \
    include/Data_Per_Day.h\
    include/Data_Per_Province.h \
    include/Data_Per_Country.h \
    include/Data_Analyzer.h

FORMS += src/mainwindow.ui

unix:MOC_DIR = build
unix:OBJECTS_DIR = build
unix:RCC_DIR = build
unix:UI_DIR = build

