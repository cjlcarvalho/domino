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
    void purshasePiece();

private:
    bool eventFilter(QObject *watched, QEvent *event);
    void repaintHand();

private:
    Ui::MainWindow *ui;
    QList<Piece *> m_pieces;
    Board *m_board;
    QList<QGraphicsRectItem *> m_handRects;
    QList<Piece *> m_boardPieces;
    QList<QGraphicsRectItem *> m_boardRects;

    int m_boardWidthCount;

};

#endif // MAINWINDOW_H
