#-------------------------------------------------
#
# Project created by QtCreator 2012-03-31T10:50:01
#
#-------------------------------------------------

QT       += core gui dbus

TARGET = yakuake-sessions
TEMPLATE = app


SOURCES += main.cpp \
    sessionlist.cpp \
    sessionmodel.cpp \
    session.cpp \
    sessionedit.cpp

HEADERS  += \
    sessionlist.h \
    sessionmodel.h \
    session.h \
    sessionedit.h

FORMS += \
    sessionedit.ui \
    sessionlist.ui
