#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

private slots:
    void setCurrentIndex(int index);
    void closeDialog();

private:
    Ui::MainDialog *ui;
};

#endif // MAINDIALOG_H
