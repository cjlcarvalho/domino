#include "playerhand.h"
#include "piece.h"

#include <iostream>

using namespace std;

PlayerHand::PlayerHand()
{
}

QList<Piece *> PlayerHand::pieces() const
{
    return m_pieces;
}

void PlayerHand::addPiece(Piece *piece)
{
    m_pieces << piece;
}

bool PlayerHand::isEmpty() const
{
    return m_pieces.isEmpty();
}

Piece *PlayerHand::choosePiece(int esq, int dir)
{
    for (Piece *piece : m_pieces) {
        if (piece->esq() == esq && piece->dir() == dir) {
            m_pieces.removeAll(piece);
            return piece;
        }
    }
    return nullptr;
}

void PlayerHand::printHand() const
{
    int cont = 1;

    for (Piece *piece : m_pieces) {
        cout << piece->esq() << ":" << piece->dir() << " | ";
        if (!(cont % 8))
            cout << endl;
        cont++;
    }
    cout << endl;
}
