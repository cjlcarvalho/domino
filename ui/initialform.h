#ifndef INITIALFORM_H
#define INITIALFORM_H

#include <QWidget>

namespace Ui {
class InitialForm;
}

class InitialForm : public QWidget
{
    Q_OBJECT

public:
    explicit InitialForm(QWidget *parent = 0);
    ~InitialForm();

signals:
    void changeStackedWidgetIndex(int index);
    void gameStarted();

private slots:
    void startAsClient();
    void startAsServer();

private:
    Ui::InitialForm *ui;
};

#endif // INITIALFORM_H
