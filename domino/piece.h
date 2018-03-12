#ifndef PIECE_H
#define PIECE_H

class Piece
{
public:
    Piece(int esq, int dir);
    int esq() const;
    int dir() const;

private:
    int m_esq;
    int m_dir;
};

#endif // PIECE_H
