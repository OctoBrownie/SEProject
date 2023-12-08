#include "equalizerwindow.h"
//#include "./ui_equalizerwindow.h"
#include "ui_equalizerwindow.h"
#include "equalizerwindow.h"
#include "QVBoxLayout"
#include <QLabel>
#include <Qfile>
#include <QJsonDocument>
#include <QJsonObject>

EqualizerWindow::EqualizerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EqualizerWindow)
{
    ui->setupUi(this);

    //Create slider & reset button data types to use in functions
    QSlider *sliderLow = ui->sliderMedium;
    QSlider *sliderMedium = ui->sliderHigh;
    QSlider *sliderHigh = ui->sliderLow;
    QPushButton *resetButton = ui->resetButton;

    //Connect each slider & reset button with the corresponding function
    connect(sliderLow, &QSlider::valueChanged, this, &EqualizerWindow::sliderChange);
    connect(sliderMedium, &QSlider::valueChanged, this, &EqualizerWindow::sliderChange);
    connect(sliderHigh, &QSlider::valueChanged, this, &EqualizerWindow::sliderChange);
    connect(resetButton, &QPushButton::clicked, this, &EqualizerWindow::resetFrequencies);

    //Replace this with the file we will have for the equalizer settings
    setSettings("filePathToSettings.json");

}

// Read Equalizer settings from a file to set as default settings when window is opened
void EqualizerWindow::setSettings(const QString &pathFile) {

    //turn given file into a QFile and then trying to open the file
    QFile file(pathFile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in (&file);
        QString jsonText = in.readAll();
        file.close();

        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonText.toUtf8());

        QJsonObject jsonObject = jsonDocument.object();

        int lowFrequency = jsonObject["LowFrequency"].toInt();
        int mediumFrequency = jsonObject["MediumFrequency"].toInt();
        int highFrequency = jsonObject["HighFrequency"].toInt();

        //Adjust sliders to given values from settings file
        ui->sliderLow->setValue(lowFrequency);
        ui->sliderMedium->setValue(mediumFrequency);
        ui->sliderHigh->setValue(highFrequency);

        //Adjust frequencies to given values from settings file
        adjustFrequencies(lowFrequency, mediumFrequency, highFrequency);

        //For Debugging Purposes
        qDebug() << "Settings loaded: " << pathFile;
    } else {
        qWarning() << "Unable to open settings file: " << pathFile;
    }
}

// Change slider values when user moves a slider
void EqualizerWindow::sliderChange() {

    //Set values to what the sliders are at (user changed)
    int lowValue = ui->sliderLow->value();
    int mediumValue = ui->sliderMedium->value();
    int highValue = ui->sliderHigh->value();

    //Turn values into type double
    double normLow= static_cast<double>(lowValue);
    double normMedium = static_cast<double>(mediumValue);
    double normHigh = static_cast<double>(highValue);

    // Adjust frequencies based on slider values
    adjustFrequencies(normLow, normMedium, normHigh);
}

// Update labels to show the values for the changed frequencies
void EqualizerWindow::adjustFrequencies(double low, double medium, double high) {

    //Substitute audioSystem with the actual system that we will use to control audio

    //audioSystem.setLowFrequency(low);
    //audioSystem.setMediumFrequency(medium);
    //audioSystem.setHighFrequency(high);

    // Update labels with the corresponding frequencies
    ui->labelLow->setText(QString("Low Frequency: %1").arg(low));
    ui->labelMedium->setText(QString("Medium Frequency: %1").arg(medium));
    ui->labelHigh->setText(QString("High Frequency: %1").arg(high));
}

// Set Frequencies to default values if user clicks reset button
void EqualizerWindow::resetFrequencies() {

    //Reset value
    const int resetValue = 50;

    //Set slider values to default value of 50
    ui->sliderMedium->setValue(resetValue);
    ui->sliderHigh->setValue(resetValue);
    ui->sliderLow->setValue(resetValue);

    //Change value into type double
    double normLow = static_cast<double>(resetValue);
    double normMedium = static_cast<double>(resetValue);
    double normHigh = static_cast<double>(resetValue);

    // Adjust frequencies based on slider values
    adjustFrequencies(normLow, normMedium, normHigh);
}

//save settings from equalizer into a file for future refrences
void EqualizerWindow::saveSettings(const QString &pathFile) {

    //Create JSON object
    QJsonObject jsonSettings;

    //Save frequencies values into the JSON object
    jsonSettings["LowFreqency"] = ui->sliderLow->value();
    jsonSettings["MediumFrequency"] = ui->sliderMedium->value();
    jsonSettings["HighFrequency"] = ui->sliderHigh->value();

    //Create JSON document
    QJsonDocument jsonDocument(jsonSettings);

    //Turn file into Qfile and try and open it to write to it
    QFile file(pathFile);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out (&file);
        out <<jsonDocument.toJson();
        file.close();

        qDebug() << "Settings saved to file:" << file.fileName();
    }
}

EqualizerWindow::~EqualizerWindow()
{
    delete ui;
}

