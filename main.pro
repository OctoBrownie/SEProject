TEMPLATE = app
TARGET = MP3_Player

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    mediaplayer.cpp \

HEADERS += \
    mediaplayer.h \

RESOURCES += \
    resources.qrc

