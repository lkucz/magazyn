#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <QDialog>
#include <QSqlTableModel>
#include "work.h"

namespace Ui {
class Production;
}

class Production : public QDialog
{
    Q_OBJECT

public:
    explicit Production(QWidget *parent = 0);
    ~Production();

    void setDB(const QSqlDatabase &db);
    void show();

private:
    Ui::Production *ui;

    QSqlDatabase db;
    QSqlTableModel *tableModel;

    Work *workWindow;

private slots:
    void accept();
    void reject();
    void on_tableView_doubleClicked(const QModelIndex &index);
};

#endif // PRODUCTION_H
