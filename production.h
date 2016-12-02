#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <QDialog>
#include <QSqlDatabase>

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

private slots:
    void accept();
    void reject();
};

#endif // PRODUCTION_H
