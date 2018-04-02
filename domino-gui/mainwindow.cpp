#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "board.h"
#include "piece.h"

#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPixmap>

/* TODO: Se ele clicar em uma peça da mão, setar como selecionada.
  Guardar a referência para a selecionada e pintá-la de vermelho no repaintHand
  Se clicar em outra peça da mão, atualizar selecionada.
  É possível casar a selecionada com o início ou fim da mesa.
  Verificar se o lado selecionado da peça é igual a esquerda do início ou a direita do fim. Se sim, encaixe.
  Ou seja, além da referência, guarde o local selecionado da peça. */

/* TODO: Criar QDialog para selecionar se será cliente ou servidor.
 * O servidor irá iniciar sempre (ignore as regras convencionais nesse caso)
*/

/* TODO: Jogar com 6! peças, como no jogo comum */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_board(new Board),
    m_boardWidthCount(0)
{
    ui->setupUi(this);
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    setWindowTitle("Super Dominó");

    ui->graphicsView->installEventFilter(this);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    scene->addPixmap(QPixmap(":/images/board.jpg"))->setScale(0.53);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setMaximumSize(1000, 800);
    ui->graphicsView->adjustSize();

    for (int i = 0; i < 7; i++) {
        m_pieces << m_board->purchasePiece();

        QGraphicsRectItem *rect = scene->addRect(0, 0, 159, 318);

        QBrush brush(QPixmap(":/images/pieces/" + QString::number(m_pieces[i]->esq()) +
                             "x" + QString::number(m_pieces[i]->dir()) + ".png"));
        rect->setBrush(brush);
        rect->setPen(QPen(QBrush(Qt::red), 4));
        rect->setPos(120 + (i * (brush.textureImage().width() * 0.25) + 2*i), 500);
        rect->setScale(0.25);

        m_handRects << rect;
    }

    connect(ui->comprarPeca, &QPushButton::clicked, this, &MainWindow::purshasePiece);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::purshasePiece()
{
    Piece *piece = m_board->purchasePiece();

    if (!piece) {
        QMessageBox message;
        message.setWindowTitle("Não é possível comprar!");
        message.setText("Não é mais possível comprar peças.");
        message.exec();
    }
    else {
        m_pieces << piece;

        QGraphicsRectItem *rect = new QGraphicsRectItem(0, 0, 159, 318);
        QBrush brush(QPixmap(":/images/pieces/" + QString::number(m_pieces[m_pieces.size() - 1]->esq()) +
                             "x" + QString::number(m_pieces[m_pieces.size() - 1]->dir()) + ".png"));
        rect->setBrush(brush);
        rect->setPen(QPen(Qt::NoPen));
        rect->setPos(120 + ((m_pieces.size() - 1) * (brush.textureImage().width() * 0.25) + 2*(m_pieces.size() - 1)), 500);
        rect->setScale(0.25);
        ui->graphicsView->scene()->addItem(rect);

        m_handRects << rect;
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    QMouseEvent *mEvent = dynamic_cast<QMouseEvent *>(event);

    if (mEvent) {
        QPoint point = mEvent->pos();
        QPointF scenePoint = ui->graphicsView->mapToScene(point);
        QGraphicsItem *item = ui->graphicsView->scene()->itemAt(scenePoint, QTransform());

        if (item) {
            for (int i = 0; i < m_handRects.size(); i++) {
                if (m_handRects[i] == item) {
                    if (m_boardPieces.empty()) {
                        QGraphicsRectItem *rect = new QGraphicsRectItem(0, 0, 159, 318);
                        QBrush brush(QPixmap(":/images/pieces/" + QString::number(m_pieces[i]->esq()) +
                                              "x" + QString::number(m_pieces[i]->dir()) + ".png"));

                        QTransform transform;
                        transform.rotate(-90);
                        rect->setBrush(brush);
                        rect->setTransform(transform);

                        rect->setPen(QPen(Qt::NoPen));

                        if (m_boardPieces.size() % 10 == 0)
                            m_boardWidthCount = 0;

                        rect->setPos(20 + (m_boardWidthCount * (brush.textureImage().height() * 0.25)) +
                                     2 * m_boardWidthCount, 150 + (brush.textureImage().width() * 0.25 *
                                                                            (m_boardPieces.size() / 10)) +
                                                                    ((m_boardPieces.size() / 10) + 2));

                        m_boardWidthCount++;

                        rect->setScale(0.25);

                        m_boardPieces << m_pieces[i];
                        m_boardRects << m_handRects[i];

                        ui->graphicsView->scene()->removeItem(m_handRects[i]);

                        m_handRects.removeAt(i);
                        m_pieces.removeAt(i);

                        ui->graphicsView->scene()->addItem(rect);

                        break;
                    }
                    else if (point.y() - 500 >  // baixo
                            ((m_handRects[i]->brush().textureImage().height() * 0.25) / 2)) {
                        if (m_pieces[i]->dir() != m_boardPieces.last()->dir()) {
                            QMessageBox message;
                            message.setWindowTitle("Jogada incorreta");
                            message.setText("Não é possível realizar a jogada, "
                                            "tente novamente, passe a vez ou compre uma peça.");
                            message.exec();
                        }
                        else {
                            QGraphicsRectItem *rect = new QGraphicsRectItem(0, 0, 159, 318);
                            QBrush brush(QPixmap(":/images/pieces/" + QString::number(m_pieces[i]->esq()) +
                                                  "x" + QString::number(m_pieces[i]->dir()) + ".png"));

                            QTransform transform;
                            transform.rotate(90);
                            rect->setBrush(brush);
                            rect->setTransform(transform);

                            rect->setPen(QPen(Qt::NoPen));

                            if (m_boardPieces.size() % 10 == 0)
                                m_boardWidthCount = 0;

                            rect->setPos(100 + (m_boardWidthCount * (brush.textureImage().height() * 0.25)) +
                                         2 * m_boardWidthCount, 110 + (brush.textureImage().width() * 0.25 *
                                                                                (m_boardPieces.size() / 10)) +
                                                                        ((m_boardPieces.size() / 10) + 2));

                            m_boardWidthCount++;

                            rect->setScale(0.25);

                            m_pieces[i]->inverter();

                            m_boardPieces << m_pieces[i];
                            m_boardRects << m_handRects[i];

                            ui->graphicsView->scene()->removeItem(m_handRects[i]);

                            m_handRects.removeAt(i);
                            m_pieces.removeAt(i);

                            ui->graphicsView->scene()->addItem(rect);

                            break;
                        }
                    }
                    else {
                        if (m_pieces[i]->esq() != m_boardPieces.last()->dir()) {
                            QMessageBox message;
                            message.setWindowTitle("Jogada incorreta");
                            message.setText("Não é possível realizar a jogada,"
                                            " tente novamente, passe a vez ou compre uma peça.");
                            message.exec();
                        }
                        else {
                            QGraphicsRectItem *rect = new QGraphicsRectItem(0, 0, 159, 318);
                            QBrush brush(QPixmap(":/images/pieces/" + QString::number(m_pieces[i]->esq()) +
                                                  "x" + QString::number(m_pieces[i]->dir()) + ".png"));

                            QTransform transform;
                            transform.rotate(-90);
                            rect->setBrush(brush);
                            rect->setTransform(transform);

                            rect->setPen(QPen(Qt::NoPen));

                            if (m_boardPieces.size() % 10 == 0)
                                m_boardWidthCount = 0;

                            rect->setPos(20 + (m_boardWidthCount * (brush.textureImage().height() * 0.25)) +
                                         2 * m_boardWidthCount, 150 + (brush.textureImage().width() * 0.25 *
                                                                                (m_boardPieces.size() / 10)) +
                                                                        ((m_boardPieces.size() / 10) + 2));

                            m_boardWidthCount++;

                            rect->setScale(0.25);

                            m_boardPieces << m_pieces[i];
                            m_boardRects << m_handRects[i];

                            ui->graphicsView->scene()->removeItem(m_handRects[i]);

                            m_handRects.removeAt(i);
                            m_pieces.removeAt(i);

                            ui->graphicsView->scene()->addItem(rect);

                            break;
                        }
                    }
                }
            }
            repaintHand();
        }
    }

    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::repaintHand()
{
    for (QGraphicsRectItem *item : m_handRects) {
        ui->graphicsView->scene()->removeItem(item);
        delete item;
    }
    m_handRects.clear();

    for (int i = 0; i < m_pieces.size(); i++) {
        QGraphicsRectItem *rect = new QGraphicsRectItem(0, 0, 159, 318);
        QBrush brush(QPixmap(":/images/pieces/" + QString::number(m_pieces[i]->esq()) + "x" +
                             QString::number(m_pieces[i]->dir()) + ".png"));
        rect->setBrush(brush);
        rect->setPen(QPen(Qt::NoPen));
        rect->setPos(120 + (i * (brush.textureImage().width() * 0.25) + 2*i), 500);
        rect->setScale(0.25);

        ui->graphicsView->scene()->addItem(rect);

        m_handRects << rect;
    }
}
