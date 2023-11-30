#ifndef BUTTON_H
#define BUTTONS_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class buttons; }
QT_END_NAMESPACE

class buttons : public QMainWindow
{
    Q_OBJECT

public:
    buttons(QWidget *parent = nullptr);
    ~buttons();

private slots:
    QString select_file();
    QString select_folder();
    bool folder_validation(const QString& folder);

private:
    Ui::buttons *ui;

    QString selectedFilePath;
    QPushButton *fileImport;
    QPushButton *playlistImport;
    QLabel *label;
};

#endif // BUTTONS_H
