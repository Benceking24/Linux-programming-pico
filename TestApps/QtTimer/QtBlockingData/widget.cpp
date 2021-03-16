#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <string>



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    timer = new QTimer(this);
    ui->setupUi(this);
    char *filename = (char*)"/dev/i2c-1";
        if ((file_i2c = open(filename, O_RDWR)) < 0)
        {
            //ERROR HANDLING: you can check errno to see what went wrong
            qDebug() << "Failed to open the i2c bus.";
        }

        int addr = 0x23;          //<<<<<The I2C address of the slave
        if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
        {
            qDebug() << "Failed to acquire bus access and/or talk to slave.";
            //ERROR HANDLING; you can check errno to see what went wrong
        }


            //----- WRITE BYTES -----
        buffer[0] = 0x10;
        buffer[1] = 0x02;
        length = 1;			//<<< Number of bytes to write
        if (write(file_i2c, buffer, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no res     onse from the device)
        {
            /* ERROR HANDLING: i2c transaction failed */
            qDebug() << "Failed to write to the i2c bus.";
        }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::tick()
{
    length = 2;			//<<< Number of bytes to read
if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
{
    //ERROR HANDLING: i2c transaction failed
    qDebug() << "Failed to read from the i2c bus.";
}
else
{
    int lux = buffer[1]+buffer[0]*256;
    double percentage = (100.0/65535.0)*lux;
  qDebug() << lux;
QString str = QString("%1 lux (%2%)").arg(lux).arg(percentage);
ui->label->setText(str);
}

}


void Widget::on_pushButton_clicked()
{
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->start(50);
}
