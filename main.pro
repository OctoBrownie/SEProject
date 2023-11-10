TEMPLATE = app
TARGET = MP3_Player

QT = core gui
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    applicationbuttons.cpp \
    customsongwidget.cpp \
    editingbuttons.cpp \
    interface.cpp \
    main.cpp \
    mediaplayer.cpp \
    picturebox.cpp \
    playlist.cpp \
    song.cpp \
    textmetadata.cpp

HEADERS += \
    applicationbuttons.h \
    customsongwidget.h \
    editingbuttons.h \
    interface.h \
    mediaplayer.h \
    picturebox.h \
    playlist.h \
    song.h \
    textmetadata.h

RESOURCES += \
    resources.qrc


win32: LIBS += -L$$PWD/taglib/lib/ -ltag

INCLUDEPATH += $$PWD/taglib/include
DEPENDPATH += $$PWD/taglib/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/taglib/lib/tag.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/taglib/lib/libtag.a

DEFINES += TAGLIB_STATIC
