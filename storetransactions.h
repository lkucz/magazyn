#ifndef STORETRANSACTIONS_H
#define STORETRANSACTIONS_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class StoreTransactions;
}

class StoreTransactions : public QDialog
{
    Q_OBJECT

public:
    explicit StoreTransactions(QWidget *parent = 0);
    ~StoreTransactions();

    void setDB(const QSqlDatabase &db);

private:
    Ui::StoreTransactions *ui;

    QSqlDatabase db;
    QSqlTableModel *tableModel;
};

#endif // STORETRANSACTIONS_H
