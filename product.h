#ifndef PRODUCT_H
#define PRODUCT_H

#include <QDialog>

namespace Ui {
class Product;
}

class Product : public QDialog
{
    Q_OBJECT

public:
    explicit Product(QWidget *parent = 0);
    ~Product();

private:
    Ui::Product *ui;
};

#endif // PRODUCT_H
