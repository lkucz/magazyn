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
    tm = 0;     //Zeruj wskaznik do modelu danych
}

Workers::~Workers()
{
    delete ui;
    if(tm) delete tm;   //Usun model danych
}

void Workers::setDB(const QSqlDatabase &db)
{
    if(tm) delete tm;           //Usun wskaznik jezeli zostal wczesniej zainnicjalizowany
    tm = new QSqlTableModel(0, db);
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
    //    int rowNumber = -1;
    //    QSqlRecord newRecord;
    //    QUuid tmpName = QUuid::createUuid();

    //    newRecord = tm->record();
    //    newRecord.setGenerated("id", true);
    //    newRecord.setValue("name", QVariant(tmpName.toString()));

    //    if(tm->insertRecord(-1, newRecord) != true)
    //    {
    //        qDebug() << "Nie moge dodac rekordu do bazy danych: tabela(" << tableName << ")";

    //        QMessageBox mb;
    //        mb.setText("Nie moge dodac nowego rekordu do bazy danych " + tableName);
    //        mb.exec();
    //        mb.setIcon(QMessageBox::Critical);
    //    }
    //    else
    //    {
    //        tm->setFilter("name='"+tmpName.toString()+"'");     // Tymczasowo ustaw filtr
    //        tm->select();                                       // Zatwierdz zmiany

    //        if(tm->rowCount() != 1) //Powinien pasowac tylko jeden rekord
    //        {
    //            qDebug() << "Zwrocono wiecej niz jeden rekord. Bald. Powiadom administratora";
    //        }
    //        else
    //        {
    //            rowNumber = 0;
    //            newRecord = tm->record(rowNumber);  //Pobierz ostatnio dodany rekord
    //            newRecord.setValue("name", newRecord.value("id")); //ustaw w polu name wartosc id
    //            tm->setRecord(rowNumber, newRecord);
    //        }

    //        tm->setFilter("");  //Przywroc filter na domyslny
    //        tm->select();       //Zatwierdz zmiany
    //    }
}

void Workers::on_deleteButton_clicked()
{

    //    QMessageBox mb;
    //    QModelIndexList list;

    //    list = ui->listView->selectionModel()->selectedIndexes(); //Pobierz listę zaznaczonych elementow
    //    if( list.empty() ) return; //Brak zaznaczenia

    //    mb.setText("Czy napewno chcesz usunąć zaznaczone rekordy?");
    //    mb.setIcon(QMessageBox::Question);
    //    mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    //    mb.setDefaultButton(QMessageBox::No);

    //    if( mb.exec() == QMessageBox::Yes )
    //    {
    //        foreach (const QModelIndex &index, list) {
    //            tm->removeRow(index.row());
    //        }
    //    }

    //    tm->select();
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

void Workers::on_listView_doubleClicked(const QModelIndex &index)
{

}
