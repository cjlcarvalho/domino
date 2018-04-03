#include "clientform.h"
#include "ui_clientform.h"

#include <QMessageBox>

#include "game.h"
#include "socket/socket.h"
#include "socket/client.h"
#include "socket/settings.h"

ClientForm::ClientForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientForm)
{
    ui->setupUi(this);

    ui->jogada->setVisible(false);

    connect(ui->conectar, &QPushButton::clicked, this, &ClientForm::start);
}

ClientForm::~ClientForm()
{
    delete ui;
}

void ClientForm::start()
{
    if (ui->lineEdit->text() == QString())
        connectionErrorMessage();
    else {
        Socket *client = new Client(ui->lineEdit->text(), PORT);

        QEventLoop loop;

        auto exitError = [&] (const QString &errorName) {
            loop.exit();
            
            ui->jogada->setVisible(false);
            ui->label->setVisible(true);
            ui->conectar->setVisible(true);
            ui->lineEdit->setVisible(true);
            
            connectionErrorMessage(errorName);
        };

        auto connError = connect(client, &Socket::error, exitError);

        auto exitSuccess = [&] () {
            loop.exit();

            connect(client, &Socket::messageReceived, [=] () {
                close();
                emit gameStarted();
            });

            Game *g = new Game(client);
            g->show();
        };

        auto connSuccess = connect(client, &Socket::connected, exitSuccess);

        ui->jogada->setVisible(true);
        ui->label->setVisible(false);
        ui->conectar->setVisible(false);
        ui->lineEdit->setVisible(false);

        loop.exec();

        disconnect(connError);
        disconnect(connSuccess);
    }
}

void ClientForm::connectionErrorMessage(const QString &erro)
{
    QMessageBox message;
    message.setWindowTitle("Erro de conexão!");
    message.setText((!erro.size()) ? "Por favor, preencha com um IP correto." : erro);
    message.show();
    message.exec();
}
