#ifndef INTERFACE_H
#define INTERFACE_H

#include<QMainWindow>

class Interface : public QMainWindow
{
public:
    Interface(QWidget* centralWidget, QDockWidget* dockedWidget, QWidget* parent=nullptr);
};

#endif // INTERFACE_H
