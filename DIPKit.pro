#-------------------------------------------------
#
# Project created by QtCreator 2014-10-27T20:09:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DIPKit
TEMPLATE = app


SOURCES += main.cpp \
    dipkit.cpp \
    dipimageview.cpp \
    dipelidelabel.cpp \
    diphistowidget.cpp \
    module/test.cpp \
    dipalphascrollarea.cpp

HEADERS  += \
    dipkit.h \
    dipelidelabel.h \
    diphistowidget.h \
    module/test.h \
    dipimageview.h \
    dipalphascrollarea.h

FORMS    += dipkit.ui

OTHER_FILES += \
    snippet.txt \
    artwork/alpha_bg.ai

RESOURCES += \
    dipkit.qrc
