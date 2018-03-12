#ifndef PLAYERHAND_H
#define PLAYERHAND_H

#include <QList>
#include <QObject>

class Piece;

class PlayerHand
{
public:
    PlayerHand();
    QList<Piece *> pieces() const;
    void addPiece(Piece *piece);
    bool isEmpty() const;
    void printHand() const;
    Piece *choosePiece(int esq, int dir);

private:
    QList<Piece *> m_pieces;
};

#endif // PLAYERHAND_H
