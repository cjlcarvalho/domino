#include "board.h"
#include "piece.h"

#include <iostream>
#include <QDebug>
#include <QtGlobal>

using namespace std;

Board::Board()
{
    init();
}

void Board::init()
{
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            m_purchaseablePieces.append(new Piece(i, j));
}

std::deque<Piece *> Board::pieces() const
{
    return m_pieces;
}

Piece *Board::purchasePiece()
{
    if (m_purchaseablePieces.empty())
        return nullptr;

    srand(time(NULL));

    int random = rand() % m_purchaseablePieces.size();

    Piece *piece = m_purchaseablePieces[random];
    m_purchaseablePieces.removeAll(piece);

    return piece;
}

bool Board::isPurchaseablePiecesEmpty() const
{
    return m_purchaseablePieces.empty();
}

void Board::addPiece(Piece *piece)
{
    if (m_pieces.empty())
        m_pieces.push_back(piece);
    else {
        if (m_pieces.front()->esq() == piece->dir())
            m_pieces.push_front(piece);
        else if (m_pieces.back()->dir() == piece->esq())
            m_pieces.push_back(piece);
    }
}

void Board::printBoard() const
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
