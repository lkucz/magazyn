#ifndef STORELIST_H
#define STORELIST_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class StoreList;
}

class StoreList : public QDialog
{
    Q_OBJECT

public:

    explicit StoreList(QWidget *parent = 0);
    ~StoreList();

    void setDB(const QSqlDatabase &db);
    void show();

signals:
    void dataSelected(QModelIndexList list);

private:
    Ui::StoreList *ui;

    QSqlDatabase db;
    QSqlTableModel *tableModel;

private slots:
    void accept();
    void reject();
    void on_tableView_doubleClicked(const QModelIndex &index);
};

#endif // STORELIST_H
