#-------------------------------------------------
#
# Project created by QtCreator 2014-04-22T14:10:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XTypo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    drawarea.cpp \
    xgrphicspixmapitem.cpp \
    xlayer.cpp \
    beziercurve.cpp \
    xgraphicsscene.cpp \
    xcolorgraphicsitem.cpp \
    xgraphicsitem.cpp \
    blendersplitter.cpp \
    xGraphicsLineItem.cpp \
    xrectangleitem.cpp \
    xellipseitem.cpp \
    xboxedgraphicsitem.cpp \
    xtransform.cpp \
    xeditor.cpp

HEADERS  += mainwindow.h \
    drawarea.h \
    xgrphicspixmapitem.h \
    xgraphicsscene.h \
    beziercurve.h \
    xlayer.h \
    xgraphicsitem.h \
    xcolorgraphicsitem.h \
    blendersplitter.h \
    xGraphicsLineItem.h \
    xrectangleitem.h \
    xellipseitem.h \
    xboxedgraphicsitem.h \
    xtransform.h \
    xeditor.h

FORMS    += mainwindow.ui

RESOURCES += \
    typoResource.qrc
