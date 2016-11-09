#ifndef USERS_H
#define USERS_H

#include <QDialog>
#include <QSqlTableModel>
#include <QString>

namespace Ui {
class Users;
}

class Users : public QDialog
{
    Q_OBJECT

public:
    explicit Users(QWidget *parent = 0);
    ~Users();

    void setDB(const QSqlDatabase &db);
    void setTable(const QString &tableName);
    void setWindowTitle(const QString &title);
    void show();

private slots:
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_closeButton_clicked();
    void on_clearFilterButton_clicked();
    void on_filterEdit_textChanged(const QString &);
    void on_listView_doubleClicked(const QModelIndex &);

private:
    Ui::Users *ui;
    QSqlTableModel *tm;
    QString tableName;
    QString windowTitle;
};

#endif // USERS_H
