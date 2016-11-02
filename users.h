#ifndef USERS_H
#define USERS_H

#include <QDialog>

namespace Ui {
class Users;
}

class Users : public QDialog
{
    Q_OBJECT

public:
    explicit Users(QWidget *parent = 0);
    ~Users();

private:
    Ui::Users *ui;
};

#endif // USERS_H
