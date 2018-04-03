#include "game.h"
#include "ui_game.h"

#include "game/board.h"
#include "game/piece.h"
#include "socket/client.h"

#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPixmap>

Game::Game(Socket *socket, QWidget *parent) :
    QMainWindow(parent),
    m_socket(socket),
    ui(new Ui::Game),
    m_board(new Board),
    m_chooseIndex(-1),
    m_direction(false)
{   
    ui->setupUi(this);
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    if (dynamic_cast<Client *>(socket)) {
        // Quando for um cliente, aguardar pelo primeiro envio de mensagem do servidor.
        QEventLoop loop;

        auto receiveMessage = [&] (const QString &message) {
            QJsonDocument doc = QJsonDocument::fromJson(message.toLocal8Bit());
            m_board->update(doc.object());

            for (QJsonValue value : doc.object()["board"].toArray())
                m_boardPieces << new Piece(value.toObject()["esq"].toInt(), value.toObject()["dir"].toInt());

            setEnabled(true);

            loop.exit();
        };

        setEnabled(false);

        auto conn = connect(socket, &Socket::messageReceived, receiveMessage);

        loop.exec();

        disconnect(conn);
    }

    connect(m_socket, &Socket::disconnected, this, &Game::lostConnection);
    connect(m_socket, &Socket::messageReceived, this, &Game::playTurn);

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

    connect(ui->comprarPeca, &QPushButton::clicked, this, &Game::purchasePiece);
    connect(ui->passarVez, &QPushButton::clicked, this, &Game::checkPlay);

    repaint();
}

Game::~Game()
{
    delete ui;
}

void Game::lostConnection()
{
    QMessageBox msg;
    msg.setWindowTitle("Conexão perdida");
    msg.setText("Fim de jogo, pois não foi possível manter a comunicação com o outro host.");
    msg.show();
    msg.exec();

    close();
}

void Game::purchasePiece()
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

    repaint();
}

bool Game::eventFilter(QObject *watched, QEvent *event)
{
    if (!ui->comprarPeca->isEnabled() && !ui->passarVez->isEnabled())
        return false;

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
                    // true -> baixo
                    // false -> cima
                    break;
                }
            }

            if (m_chooseIndex != -1) {
                if (m_boardRects.empty()) {
                    m_boardPieces.append(m_pieces[m_chooseIndex]);
                    m_pieces.removeAt(m_chooseIndex);

                    checkPlay();
                }
                else if (m_boardRects.size() == 1 && item == m_boardRects.first()){
                    if ((point.x() - 100) < (m_boardRects.first()->brush().textureImage().height() * 0.25 / 2)) { // esq
                        if (m_direction) {
                            if (m_pieces[m_chooseIndex]->dir() == m_boardPieces.first()->esq()) {
                                m_boardPieces.prepend(m_pieces[m_chooseIndex]);
                                m_pieces.removeAt(m_chooseIndex);

                                checkPlay();
                            }
                            else
                                showErrorMessage();
                        }
                        else {
                            if (m_pieces[m_chooseIndex]->esq() == m_boardPieces.first()->esq()) {
                                m_pieces[m_chooseIndex]->inverter();

                                m_boardPieces.prepend(m_pieces[m_chooseIndex]);
                                m_pieces.removeAt(m_chooseIndex);

                                checkPlay();
                            }
                            else
                                showErrorMessage();
                        }
                    }
                    else { // dir
                        if (m_direction) {
                            if (m_pieces[m_chooseIndex]->dir() == m_boardPieces.first()->dir()) {
                                m_pieces[m_chooseIndex]->inverter();

                                m_boardPieces.append(m_pieces[m_chooseIndex]);
                                m_pieces.removeAt(m_chooseIndex);

                                checkPlay();
                            }
                            else
                                showErrorMessage();
                        }
                        else {
                            if (m_pieces[m_chooseIndex]->esq() == m_boardPieces.first()->dir()) {
                                m_boardPieces.append(m_pieces[m_chooseIndex]);
                                m_pieces.removeAt(m_chooseIndex);

                                checkPlay();
                            }
                            else
                                showErrorMessage();
                        }
                    }
                }
                else {
                    if (item == m_boardRects.first()) {
                        if (m_direction) {
                            if (m_pieces[m_chooseIndex]->dir() == m_boardPieces.first()->esq()) {
                                m_boardPieces.prepend(m_pieces[m_chooseIndex]);
                                m_pieces.removeAt(m_chooseIndex);

                                checkPlay();
                            }
                            else
                                showErrorMessage();
                        }
                        else {
                            if (m_pieces[m_chooseIndex]->esq() == m_boardPieces.first()->esq()) {
                                m_pieces[m_chooseIndex]->inverter();

                                m_boardPieces.prepend(m_pieces[m_chooseIndex]);
                                m_pieces.removeAt(m_chooseIndex);

                                checkPlay();
                            }
                            else
                                showErrorMessage();
                        }
                    }
                    else if (item == m_boardRects.last()){
                        if (m_direction) {
                            if (m_pieces[m_chooseIndex]->dir() == m_boardPieces.last()->dir()) {
                                m_pieces[m_chooseIndex]->inverter();

                                m_boardPieces.append(m_pieces[m_chooseIndex]);
                                m_pieces.removeAt(m_chooseIndex);

                                checkPlay();
                            }
                            else
                                showErrorMessage();
                        }
                        else {
                            if (m_pieces[m_chooseIndex]->esq() == m_boardPieces.last()->dir()) {
                                m_boardPieces.append(m_pieces[m_chooseIndex]);
                                m_pieces.removeAt(m_chooseIndex);

                                checkPlay();
                            }
                            else
                                showErrorMessage();
                        }
                    }
                }
            }
            repaint();
        }
    }

    return QMainWindow::eventFilter(watched, event);
}

void Game::playTurn(const QString &message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toLocal8Bit());
    QJsonObject obj = doc.object();

    m_board->update(obj);

    // TODO: Verificar empate.

    if (obj["win"].toString() == "Y") {
        QMessageBox msg;
        msg.setWindowTitle("Você perdeu!");
        msg.setText("Seu oponente ganhou!");
        msg.show();
        msg.exec();

        close();
    }
    else {
        setEnabled(true);

        m_board->update(obj);

        qDeleteAll(m_boardPieces);
        m_boardPieces.clear();

        for (QJsonValue value : obj["board"].toArray())
            m_boardPieces << new Piece(value.toObject()["esq"].toInt(), value.toObject()["dir"].toInt());

        repaint();
    }
}

void Game::checkPlay()
{
    m_direction = false;
    m_chooseIndex = -1;

    if (m_pieces.empty()) {
        showWinMessage();

        QJsonObject obj;

        obj["win"] = "Y";
        obj["purchaseablePieces"] = m_board->asJson();

        QJsonArray array;

        for (Piece *piece : m_boardPieces)
            array.append(piece->asJson());

        obj["board"] = array;

        QJsonDocument doc;
        doc.setObject(obj);

        m_socket->send(doc.toJson());

        m_socket->close();

        close();
    }
    else {
        QJsonObject obj;
        obj["win"] = "N";
        obj["purchaseablePieces"] = m_board->asJson();

        QJsonArray array;

        for (Piece *piece : m_boardPieces)
            array.append(piece->asJson());

        obj["board"] = array;

        QJsonDocument doc;
        doc.setObject(obj);

        m_socket->send(doc.toJson());

        setEnabled(false);
    }
}

void Game::repaint()
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
                     120 + (brush.textureImage().width() * 0.25 * (i / 10)) + ((i / 10) + 2));

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

    ui->label->setText("Peças p/ comprar: " + QString::number(m_board->purchaseablePiecesCount()));
}

void Game::showErrorMessage()
{
    QMessageBox message;
    message.setWindowTitle("Jogada incorreta");
    message.setText("Não é possível realizar a jogada,"
                    " tente novamente, passe a vez ou compre uma peça.");
    message.show();
    message.exec();
}

void Game::showWinMessage()
{
    QMessageBox message;
    message.setWindowTitle("Você venceu!");
    message.setText("Parabéns, você venceu!");
    message.show();
    message.exec();
}
