#-------------------------------------------------
#
# Project created by QtCreator 2016-02-04T23:44:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = virsh-gui
TEMPLATE = app


SOURCES += main.cpp\
        virshgui.cpp \
        sshcommunication.cpp \
        vm.cpp\
        diskimage.cpp\
        Snapshot.cpp\
        tinyxml2/tinyxml2.cpp\
        basic-xml-syntax-highlighter/basic-xml-syntax-highlighter/BasicXMLSyntaxHighlighter.cpp

HEADERS  += virshgui.h \
    sshcommunication.h \
    vm.h\
    diskimage.h\
    Snapshot.h\
    tinyxml2/tinyxml2.h\
    basic-xml-syntax-highlighter/basic-xml-syntax-highlighter/BasicXMLSyntaxHighlighter.h

FORMS    += virshgui.ui

LIBS += -lssh

CONFIG += c++11 debug_and_release
QMAKE_CXXFLAGS += -std=c++11 -g
#DEFINES += "_WIN32"
#QMAKE_LFLAGS += -static
