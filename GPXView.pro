#-------------------------------------------------
#
# Project created by QtCreator 2018-09-24T12:32:13
#
#-------------------------------------------------
QT += xml
QT += charts


TARGET = GPXView
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


HEADERS += \
    chartview.h \
    cdata.h \
    cgeoconvert.h \
    cgpxtools.h \
    mainwidget.h \
    callout.h \
    mainwindow.h \
    speedometermanager.h \
    speedometerchart.h \
    caveragefactory.h \
    qcgaugewidget.h

SOURCES += \
    main.cpp \
    chartview.cpp \
    cdata.cpp \
    cgeoconvert.cpp \
    cgpxtools.cpp \
    mainwidget.cpp \
    callout.cpp \
    mainwindow.cpp \
    speedometermanager.cpp \
    speedometerchart.cpp \
    caveragefactory.cpp \
    qcgaugewidget.cpp
