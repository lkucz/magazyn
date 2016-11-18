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

private slots:
    void accept();
    void reject();

private:
    Ui::Product *ui;
    QString windowTitle;

    QSqlTableModel *nameTableModel;
    QSqlTableModel *typeTableModel;
    QSqlTableModel *materialTableModel;
    QSqlTableModel *finishTableModel;
    QSqlTableModel *colorTableModel;

    QSqlTableModel *productTableModel;
};

#endif // PRODUCT_H
