#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class Playlist;
class QVBoxLayout;

QT_BEGIN_NAMESPACE
namespace Ui { class SearchWindow; }
QT_END_NAMESPACE

class SearchWindow : public QMainWindow
{
    Q_OBJECT

public:
    //Constructor and Destructor
    SearchWindow(Playlist* playlist, QWidget *parent = nullptr);
    ~SearchWindow();

    //Run the query and return the results of the query in a GUI
    void query();

    //Set and change the playlist
    void setPlaylist(Playlist* playlist);


private:
    //GUI
    Ui::SearchWindow *ui;

    //Currently searching playlist
    Playlist* playlist;

    //The layout containing all of the songs
    QVBoxLayout* songs;
};
#endif // MAINWINDOW_H
