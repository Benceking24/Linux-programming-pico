#include "pico.h"
#include "ui_pico.h"

PiCo::PiCo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PiCo)
{
    ui->setupUi(this);
    running = false;
    timer = new QTimer(this);

    lux = 0;
    ledBright = 0;
    luxMeasurementMin=luxMax;
    luxMeasurementMax=0;
    luxMeasurementAvg=0;
    luxMeasurementSum=0;
    luxMeasurementCount=0;
    tableType=0;

    frequency = freqDEF;

    ui->spinFreq->setValue(freqDEF);
    ui->spinFreq->setMinimum(freqMIN);
    ui->spinFreq->setMaximum(freqMAX);

    wiringPiSetup();
    pinMode (1, PWM_OUTPUT);

    char *filename = (char*)"/dev/i2c-1";
        if ((file_i2c = open(filename, O_RDWR)) < 0)
        {        
            qDebug() << "Failed to open the i2c bus.";
        }

        int addr = 0x23;         
        if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
        {
            qDebug() << "Failed to acquire bus access and/or talk to slave.";
        }


            //----- WRITE BYTES -----
        buffer[0] = 0x10;
        buffer[1] = 0x02;
        length = 1;			
        if (write(file_i2c, buffer, length) != length)		
        {
            qDebug() << "Failed to write to the i2c bus.";
        }
}

PiCo::~PiCo()
{
    delete ui;
}


void PiCo::on_pushButton_clicked()
{
    if(!running){
        running = true;
        ui->pushButton->setText("Stop\nmeasurement");
        connect(timer, SIGNAL(timeout()),this,SLOT(readmeasurement()));
        timer->start(frequency);
    }else if(running){
        running = false;
        ui->pushButton->setText("Start\nmeasurement");
        timer->stop();
        disconnect(timer, SIGNAL(timeout()),this,SLOT(readmeasurement()));
    }
}

void PiCo::readmeasurement()
{

    ssize_t length = 2;			
if (read(file_i2c, buffer, length) != length)		
{
    qDebug() << "Failed to read from the i2c bus.";
}
else
{
    lux = buffer[1]+buffer[0]*256;
    ui->lcdLux->display(lux);
    int percentage = (int)((100.0/luxMax)*lux);
    ui->progLux->setValue(percentage);

    if(lux<luxMeasurementMin)luxMeasurementMin=lux;
    if(lux>luxMeasurementMax)luxMeasurementMax=lux;
    luxMeasurementCount++;
    luxMeasurementSum+=lux;
    luxMeasurementAvg=luxMeasurementSum/luxMeasurementCount;
    drawTable();
}



}

void PiCo::on_spinFreq_valueChanged(int arg1)
{
    if(arg1>=freqMIN && arg1 <= freqMAX)
    {
        frequency = arg1;
        if(running) timer->start(frequency);
    }
    else{
        qDebug()  << "Frequency out of range";
    }
}

void PiCo::on_ledOn_clicked()
{
    ledBright = 100;
changeLed();
}

void PiCo::on_ledOff_clicked()
{
    ledBright = 0;
changeLed();
}

void PiCo::on_ledAlt1_clicked()
{
    ledBright = 30;
changeLed();
}

void PiCo::on_ledAlt2_clicked()
{
    ledBright = 70;
changeLed();
}

void PiCo::on_scrollLed_sliderMoved(int position)
{
    ledBright = position;
    changeLed();
}

void PiCo::changeLed()
{
    pwmWrite(1,ledBright);
    ui->lcdLed->display(ledBright);
    ui->scrollLed->setValue(ledBright);
}

void PiCo::drawTable()
{
    double multiplier = 0;
    int precision = 0;
    std::string type;

    if(!tableType)
    {
        precision = 0;
        multiplier = 1;
        type = "lux";
    } else
    {
        precision = 1;
        multiplier = 100.0/luxMax;
        type = "%";
    }
    ui->luxMinVal->setText(QString("%1 %2").arg(luxMeasurementMin*multiplier, 0, 'f', precision).arg(type.c_str()));
    ui->luxMaxVal->setText(QString("%1 %2").arg(luxMeasurementMax*multiplier, 0, 'f', precision).arg(type.c_str()));
    ui->luxAvgVal->setText(QString("%1 %2").arg(luxMeasurementAvg*multiplier, 0, 'f', precision).arg(type.c_str()));
}

void PiCo::on_radioLux_clicked()
{
    tableType = 0;
    drawTable();
}

void PiCo::on_radioPerc_clicked()
{
    tableType = 1;
    drawTable();
}

void PiCo::on_pushButton_2_clicked()
{
    luxMeasurementMin=luxMax;
    luxMeasurementMax=0;
    luxMeasurementAvg=0;
    luxMeasurementSum=0;
    luxMeasurementCount=0;
    drawTable();
}
