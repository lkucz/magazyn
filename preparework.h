#ifndef PREPAREWORK_H
#define PREPAREWORK_H

#include <QStandardItemModel>
#include <QDialog>
#include <QSqlTableModel>
#include <QString>
#include <QModelIndex>
#include <QList>
#include "productlist.h"

namespace Ui {
class PrepareWork;
}

class PrepareWork : public QDialog
{
    Q_OBJECT

public:
    explicit PrepareWork(QWidget *parent = 0);
    ~PrepareWork();

    void setDB(const QSqlDatabase &db);
    void show();

private:
    Ui::PrepareWork *ui;

    QSqlTableModel *productList;
    QSqlDatabase db;
    QStandardItemModel *tableModel;
    QList<QModelIndexList> products;

    ProductList *productListWindow;

private slots:
    void addProduct();
    void deleteProduct();
    void dataSelected(QModelIndexList );
    void accept();
    void reject();

};

#endif // PREPAREWORK_H