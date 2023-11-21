TEMPLATE = app
TARGET = mp3_player

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += multimedia

SOURCES += \
    src/main.cpp \
    src/musicplayer.cpp \
    src/app.cpp \
    src/equalizer.cpp \
    src/song.cpp \
    src/customsongwidget.cpp

HEADERS += \
   include/app.h \
   include/equalizer.h \
   include/customsongwidget.h \
   include/musicplayer.h \
   include/song.h

RESOURCES += \
    resources.qrc

INCLUDEPATH += include/

# for fftw3
INCLUDEPATH += $$PWD/lib/fftw-3.3.5-dll64
LIBS += -L$$PWD/lib/fftw-3.3.5-dll64 -llibfftw3-3
win32:!win32-g++ {
    PRE_TARGETDEPS += $$PWD\lib\fftw-3.3.5-dll64\libfftw3f-3.lib \
    $$PWD\lib\fftw-3.3.5-dll64\libfftw3l-3.lib \
    $$PWD\lib\fftw-3.3.5-dll64\libfftw3-3.lib
}

# for ffmpeg
INCLUDEPATH += $$PWD/lib/ffmpeg-6.1/include
LIBS += -L$$PWD/lib/ffmpeg-6.1/lib -L$$PWD/lib/ffmpeg-6.1/bin -lavcodec -lavformat -lavutil
win32 {
    win32-g++ {
        PRE_TARGETDEPS += $$PWD/lib/ffmpeg-6.1/lib/libavcodec.dll.a \
        $$PWD/lib/ffmpeg-6.1/lib/libavdevice.dll.a \
        $$PWD/lib/ffmpeg-6.1/lib/libavfilter.dll.a \
        $$PWD/lib/ffmpeg-6.1/lib/libavformat.dll.a \
        $$PWD/lib/ffmpeg-6.1/lib/libavutil.dll.a \
        $$PWD/lib/ffmpeg-6.1/lib/libpostproc.dll.a \
        $$PWD/lib/ffmpeg-6.1/lib/libswresample.dll.a \
        $$PWD/lib/ffmpeg-6.1/lib/libswscale.dll.a \
    }
    else {
        PRE_TARGETDEPS += $$PWD/lib/ffmpeg-6.1/lib/avcodec.lib \
        $$PWD/lib/ffmpeg-6.1/lib/avdevice.lib \
        $$PWD/lib/ffmpeg-6.1/lib/avfilter.lib \
        $$PWD/lib/ffmpeg-6.1/lib/avformat.lib \
        $$PWD/lib/ffmpeg-6.1/lib/avutil.lib \
        $$PWD/lib/ffmpeg-6.1/lib/postproc.lib \
        $$PWD/lib/ffmpeg-6.1/lib/swresample.lib \
        $$PWD/lib/ffmpeg-6.1/lib/swscale.lib
    }
}

# for SDL
INCLUDEPATH += $$PWD/lib/SDL2-2.28.5/include
LIBS += -L$$PWD/lib/SDL2-2.28.5/lib -lsdl2
win32:!win32-g++ {
    PRE_TARGETDEPS += $$PWD/lib/SDL2-2.28.5/lib/SDL2.lib \
    $$PWD/lib/SDL2-2.28.5/lib/SDL2main.lib \
    $$PWD/lib/SDL2-2.28.5/lib/SDL2test.lib
}

# for TagLib
INCLUDEPATH += $$PWD/lib/taglib/include
LIBS += -L$$PWD/lib/taglib/lib/ -ltag
win32 {
    win32-g++: PRE_TARGETDEPS += $$PWD/lib/taglib/lib/libtag.a
    else: PRE_TARGETDEPS += $$PWD/lib/taglib/lib/tag.lib
}
DEFINES += TAGLIB_STATIC
