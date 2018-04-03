#ifndef BOARD_H
#define BOARD_H

#include <QJsonObject>
#include <QList>

class Piece;

class Board
{
public:
    Board();
    Board(const QJsonObject &object);
    void update(const QJsonObject &object);
    Piece *purchasePiece();
    bool isPurchaseablePiecesEmpty() const;
    unsigned int purchaseablePiecesCount() const;
    const QJsonArray asJson() const;

private:
    void init();

private:
    QList<Piece *> m_purchaseablePieces;
};

#endif // BOARD_H
