#ifndef PREPAREWORK_H
#define PREPAREWORK_H

#include <QDialog>

namespace Ui {
class PrepareWork;
}

class PrepareWork : public QDialog
{
    Q_OBJECT

public:
    explicit PrepareWork(QWidget *parent = 0);
    ~PrepareWork();

private:
    Ui::PrepareWork *ui;
};

#endif // PREPAREWORK_H
