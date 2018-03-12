#include "game.h"
#include "board.h"
#include "piece.h"
#include "playerhand.h"

#include <iostream>
#include <string>
#include <QtGlobal>

using namespace std;

Game::Game() :
    m_turn(false),
    m_board(new Board),
    m_player1(new PlayerHand),
    m_player2(new PlayerHand)
{
    int player1choice;

    cout << "Player 1 cara ou coroa [0 para cara, 1 para coroa]: ";
    cin >> player1choice;

    int random = qrand();

    if (random % 2 && !player1choice) // se for ímpar, é cara
        m_turn = false;
    else if (random % 2 == 0 && player1choice) // se for par, é coroa
        m_turn = false;
    else
        m_turn = true;
}

void Game::start()
{
    for (int i = 0; i < 7; i++) {
        m_player1->addPiece(m_board->purchasePiece());
        m_player2->addPiece(m_board->purchasePiece());
    }

    while (playTurn()) { }
}

bool Game::playTurn()
{
    m_board->printBoard();
    if (!m_turn) {
        cout << "Player 1 turn: " << endl;
        while (true) {
            m_player1->printHand();
            cout << "Buy piece? [Y-N] ";
            char op;
            cin >> op;
            if (op == 'Y')
                m_player1->addPiece(m_board->purchasePiece());
            else
                break;
        }
        int pEsq, pDir;
        cout << "Choose piece numbers: [esq-dir] ";
        cin >> pEsq >> pDir;
        Piece *piece = m_player1->choosePiece(pEsq, pDir);
        if (piece)
            m_board->addPiece(piece);

        if (m_player1->isEmpty()) {
            cout << "Player 1 is the winner!" << endl;
            return false;
        }

        // ver o caso em que os dois não podem mais jogar, e a contagem de pontos é realizada

        if (m_board->isPurchaseablePiecesEmpty()) {
            if (m_player1->pieces().size() <= m_player2->pieces().size()) {
                cout << "Player 1 is the winner!" << endl;
                return false;
            }
            else {
                cout << "Player 2 is the winner!" << endl;
                return false;
            }
        }

    }
    else {
        cout << "Player 2 turn: " << endl;

        while (true) {
            m_player2->printHand();
            cout << "Buy piece? [Y-N] ";
            char op;
            cin >> op;
            if (op == 'Y')
                m_player2->addPiece(m_board->purchasePiece());
            else
                break;
        }
        int pEsq, pDir;
        cout << "Choose piece numbers: [esq-dir] ";
        cin >> pEsq >> pDir;
        Piece *piece = m_player2->choosePiece(pEsq, pDir);
        if (piece)
            m_board->addPiece(piece);

        if (m_player2->isEmpty()) {
            cout << "Player 2 is the winner!" << endl;
            return false;
        }

        // ver o caso em que os dois não podem mais jogar, e a contagem de pontos é realizada

        if (m_board->isPurchaseablePiecesEmpty()) {
            if (m_player2->pieces().size() <= m_player1->pieces().size()) {
                cout << "Player 2 is the winner!" << endl;
                return false;
            }
            else {
                cout << "Player 1 is the winner!" << endl;
                return false;
            }
        }
    }
    m_turn = !m_turn;
    return true;
}
