#ifndef PIECE_H
#define PIECE_H

#include <QJsonObject>

class Piece
{
public:
    Piece(int esq, int dir);
    Piece(const QJsonObject &object);
    int esq() const;
    int dir() const;
    void inverter();
    const QJsonObject asJson() const;

private:
    int m_esq;
    int m_dir;
};

#endif // PIECE_H
