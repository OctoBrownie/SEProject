#include "import_buttons.h"
#include "ui_import_buttons.h"
#include <QFileDialog>
#include <QPushButton>
#include <QLabel>
#include <QDir>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPushButton *file = ui->fileImport;
    QPushButton *playlist = ui->playlistImport;

    QLabel *myLabel = ui->label;

    connect(ui->fileImport, &QPushButton::clicked, this, &MainWindow::select_file);
    selectedFilePath = "";

    connect(ui->playlistImport, &QPushButton::clicked, this, &MainWindow::select_folder);

}

void MainWindow::select_file()
{

    QFileDialog dialog(this);
    dialog.setNameFilter("MP3 Files (*.mp3);;All Files (*.*);;Text Files (*.txt);;XML Files (*.xml)");

    // Set the dialog to select a single file
    dialog.setFileMode(QFileDialog::ExistingFile);

    // Open the dialog and get the selected file path
    QString selectedFilePath = dialog.getOpenFileName(this, "Select an MP3 file", QString(), "MP3 Files (*.mp3)");

    if (selectedFilePath.isEmpty()) {
        // The selectedFilePath is empty, so open the file dialog
        QString filter = "MP3 Files (*.mp3);;All Files (*.*);;Text Files (*.txt);;XML Files (*.xml)";

        // Check if the selected file has the ".mp3" extension
        if (!selectedFilePath.endsWith(".mp3", Qt::CaseInsensitive)) {
            // The user selected a file that is not an MP3 file; display an error message
            QMessageBox::critical(this, "Error", "Please select an MP3 file.");
            selectedFilePath.clear();  // Clear the selectedFilePath
            return;  // Do not proceed further
        }
    }

    // The user selected a valid MP3 file or previously selected one; proceed with the import logic
    ui->label->setText("Selected MP3 File: " + selectedFilePath);
    //return selectedFilePath;
}

void MainWindow::select_folder() {

    //Make it a function later
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::List);
    dialog.setNameFilter("MP3 Files (*.mp3)");
    dialog.setWindowTitle("Select Folders with  MP3 Files");

    dialog.setOption(QFileDialog::ShowDirsOnly, true);

    //Single Folder
    QString selectedFolder = dialog.getExistingDirectory(this, "Select Folder", QString(), QFileDialog::ReadOnly);

    //Multiple Folders
    //QStringList selectedFolders = dialog.getExistingDirectories(this, "Select Folders", QString(), QFileDialog::ReadOnly);

    if(selectedFolder.isEmpty()){
        return;
    }

    //QString selectedFolderString = selectedFolder.join(", ");

    //Check if all files in folder are of MP3 type
    if (!folder_validation(selectedFolder)){
        QMessageBox::critical(this, "Error", "The selected folder is invalid. Try again");
        return;
    }

    ui->label->setText("Selected Folder: " + selectedFolder);
}

bool MainWindow::folder_validation(const QString& folder) {

    QDir dir(folder);

    if (!dir.exists()) {
        qDebug() << "Folder does not exist: " << folder;
        return false;
    }

    QStringList allFiles = dir.entryList(QDir::Files);

    foreach (const QString& mp3File, allFiles)
    {
        if (!mp3File.toLower().endsWith(".mp3"))
        {
            qDebug() << "Non-MP3 File Found: " << mp3File;
            return false;
        }
    }

    return true;
}


MainWindow::~MainWindow()
{
    delete ui;
}


