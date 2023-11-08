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

# for fftw3
INCLUDEPATH += $$PWD/lib/fftw-3.3.5-dll64
LIBS += -L$$PWD/lib/fftw-3.3.5-dll64 -llibfftw3-3
DEPENDPATH += $$PWD/lib/fftw-3.3.5-dll64

# for ffmpeg
INCLUDEPATH += $$PWD/lib/ffmpeg-6.1/include
LIBS += -L$$PWD/lib/ffmpeg-6.1/lib
DEPENDPATH += $$PWD/lib/ffmpeg-6.1/bin

