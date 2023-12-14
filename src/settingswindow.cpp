#include "settingswindow.h"
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QFileDialog>

SettingsWindow::SettingsWindow(int option, QString filePath): QMainWindow()
{
    this->choice = option;
    this->filePath = filePath;
    this->fileBrowseLabel = new QLabel();
    this->setLabel();


    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setAlignment(Qt::AlignTop);
    QLabel* settingsTitle = new QLabel("Settings");
    QFont font;
    font.setPointSize(20);
    settingsTitle->setFont(font);
    mainLayout->addWidget(settingsTitle);

    QSpacerItem* spacer = new QSpacerItem(50, 50);
    mainLayout->addSpacerItem(spacer);

    QVBoxLayout* radioButtonOptions = new QVBoxLayout();


    QRadioButton* defaultOption = new QRadioButton("Create New Playlist on Open");
    connect(defaultOption, &QRadioButton::clicked, [=] {
        this->choice = 0;
    });

    QRadioButton* loadOption = new QRadioButton("Open Chosen Playlist on Open");
    connect(loadOption, &QRadioButton::clicked, [=] {
        this->choice = 1;
    });


    radioButtonOptions->addWidget(defaultOption);
    radioButtonOptions->addWidget(loadOption);

    if (choice == 0) {
        defaultOption->setChecked(true);
    } else {
        loadOption->setChecked(true);
    }

    mainLayout->addLayout(radioButtonOptions);


    QWidget* searchBox = new QWidget();
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->setAlignment(Qt::AlignLeft);
    QPushButton* browseButton = new QPushButton("Search");

    connect(browseButton, &QPushButton::clicked, this, &SettingsWindow::setImportPlaylist);

    searchLayout->addWidget(this->fileBrowseLabel);
    searchLayout->addWidget(browseButton);

    searchBox->setLayout(searchLayout);

    mainLayout->addWidget(searchBox);

    QWidget* mainOutput = new QWidget();
    mainOutput->setLayout(mainLayout);

    this->setCentralWidget(mainOutput);
    this->resize(800, 500);
}





void SettingsWindow::setImportPlaylist() {
    QMainWindow* newWindow = new QMainWindow();
    QFileDialog dialog;
    dialog.setNameFilter("Playlist Files (*.pa)");
    dialog.setFileMode(QFileDialog::Directory);

    QString selectedFilePath = dialog.getOpenFileName(newWindow, "Selected a Playlist file", QString(), "Playlist Files (*.pa)");

    if (!selectedFilePath.isEmpty()) {
        this->filePath = selectedFilePath;
        this->setLabel();
    }
}



void SettingsWindow::setLabel() {
    if (this->filePath.isEmpty()) {
        this->fileBrowseLabel->setText("No file selected");
    } else {
        this->fileBrowseLabel->setText(this->filePath);
    }
}


void SettingsWindow::hideEvent(QHideEvent* event) {
    this->saveSettings();
}


void SettingsWindow::saveSettings() {
    QFile file("./config/settings.json");

    QJsonObject jsonSettings;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in (&file);
        QString jsonText = in.readAll();

        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonText.toUtf8());

        jsonSettings = jsonDocument.object();

        file.close();
    } else {
        return;
    }


    //Save frequencies values into the JSON object
    jsonSettings["Option"] = this->choice;
    jsonSettings["Filename"] = this->filePath;

    QJsonDocument jsonOutDocument(jsonSettings);

    //Turn file into Qfile and try and open it to write to it
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(jsonOutDocument.toJson());
        file.close();
    } else {
        return;
    }
}
