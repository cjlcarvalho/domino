#include "board.h"
#include "piece.h"

#include <cstdlib>
#include <QDebug>
#include <QJsonArray>
#include <QtGlobal>

using namespace std;

Board::Board()
{
    init();
}

Board::Board(const QJsonObject &object)
{
    update(object);
}

void Board::init()
{
    for (int i = 0; i < 7; i++)
        for (int j = 0; j <= i; j++)
            m_purchaseablePieces.append(new Piece(i, j));
}

void Board::update(const QJsonObject &object)
{
    qDeleteAll(m_purchaseablePieces);
    m_purchaseablePieces.clear();

    if (object.empty())
        return;

    QJsonArray array = object["purchaseablePieces"].toArray();

    for (const QJsonValue &value : array)
        m_purchaseablePieces << new Piece(value.toObject());
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

unsigned int Board::purchaseablePiecesCount() const
{
    return m_purchaseablePieces.size();
}

const QJsonArray Board::asJson() const
{
    QJsonArray purchaseablePieces;

    for (Piece *piece : m_purchaseablePieces)
        purchaseablePieces.append(piece->asJson());

    return purchaseablePieces;
}
