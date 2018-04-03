#ifndef SERVIDORFORM_H
#define SERVIDORFORM_H

#include <QWidget>

namespace Ui {
class ServidorForm;
}

class ServidorForm : public QWidget
{
    Q_OBJECT

public:
    explicit ServidorForm(QWidget *parent = 0);
    ~ServidorForm();

private:
    Ui::ServidorForm *ui;
};

#endif // SERVIDORFORM_H
