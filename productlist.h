#ifndef PRODUCTLIST_H
#define PRODUCTLIST_H

#include <QDialog>
#include <QSqlTableModel>
#include <QString>
#include "product.h"

namespace Ui {
class ProductList;
}

class ProductList : public QDialog
{
    Q_OBJECT

public:

    enum ProductListFlags
    {
        Select = 1,
    };

    explicit ProductList(QWidget *parent = 0);
    ~ProductList();

    void setDB(const QSqlDatabase &db);
    void show();
    void setFlags(unsigned int flags);
    unsigned int getFlags();

signals:
    void dataSelected(QModelIndexList list);

private slots:
    void refreshTable();
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_tableView_doubleClicked(const QModelIndex &index);

    void accept();
    void reject();


private:
    Ui::ProductList *ui;

    QSqlTableModel *tm;
    QSqlTableModel *productTableModel;
    QSqlDatabase db;

    unsigned int flags;
    Product *productWindow;
};

#endif // PRODUCTLIST_H
