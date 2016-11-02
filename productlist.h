#ifndef PRODUCTLIST_H
#define PRODUCTLIST_H

#include <QDialog>
#include <QSqlTableModel>
#include <QString>

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

private:
    Ui::ProductList *ui;
    QSqlTableModel *tm;
    QString tableName;
    QString windowTitle;
};

#endif // PRODUCTLIST_H
