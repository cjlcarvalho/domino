#ifndef BOARD_H
#define BOARD_H

#include <deque>
#include <QList>

class Piece;

class Board
{
public:
    Board();
    void init();
    std::deque<Piece *> pieces() const;
    Piece *purchasePiece();
    bool isPurchaseablePiecesEmpty() const;
    void addPiece(Piece *piece);
    void printBoard() const;

private:
    std::deque<Piece *> m_pieces;
    QList<Piece *> m_purchaseablePieces;
};

#endif // BOARD_H
