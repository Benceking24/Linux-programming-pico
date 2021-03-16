#include "pico.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PiCo w;
    w.show();
    return a.exec();
}
