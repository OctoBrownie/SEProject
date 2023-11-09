#ifndef APPLICATIONBUTTONS_H
#define APPLICATIONBUTTONS_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>

class ApplicationButtons: public QHBoxLayout
{
public:
    ApplicationButtons(QWidget* parent=nullptr);
};

#endif // APPLICATIONBUTTONS_H

