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
    dipalphascrollarea.cpp \
    diptooldialog.cpp \
    module/dipmodulebase.cpp \
    module/dipmoduleht.cpp

HEADERS  += \
    dipkit.h \
    dipelidelabel.h \
    diphistowidget.h \
    dipimageview.h \
    dipalphascrollarea.h \
    diptooldialog.h \
    module/dipmodulebase.h \
    module/dipmoduleht.h

FORMS    += dipkit.ui

OTHER_FILES += \
    artwork/alpha_bg.ai \
    snippet.txt \
    todo.txt

RESOURCES += \
    dipkit.qrc
