# Software Engineering I Project

Created by:
- Connor Benware
- Erik Billquist
- Jack Kammerer
- Crystal Lee
- Max Maldonado
- Bryan Piedra

### Notes for Setup & Install:
This project must be compiled and built through using the Qt framework. This means that qmake, cmake, and the relevant Qt .dll libraries are all installed. The easiest way to compile this project is:

- Open the "main.pro" file in Qt Creator
- Set the project to compile using MinGW
- Click "Run"

This is the fastest method for starting the software. This will create a build folder that will not run. In the build directory, add the "./config/settings.json" folder from the source code into the directory. 

To build for release, consult the Qt documentation.

All of the libraries used in this project are open source. The libraries included in this directory are prebuilt for Windows 64-bit using the MinGW compiler. To use on your system, rebuild the libraries for your system. Here is the source code for the libraries used in this project:

- Qt: [https://github.com/qt/qt5](https://github.com/qt/qt5)
- TagLib: [https://github.com/taglib/taglib](https://github.com/taglib/taglib)
- FFMPEG: [https://github.com/FFmpeg/FFmpeg](https://github.com/FFmpeg/FFmpeg)
- SDL2: [https://github.com/libsdl-org/SDL](https://github.com/libsdl-org/SDL)


