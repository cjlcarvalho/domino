#include "servidorform.h"
#include "ui_servidorform.h"

ServidorForm::ServidorForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServidorForm)
{
    ui->setupUi(this);
}

ServidorForm::~ServidorForm()
{
    delete ui;
}
