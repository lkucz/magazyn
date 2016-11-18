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
    void on_deleteButton_clicked();

private:
    Ui::ProductList *ui;

    QSqlTableModel *tm;
    QString tableName;
    QString windowTitle;
    QSqlDatabase db;

    Product *productWindow;
};

#endif // PRODUCTLIST_H
