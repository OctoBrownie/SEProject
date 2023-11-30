#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include "playlist.h"
#include <songclone.h>



QT_BEGIN_NAMESPACE
namespace Ui { class SearchWindow; }
QT_END_NAMESPACE

class SearchWindow : public QMainWindow
{
    Q_OBJECT

public:
    SearchWindow(Playlist* playlist, QWidget *parent = nullptr);
    ~SearchWindow();

    void query();

protected:


private:
    Ui::SearchWindow *ui;

    Playlist* playlist;

    QVBoxLayout* songs;
};
#endif // MAINWINDOW_H
