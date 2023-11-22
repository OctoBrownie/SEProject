#include<QWidget>
#include<QDockWidget>

#include "interface.h"

Interface::Interface(QWidget* centralWidget, QDockWidget* dockedWidget, QWidget* parent) : QMainWindow(parent)
{
    setCentralWidget(centralWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dockedWidget);
    resize(1500, 1000);
}
