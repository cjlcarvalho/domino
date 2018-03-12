#ifndef GAME_H
#define GAME_H

class Board;
class PlayerHand;

class Game
{
public:
    Game();
    void start();

private:
    bool playTurn();

private:
    bool m_turn;
    Board *m_board;
    PlayerHand *m_player1;
    PlayerHand *m_player2;
};

#endif // GAME_H
