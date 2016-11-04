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
    explicit ProductList(QWidget *parent = 0);
    ~ProductList();

    void setDB(const QSqlDatabase &db);
    void setTable(const QString &tableName);
    void setWindowTitle(const QString &title);
    void show();

private slots:
    void on_addButton_clicked();

private:
    Ui::ProductList *ui;
    Product *product;

    QString tableName;
    QString windowTitle;

    QSqlTableModel *tm;

};

#endif // PRODUCTLIST_H
