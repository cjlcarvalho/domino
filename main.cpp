#include "ui/maindialog.h"

#include <QApplication>
#include <QDebug>
#include <QIcon>
#include <QPixmap>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(QPixmap(":/images/domino.png")));

    MainDialog md;
    md.exec();

    return a.exec();
}
