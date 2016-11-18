#ifndef WORKER_H
#define WORKER_H

#include <QDialog>
#include <QSqlTableModel>
#include <QString>

namespace Ui {
class Worker;
}

class Worker : public QDialog
{
    Q_OBJECT

public:
    explicit Worker(QWidget *parent = 0);
    ~Worker();

    void setDB(const QSqlDatabase &db);
    void setTable(const QString &tableName);
    void setWindowTitle(const QString &title);

private slots:
    void accept();
    void reject();

private:
    Ui::Worker *ui;

    QSqlTableModel *tm;
    QString windowTitle;
    QString tableName;
};

#endif // WORKER_H
