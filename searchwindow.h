#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class SearchWindow; }
QT_END_NAMESPACE

class SearchWindow : public QMainWindow
{
    Q_OBJECT

public:
    SearchWindow(QWidget *parent = nullptr);
    ~SearchWindow();

private:
    Ui::SearchWindow *ui;
};
#endif // MAINWINDOW_H
