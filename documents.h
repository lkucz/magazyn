#ifndef DOCUMENTS_H
#define DOCUMENTS_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class Documents;
}

class Documents : public QDialog
{
    Q_OBJECT

public:
    explicit Documents(QWidget *parent = 0);
    ~Documents();

    void setDB(const QSqlDatabase &db);

private:
    Ui::Documents *ui;

    QSqlDatabase db;
    QSqlTableModel *tableModel;
};

#endif // DOCUMENTS_H
