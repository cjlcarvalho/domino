#include "game.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Game *game = new Game;
    game->start();

    return a.exec();
}
