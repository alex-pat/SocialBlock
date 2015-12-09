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


SOURCES += src/main.cpp \
    src/blockinterval.cpp \
    src/profile.cpp \
    src/manager.cpp \
    src/connector.cpp

HEADERS  += \
    src/blockinterval.h \
    src/profile.h \
    src/manager.h \
    src/connector.h

RESOURCES += \
    res/resources.qrc
