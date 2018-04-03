#include "maindialog.h"
#include "ui_maindialog.h"

#include "clientform.h"
#include "initialform.h"
#include "servidorform.h"

#include <QDialogButtonBox>
#include <QPushButton>

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    setWindowTitle("Super Dominó");

    InitialForm *initial = new InitialForm(this);
    ServidorForm *servidor = new ServidorForm(this);
    ClientForm *cliente = new ClientForm(this);

    ui->stackedWidget->addWidget(initial);
    ui->stackedWidget->addWidget(servidor);
    ui->stackedWidget->addWidget(cliente);

    connect(initial, &InitialForm::changeStackedWidgetIndex, this, &MainDialog::setCurrentIndex);
    connect(initial, &InitialForm::gameStarted, this, &MainDialog::closeDialog);
    connect(cliente, &ClientForm::gameStarted, this, &MainDialog::closeDialog);

    ui->stackedWidget->setCurrentIndex(2);
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::setCurrentIndex(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

void MainDialog::closeDialog()
{
    close();
}
