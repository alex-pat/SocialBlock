#-------------------------------------------------
#
# Project created by QtCreator 2015-10-19T22:17:33
#
#-------------------------------------------------

QT += core \
    gui \
    quick \
    qml \
    widgets \
    quickwidgets

TARGET = SocialBlock
TEMPLATE = app
CONFIG += c++11

SOURCES += src/main.cpp \
    src/blockinterval.cpp \
    src/profile.cpp \
    src/database.cpp \
    src/manager.cpp

HEADERS  += \
    src/blockinterval.h \
    src/profile.h \
    src/database.h \
    src/manager.h \
    src/os_dependent.h

RESOURCES += \
    res/resources.qrc
