#ifndef CLIENTFORM_H
#define CLIENTFORM_H

#include <QWidget>

namespace Ui {
class ClientForm;
}

class ClientForm : public QWidget
{
    Q_OBJECT

public:
    explicit ClientForm(QWidget *parent = 0);
    ~ClientForm();

signals:
    void gameStarted();

public slots:
    void start();

private:
    void connectionErrorMessage(const QString &erro = QString());

private:
    Ui::ClientForm *ui;
};

#endif // CLIENTFORM_H
