#include "equalizerwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    EqualizerWindow equalizerWindow;
    equalizerWindow.show();

    return a.exec();
}
