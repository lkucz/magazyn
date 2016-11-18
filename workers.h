#ifndef WORKERS_H
#define WORKERS_H

#include <QDialog>
#include <QSqlTableModel>
#include <QString>
#include "worker.h"

namespace Ui {
class Workers;
}

class Workers : public QDialog
{
    Q_OBJECT

public:
    explicit Workers(QWidget *parent = 0);
    ~Workers();

    void setDB(const QSqlDatabase &db);
    void setTable(const QString &tableName);
    void setWindowTitle(const QString &title);
    void show();

private slots:
    void on_filterEdit_textChanged(const QString &arg1);
    void on_clearFilterButton_clicked();
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_closeButton_clicked();
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::Workers *ui;

    QSqlTableModel *tm;
    QString tableName;
    QString windowTitle;
    QSqlDatabase db;

    Worker *workerWindow;
};

#endif // WORKERS_H
