#ifndef GAME_H
#define GAME_H

#include "socket/socket.h"

#include <QGraphicsRectItem>
#include <QList>
#include <QMainWindow>

namespace Ui {
class Game;
}

class Piece;
class Board;

class Game : public QMainWindow
{
    Q_OBJECT

public:
    explicit Game(Socket *socket, QWidget *parent = 0);
    ~Game();

private slots:
    void purchasePiece();
    void playTurn(const QString &message);
    void checkPlay();
    void pass();
    void lostConnection();

private:
    bool eventFilter(QObject *watched, QEvent *event);
    void repaint();
    void showErrorMessage();
    void showWinMessage();

private:
    Socket *m_socket;
    Ui::Game *ui;
    QList<Piece *> m_pieces;
    Board *m_board;
    QList<QGraphicsRectItem *> m_handRects;
    QList<Piece *> m_boardPieces;
    QList<QGraphicsRectItem *> m_boardRects;

    int m_chooseIndex;
    bool m_direction;
    bool m_pendingDraw;
};

#endif // GAME_H
