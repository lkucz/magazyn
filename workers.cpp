#include <QSqlTableModel>
#include <QSqlRecord>
#include <QDebug>
#include <QMessageBox>
#include <QUuid>
#include <QString>
#include <QModelIndex>
#include <iostream>
#include "workers.h"
#include "ui_workers.h"

Workers::Workers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Workers)
{
    ui->setupUi(this);

    tm = 0;     //Zeruj wskaznik do obiektów
    worker = 0;
}

Workers::~Workers()
{
    delete ui;

    if(tm) delete tm;   //Usun zaalokowane obiekty
    if(worker) delete worker;
}

void Workers::setDB(const QSqlDatabase &db)
{
    if(tm) delete tm;           //Usun wskaznik jezeli zostal wczesniej zainnicjalizowany
    tm = new QSqlTableModel(0, db);
    this->db = db;
}

void Workers::setTable(const QString &tableName)
{
    if(tm)
    {
        this->tableName = tableName;

        tm->setTable(tableName);
        tm->setEditStrategy(QSqlTableModel::OnManualSubmit);
        tm->setSort(1, Qt::AscendingOrder);
        tm->select();
    }
}

void Workers::setWindowTitle(const QString &title)
{
    windowTitle = title;
    QDialog::setWindowTitle(windowTitle);
}

void Workers::show()
{
    if(tm)
    {
        tm->select();               //Wykonaj zapytanie przed otwqarciem okna
        ui->tableView->setModel(tm);
    }
    QDialog::show();
}

void Workers::on_addButton_clicked()
{
    if(!worker){
        worker = new Worker(this);
        worker->setDB(this->db);
        worker->setTable("worker");
    }

    worker->show();
}

void Workers::on_deleteButton_clicked()
{
    QMessageBox mb;
    QModelIndexList list;

    list = ui->tableView->selectionModel()->selectedIndexes(); //Pobierz listę zaznaczonych elementow

    if( list.empty() ) return; //Brak zaznaczenia

    mb.setText("Czy napewno chcesz usunąć zaznaczone rekordy?");
    mb.setIcon(QMessageBox::Question);
    mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    mb.setDefaultButton(QMessageBox::No);

    if( mb.exec() == QMessageBox::Yes )
    {
        foreach (const QModelIndex &index, list) {
            tm->removeRow(index.row());
        }
    }

    tm->submitAll();
    tm->select();
}

void Workers::on_closeButton_clicked()
{
    this->hide();
}

void Workers::on_clearFilterButton_clicked()
{
    ui->filterEdit->clear();
}

void Workers::on_filterEdit_textChanged(const QString &filter)
{
    tm->setFilter("name like '" + filter + "%'");
    tm->select();
}

void Workers::on_tableView_doubleClicked(const QModelIndex &index)
{

}
