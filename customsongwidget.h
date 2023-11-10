#ifndef CUSTOMSONGWIDGET_H
#define CUSTOMSONGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>

class CustomSongWidget: public QWidget
{
public:
    CustomSongWidget(QString* title, QString* artist, QString* album, QImage* albumArt);

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // CUSTOMSONGWIDGET_H
