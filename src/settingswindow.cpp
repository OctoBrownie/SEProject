#include "settingswindow.h"

SettingsWindow::SettingsWindow(): QMainWindow()
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QLabel* settingsTitle = new QLabel("Settings");
    QFont font;
    font.setPointSize(20);
    settingsTitle->setFont(font);
    mainLayout->addWidget(settingsTitle);

    QVBoxLayout* radioButtonOptions = new QVBoxLayout();
    QRadioButton* defaultOption = new QRadioButton("Create New Playlist on Open");
    QRadioButton* loadOption = new QRadioButton("Open Current Playlist on Open");
    radioButtonOptions->addWidget(defaultOption);
    radioButtonOptions->addWidget(loadOption);
    defaultOption->setChecked(true);

    mainLayout->addLayout(radioButtonOptions);

    QVBoxLayout* masterVolume = new QVBoxLayout();
    QLabel* volumeName = new QLabel("Volume");
    QFont vFont;
    vFont.setPointSize(30);
    volumeName->setFont(vFont);
    QSlider* volumeSlider = new QSlider(Qt::Horizontal);
    masterVolume->addWidget(volumeName);
    masterVolume->addWidget(volumeSlider);

    mainLayout->addLayout(masterVolume);

    QWidget* mainOutput = new QWidget();
    mainOutput->setLayout(mainLayout);

    this->setCentralWidget(mainOutput);
    this->resize(800, 500);
}
