#ifndef WORK_H
#define WORK_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class Work;
}

class Work : public QDialog
{
    Q_OBJECT

public:
    explicit Work(QWidget *parent = 0);
    ~Work();

    void setDB(const QSqlDatabase &db);
    void show();

private:
    Ui::Work *ui;

    QSqlDatabase db;
    QSqlTableModel *tableModel;
};

#endif // WORK_H
