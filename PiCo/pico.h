#ifndef PICO_H
#define PICO_H

#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <wiringPi.h>

#define freqMIN 1
#define freqMAX 1000
#define freqDEF 5
#define luxMax 65536
QT_BEGIN_NAMESPACE
namespace Ui { class PiCo; }
QT_END_NAMESPACE

class PiCo : public QWidget
{
    Q_OBJECT

public:
    PiCo(QWidget *parent = nullptr);
    ~PiCo();

private slots:
    void on_pushButton_clicked();
    void readmeasurement();
    void on_spinFreq_valueChanged(int arg1);
    void on_ledOn_clicked();
    void on_ledOff_clicked();
    void on_ledAlt1_clicked();
    void on_ledAlt2_clicked();
    void on_scrollLed_sliderMoved(int position);
    void on_radioLux_clicked();
    void on_radioPerc_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::PiCo *ui;
    QTimer *timer;
    bool running;
    int frequency; //Frequency of sensor readings (freqMin - freqMax)
    int lux;       //Sensor readout (0 - luxMax)
    int ledBright; //LED brightness percentage (0 - 100)
    int luxMeasurementMin;
    int luxMeasurementMax;
    int luxMeasurementAvg;
    int luxMeasurementSum;
    int luxMeasurementCount;
    int tableType; //0=lux; 1=percentage(%)
    int file_i2c;
    ssize_t length;
    unsigned char buffer[60];
    void changeLed();
    void drawTable();
};
#endif // PICO_H
