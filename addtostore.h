#ifndef ADDTOSTORE_H
#define ADDTOSTORE_H

#include <QStandardItemModel>
#include <QDialog>
#include <QSqlTableModel>
#include <QString>
#include <QModelIndex>
#include <QList>
#include "settings.h"
#include "productlist.h"

namespace Ui {
class AddToStore;
}

class AddToStore : public QDialog
{
    Q_OBJECT

public:
    explicit AddToStore(QWidget *parent = 0);
    ~AddToStore();

    void setDB(const QSqlDatabase &db);
    void show();

private:
    Ui::AddToStore *ui;

    QSqlDatabase db;
    QStandardItemModel *productListTableModel;
    QSqlTableModel *docTypeTableModel;
    QSqlTableModel *documentTableModel;
    QSqlTableModel *storeTableModel;
    QList<QModelIndexList> products;

    ProductList *productListWindow;

private slots:
    void addProduct();
    void deleteProduct();
    void dataSelected(QModelIndexList );
    void accept();
    void reject();
};

#endif // ADDTOSTORE_H
