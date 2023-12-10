#include "equalizerwindow.h"
#include "ui_equalizerwindow.h"
#include "equalizer.h"

#include <QVBoxLayout>
#include <QLabel>
#include <Qfile>
#include <QJsonDocument>
#include <QJsonObject>

#define EQWINDOW_DEFAULT_SLIDER_VAL 50
#define EQWINDOW_SLIDER_MAX 100

EqualizerWindow::EqualizerWindow(int highgain, int midgain, int lowgain, QWidget *parent)
	: QMainWindow(parent), ui(new Ui::EqualizerWindow), equalizer{new Equalizer(2, highgain, midgain, lowgain)}
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
    this->setSettings(highgain, midgain, lowgain);

}

/*
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
}*/

void EqualizerWindow::setSettings(int highgain, int midgain, int lowgain) {
    ui->sliderLow->setValue(lowgain);
    ui->sliderMedium->setValue(midgain);
    ui->sliderHigh->setValue(highgain);

    adjustFrequencies(lowgain, midgain, highgain);
}

// Change slider values when user moves a slider
void EqualizerWindow::sliderChange() {
	adjustFrequencies(ui->sliderLow->value(), ui->sliderMedium->value(), ui->sliderHigh->value());
}

// Update labels to show the values for the changed frequencies
void EqualizerWindow::adjustFrequencies(double low, double medium, double high) {
	this->equalizer->setLowMult(low*EQUALIZER_MAX_MULT/EQWINDOW_SLIDER_MAX);
	this->equalizer->setMidMult(medium*EQUALIZER_MAX_MULT/EQWINDOW_SLIDER_MAX);
	this->equalizer->setHighMult(high*EQUALIZER_MAX_MULT/EQWINDOW_SLIDER_MAX);

    // Update labels with the corresponding frequencies
    ui->labelLow->setText(QString("Low Frequency: %1").arg(low));
    ui->labelMedium->setText(QString("Medium Frequency: %1").arg(medium));
    ui->labelHigh->setText(QString("High Frequency: %1").arg(high));
}

// Set Frequencies to default values if user clicks reset button
void EqualizerWindow::resetFrequencies() {
	//Set slider values to default value of 50
	ui->sliderMedium->setValue(EQWINDOW_DEFAULT_SLIDER_VAL);
	ui->sliderHigh->setValue(EQWINDOW_DEFAULT_SLIDER_VAL);
	ui->sliderLow->setValue(EQWINDOW_DEFAULT_SLIDER_VAL);

	// Adjust frequencies based on slider values
	adjustFrequencies(EQWINDOW_DEFAULT_SLIDER_VAL, EQWINDOW_DEFAULT_SLIDER_VAL, EQWINDOW_DEFAULT_SLIDER_VAL);
}

//save settings from equalizer into a file for future refrences
void EqualizerWindow::saveSettings() {

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
    jsonSettings["LowFrequency"] = ui->sliderLow->value();
    jsonSettings["MediumFrequency"] = ui->sliderMedium->value();
    jsonSettings["HighFrequency"] = ui->sliderHigh->value();

    //Create JSON document
    QJsonDocument jsonDocument(jsonSettings);

    //Turn file into Qfile and try and open it to write to it
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(jsonDocument.toJson());
        file.close();
    } else {
        return;
    }
}

void EqualizerWindow::hideEvent(QHideEvent* event) {
    saveSettings();
}

EqualizerWindow::~EqualizerWindow()
{
    delete ui;
}

