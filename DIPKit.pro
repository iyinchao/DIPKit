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
    module/dipmoduleht.cpp \
    module/dipmoduleag.cpp \
    module/dipmodulecv.cpp \
    dipimagelabel.cpp \
    module/dipmodulemm.cpp \
    dipcolortag.cpp

HEADERS  += \
    dipkit.h \
    dipelidelabel.h \
    diphistowidget.h \
    dipimageview.h \
    dipalphascrollarea.h \
    diptooldialog.h \
    module/dipmodulebase.h \
    module/dipmoduleht.h \
    module/dipmoduleag.h \
    module/dipmodulecv.h \
    dipimagelabel.h \
    module/dipmodulemm.h \
    dipcolortag.h

FORMS    += dipkit.ui

OTHER_FILES += \
    artwork/alpha_bg.ai \
    snippet.txt \
    todo.txt \
    blog.txt

RESOURCES += \
    dipkit.qrc

RC_ICONS = resource/icon.ico
