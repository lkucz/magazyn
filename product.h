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
    void setWindowTitle(const QString &title);
    void show();

private:
    Ui::Product *ui;
    QSqlTableModel *tm;
    QString tableName;
    QString windowTitle;
};

#endif // PRODUCT_H
