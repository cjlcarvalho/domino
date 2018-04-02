#include "piece.h"

Piece::Piece(int esq, int dir) :
    m_esq(esq),
    m_dir(dir)
{

}

Piece::Piece(const QJsonObject &object) :
    m_esq(object["esq"].toInt()),
    m_dir(object["dir"].toInt())
{

}

int Piece::esq() const
{
    return m_esq;
}

int Piece::dir() const
{
    return m_dir;
}

void Piece::inverter()
{
    int _ = m_esq;
    m_esq = m_dir;
    m_dir = _;
}

const QJsonObject Piece::asJson() const
{
    QJsonObject object;

    object["esq"] = m_esq;
    object["dir"] = m_dir;

    return object;
}
