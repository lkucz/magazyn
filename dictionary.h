#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QDialog>
#include <QSqlTableModel>
#include <QString>

namespace Ui {
class Dictionary;
}

class Dictionary : public QDialog
{
    Q_OBJECT

public:
    explicit Dictionary(QWidget *parent = 0);
    ~Dictionary();

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

private:
    Ui::Dictionary *ui;
    QSqlTableModel *tm;
    QString tableName;
    QString windowTitle;
};

#endif // DICTIONARY_H
