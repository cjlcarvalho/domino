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

// TODO: Criar dialog para selecionar entre cliente e servidor.

// TODO: Criar comunicação entre cliente e servidor. O servidor sempre jogará primeiro.

// TODO: Criar transmissão de json entre cliente e servidor.

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    // TODO: Iniciar board apenas caso for servidor.
    m_board(new Board),
    m_chooseIndex(-1),
    m_direction(false)
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

    for (int i = 0; i < 7; i++)
        m_pieces << m_board->purchasePiece();

    ui->label->setText("Peças p/ comprar: " + QString::number(m_board->purchaseablePiecesCount()));

    connect(ui->comprarPeca, &QPushButton::clicked, this, &MainWindow::purchasePiece);

    repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::purchasePiece()
{
    Piece *piece = m_board->purchasePiece();

    if (!piece) {
        QMessageBox message;
        message.setWindowTitle("Não é possível comprar!");
        message.setText("Não é mais possível comprar peças.");
        message.exec();
    }
    else
        m_pieces << piece;

    m_chooseIndex = -1;
    m_direction = false;

    ui->label->setText("Peças p/ comprar: " + QString::number(m_board->purchaseablePiecesCount()));

    repaint();
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
                    m_chooseIndex = i;
                    m_direction = (point.y() - 500 > ((m_handRects[i]->brush().textureImage().height() * 0.25) / 2));
                    // true -> baixo // false -> cima
                    break;
                }
            }

            if (m_chooseIndex != -1) {
                if (m_boardRects.empty()) {
                    m_boardPieces << m_pieces[m_chooseIndex];

                    m_pieces.removeAt(m_chooseIndex);

                    m_direction = false;
                    m_chooseIndex = -1;
                }
                else if (m_boardRects.size() == 1 && item == m_boardRects.first()){
                    // ver se a parte clicada do primeiro item é a esquerda ou direita

                    if ((point.x() - 100) < (m_boardRects.first()->brush().textureImage().height() * 0.25 / 2)) { // esq
                        if (m_direction) {
                            if (m_pieces[m_chooseIndex]->dir() == m_boardPieces.first()->esq()) {
                                m_boardPieces.prepend(m_pieces[m_chooseIndex]);
                                m_pieces.removeAt(m_chooseIndex);

                                m_direction = false;
                                m_chooseIndex = -1;
                            }
                            else {
                                showErrorMessage();
                            }
                        }
                        else {
                            if (m_pieces[m_chooseIndex]->esq() == m_boardPieces.first()->esq()) {
                                m_pieces[m_chooseIndex]->inverter();

                                m_boardPieces.prepend(m_pieces[m_chooseIndex]);
                                m_pieces.removeAt(m_chooseIndex);

                                m_direction = false;
                                m_chooseIndex = -1;
                            }
                            else {
                                showErrorMessage();
                            }
                        }
                    }
                    else { // dir
                        if (m_direction) {
                            if (m_pieces[m_chooseIndex]->dir() == m_boardPieces.first()->dir()) {
                                m_pieces[m_chooseIndex]->inverter();

                                m_boardPieces.append(m_pieces[m_chooseIndex]);
                                m_pieces.removeAt(m_chooseIndex);

                                m_direction = false;
                                m_chooseIndex = -1;
                            }
                            else {
                                showErrorMessage();
                            }
                        }
                        else {
                            if (m_pieces[m_chooseIndex]->esq() == m_boardPieces.first()->dir()) {
                                m_boardPieces.append(m_pieces[m_chooseIndex]);
                                m_pieces.removeAt(m_chooseIndex);

                                m_direction = false;
                                m_chooseIndex = -1;
                            }
                            else {
                                showErrorMessage();
                            }
                        }
                    }
                }
                else {
                    if (item == m_boardRects.first()) {
                        if (m_direction) {
                            // comparar a direita da peça escolhida com a esquerda do início
                            if (m_pieces[m_chooseIndex]->dir() == m_boardPieces.first()->esq()) {
                                m_boardPieces.prepend(m_pieces[m_chooseIndex]);
                                m_pieces.removeAt(m_chooseIndex);

                                m_direction = false;
                                m_chooseIndex = -1;
                            }
                            else {
                                showErrorMessage();
                            }
                        }
                        else {
                            // comparar o lado de cima da peça escolhida com a esquerda do início
                            if (m_pieces[m_chooseIndex]->esq() == m_boardPieces.first()->esq()) {
                                m_pieces[m_chooseIndex]->inverter();

                                m_boardPieces.prepend(m_pieces[m_chooseIndex]);
                                m_pieces.removeAt(m_chooseIndex);

                                m_direction = false;
                                m_chooseIndex = -1;
                            }
                            else {
                                showErrorMessage();
                            }
                        }
                    }
                    else if (item == m_boardRects.last()){
                        if (m_direction) {
                            if (m_pieces[m_chooseIndex]->dir() == m_boardPieces.last()->dir()) {
                                m_pieces[m_chooseIndex]->inverter();

                                m_boardPieces.append(m_pieces[m_chooseIndex]);
                                m_pieces.removeAt(m_chooseIndex);

                                m_direction = false;
                                m_chooseIndex = -1;
                            }
                            else {
                                showErrorMessage();
                            }
                        }
                        else {
                            if (m_pieces[m_chooseIndex]->esq() == m_boardPieces.last()->dir()) {
                                m_boardPieces.append(m_pieces[m_chooseIndex]);
                                m_pieces.removeAt(m_chooseIndex);

                                m_direction = false;
                                m_chooseIndex = -1;
                            }
                            else {
                                showErrorMessage();
                            }
                        }
                    }
                }
            }
            repaint();

            if (m_pieces.empty()) {
                showWinMessage();

                // TODO: enviar mensagem que venceu o jogo

                exit(0);
            }
            else {
                // TODO: enviar atualização da mesa para o oponente
            }
        }
    }

    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::repaint()
{
    for (QGraphicsRectItem *item : m_handRects)
        ui->graphicsView->scene()->removeItem(item);
    
    for (QGraphicsRectItem *item : m_boardRects)
        ui->graphicsView->scene()->removeItem(item);

    qDeleteAll(m_handRects);
    qDeleteAll(m_boardRects);

    m_handRects.clear();
    m_boardRects.clear();

    int boardWidthCount = 0;

    for (int i = 0; i < m_boardPieces.size(); i++) {
        QGraphicsRectItem *rect = new QGraphicsRectItem(0, 0, 159, 318);
        QBrush brush(QPixmap(":/images/pieces/" + QString::number(m_boardPieces[i]->esq()) +
                              "x" + QString::number(m_boardPieces[i]->dir()) + ".png"));

        QTransform transform;
        transform.rotate(-90);
        rect->setBrush(brush);
        rect->setTransform(transform);

        rect->setPen(QPen(Qt::NoPen));

        if (i % 10 == 0)
            boardWidthCount = 0;

        rect->setPos(20 + (boardWidthCount * (brush.textureImage().height() * 0.25)) + 2 * boardWidthCount,
                     150 + (brush.textureImage().width() * 0.25 * (i / 10)) + ((i / 10) + 2));

        boardWidthCount++;

        rect->setScale(0.25);

        ui->graphicsView->scene()->addItem(rect);

        m_boardRects << rect;
    }

    for (int i = 0; i < m_pieces.size(); i++) {
        QGraphicsRectItem *rect = new QGraphicsRectItem(0, 0, 159, 318);
        QBrush brush(QPixmap(":/images/pieces/" + QString::number(m_pieces[i]->esq()) + "x" +
                             QString::number(m_pieces[i]->dir()) + ".png"));
        rect->setBrush(brush);

        if (m_chooseIndex == i)
            rect->setPen(QPen(QBrush(Qt::red), 9));
        else
            rect->setPen(QPen(Qt::NoPen));

        rect->setPos(120 + (i * (brush.textureImage().width() * 0.25) + 2*i), 500);
        rect->setScale(0.25);

        ui->graphicsView->scene()->addItem(rect);

        m_handRects << rect;
    }
}

void MainWindow::showErrorMessage()
{
    QMessageBox message;
    message.setWindowTitle("Jogada incorreta");
    message.setText("Não é possível realizar a jogada,"
                    " tente novamente, passe a vez ou compre uma peça.");
    message.show();
    message.exec();
}

void MainWindow::showWinMessage()
{
    QMessageBox message;
    message.setWindowTitle("Você venceu!");
    message.setText("Parabéns, você venceu!");
    message.show();
    message.exec();
}
