TEMPLATE = app
TARGET = MP3_Player

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    applicationbuttons.cpp \
    editingbuttons.cpp \
    interface.cpp \
    main.cpp \
    picturebox.cpp \
    textmetadata.cpp

HEADERS += \
    applicationbuttons.h \
    editingbuttons.h \
    interface.h \
    picturebox.h \
    textmetadata.h

