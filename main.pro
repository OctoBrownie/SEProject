TEMPLATE = app
TARGET = MP3_Player

QT = core gui
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    playlist.cpp \
    song.cpp \

HEADERS += \
    playlist.h \
    song.h \

