#ifndef IMPORT_BUTTONS_H
#define IMPORT_BUTTONS_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void select_file();
    void select_folder();
    bool folder_validation(const QString& folder);

private:
    Ui::MainWindow *ui;

    QString selectedFilePath;
    QPushButton *fileImport;
    QPushButton *playlistImport;
    QLabel *label;
};


#endif // IMPORT_BUTTONS_H
