TEMPLATE = app
TARGET = mp3_player

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += multimedia

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/mp3_player.cpp \
    $$PWD/src/music_app.cpp

HEADERS += \
   $$PWD/include/mp3_player.h \
   $$PWD/include/music_app.h

INCLUDEPATH += include/



