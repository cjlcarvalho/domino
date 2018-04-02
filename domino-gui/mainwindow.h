#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsRectItem>
#include <QList>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class Piece;
class Board;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void purchasePiece();

private:
    bool eventFilter(QObject *watched, QEvent *event);
    void repaint();
    void showErrorMessage();
    void showWinMessage();

private:
    Ui::MainWindow *ui;
    QList<Piece *> m_pieces;
    Board *m_board;
    QList<QGraphicsRectItem *> m_handRects;
    QList<Piece *> m_boardPieces;
    QList<QGraphicsRectItem *> m_boardRects;

    int m_chooseIndex;
    bool m_direction;
};

#endif // MAINWINDOW_H
