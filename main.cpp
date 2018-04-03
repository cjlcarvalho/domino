#include "ui/maindialog.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainDialog md;
    md.exec();

    return a.exec();
}
