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
    void setWorkID(QString workID);
    void setWorkerID(int workerID);
    void show();

private:
    Ui::Work *ui;

    QSqlDatabase db;
    QSqlTableModel *workerTableModel;
    QSqlTableModel *workDetailsTableModel;

    QString workID;          //numer pracy
    int workerID;           //pracownik ustawiony początkowo w ComboBox
    bool workerChanged;     //zmienna logiczna służąca do zaznaczenia, że nastąpiła zmiana pracownika
    bool workChanged;       //zmianne logiczna służąca do zaznaczenia, że nastąpiła zmiana półproduktów

private slots:
    void addProduct();
    void deleteProduct();
    void accept();
    void reject();
    void on_worker_currentIndexChanged(int index);
};

#endif // WORK_H
