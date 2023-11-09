TEMPLATE = app
TARGET = MP3_Player

QT = core gui
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    applicationbuttons.cpp \
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
    editingbuttons.h \
    interface.h \
    mediaplayer.h \
    picturebox.h \
    playlist.h \
    song.h \
    textmetadata.h

RESOURCES += \
    resources.qrc

