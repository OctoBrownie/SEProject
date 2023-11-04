#include "interface.h"
#include <QMainWindow>

Interface::Interface(QWidget* centralWidget, QDockWidget* dockedWidget, QWidget* parent) : QMainWindow(parent)
{
    setCentralWidget(centralWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dockedWidget);
    resize(1500, 1000);
}
