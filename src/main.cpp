#include<QApplication>

#include "music_app.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    Music_App a;
    a.show();

    return app.exec();
}
