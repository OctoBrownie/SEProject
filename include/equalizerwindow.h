#ifndef EQUALIZERWINDOW_H
#define EQUALIZERWINDOW_H

#include <QMainWindow>
#include <QSlider>

QT_BEGIN_NAMESPACE
namespace Ui { class EqualizerWindow; }
QT_END_NAMESPACE

class EqualizerWindow : public QMainWindow
{
    Q_OBJECT

public:
    EqualizerWindow(int highgain, int midgain, int lowgain, QWidget *parent = nullptr);
    ~EqualizerWindow();

private:
    //void setupUi();
    //QSlider* createSlider(const QString &label, QWidget *parent);
    void setSettings(int highgain, int midgain, int lowgain);

    void sliderChange();
    void adjustFrequencies(double low, double medium, double high);
    void resetFrequencies();

    void saveSettings();

    Ui::EqualizerWindow *ui;

    void hideEvent(QHideEvent* event) override;


    /*QSlider *sliderHigh;
    QSlider *sliderMedium;
    QSlider *sliderLow;*/
};
#endif // EQUALIZERWINDOW_H
