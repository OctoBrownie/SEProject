TEMPLATE = app
TARGET = MP3_Player

QT = core gui
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    src/interface.cpp \
    src/main.cpp \
    src/mediaplayer.cpp \
    src/picturebox.cpp \
    src/playlist.cpp \
    src/settingswindow.cpp \
    src/song.cpp \
    src/textmetadata.cpp

HEADERS += \
    include/interface.h \
    include/mediaplayer.h \
    include/picturebox.h \
    include/playlist.h \
    include/song.h \
    include/textmetadata.h \
    include/settingswindow.h

INCLUDEPATH += include/

RESOURCES += \
    resources.qrc


# for TagLib
INCLUDEPATH += $$PWD/lib/taglib/include
LIBS += -L$$PWD/lib/taglib/lib/ -ltag
win32 {
    win32-g++: PRE_TARGETDEPS += $$PWD/lib/taglib/lib/libtag.a
    else: PRE_TARGETDEPS += $$PWD/lib/taglib/lib/tag.lib
}
DEFINES += TAGLIB_STATIC
