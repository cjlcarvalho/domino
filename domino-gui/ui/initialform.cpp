#include "initialform.h"
#include "ui_initialform.h"

#include "game.h"
#include "socket/socket.h"
#include "socket/server.h"
#include "socket/client.h"

#include "socket/settings.h"

InitialForm::InitialForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InitialForm)
{
    ui->setupUi(this);

    connect(ui->servidor, &QPushButton::clicked, this, &InitialForm::startAsServer);
    connect(ui->cliente, &QPushButton::clicked, this, &InitialForm::startAsClient);
}

InitialForm::~InitialForm()
{
    delete ui;
}

void InitialForm::startAsClient()
{
    emit changeStackedWidgetIndex(4);
}

void InitialForm::startAsServer()
{
    emit changeStackedWidgetIndex(3);

    Socket *socket = new Server(PORT);

    QEventLoop loop;

    auto conn = connect(socket, &Socket::connected, [&](){ loop.exit(); });

    loop.exec();

    disconnect(conn);

    close();

    Game *g = new Game(socket);
    g->show();

    emit gameStarted();
}
