#ifndef PRODUCT_H
#define PRODUCT_H

#include <QDialog>
#include <QSqlTableModel>
#include <QString>

namespace Ui {
class Product;
}

class Product : public QDialog
{
    Q_OBJECT

public:
    explicit Product(QWidget *parent = 0);
    ~Product();

    void setDB(const QSqlDatabase &db);
    void show();

signals:
    void dataChanged();

private slots:
    void accept();
    void reject();

private:
    Ui::Product *ui;

    QSqlTableModel *nameTableModel;
    QSqlTableModel *typeTableModel;
    QSqlTableModel *materialTableModel;
    QSqlTableModel *finishTableModel;
    QSqlTableModel *colorTableModel;
    QSqlTableModel *unitTableModel;
    QSqlTableModel *productTableModel;
};

#endif // PRODUCT_H
