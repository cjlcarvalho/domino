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
    md.show();

    return a.exec();
}
