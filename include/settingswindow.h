#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow.h>
#include <QLabel.h>
#include <QBoxLayout.h>
#include <QRadioButton.h>
#include <QSlider.h>

class SettingsWindow: public QMainWindow
{
public:
    SettingsWindow(int option, QString filePath);


private:

    void hideEvent(QHideEvent* event) override;

    int choice;
    QString filePath;
    QLabel* fileBrowseLabel;

    void setImportPlaylist();
    void setLabel();

    void saveSettings();
};

#endif // SETTINGSWINDOW_H
