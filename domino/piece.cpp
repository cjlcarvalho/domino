#include "piece.h"

Piece::Piece(int esq, int dir) :
    m_esq(esq),
    m_dir(dir)
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
