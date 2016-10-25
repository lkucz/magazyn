#include <QSqlTableModel>
#include <QSqlRecord>
#include <QDebug>
#include <QMessageBox>
#include <QUuid>
#include <QString>
#include <iostream>
#include "dictionary.h"
#include "ui_dictionary.h"
#include "settings.h"

Dictionary::Dictionary(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dictionary)
{
    ui->setupUi(this);

    tm = 0;     //Zeruj wskaznik do modelu danych
}

void Dictionary::setDB(const QSqlDatabase &db)
{
    if(tm) delete tm;           //Usun wskaznik jezeli zostal wczesniej zainnicjalizowany
    tm = new QSqlTableModel(0, db);
}

void Dictionary::setTable(const QString &tableName)
{
    if(tm)
    {
        this->tableName = tableName;

        tm->setTable(tableName);
        tm->setEditStrategy(QSqlTableModel::OnFieldChange);
        tm->setSort(1, Qt::AscendingOrder);
        tm->setHeaderData(0, Qt::Horizontal, tr("id"));
        tm->setHeaderData(1, Qt::Horizontal, tr("name"));
        tm->select();
    }
}

void Dictionary::show()
{
    if(tm)
    {
        tm->select();    //Wykonaj zapytanie przed otwqarciem okna
        ui->listView->setModel(tm);
        ui->listView->setModelColumn(1);
    }
    QDialog::show();
}

Dictionary::~Dictionary()
{
    delete ui;
    if(tm) delete tm;
}


void Dictionary::on_addButton_clicked()
{
    int rowNumber = -1;
    QSqlRecord newRecord;
    QUuid tmpName = QUuid::createUuid();

    newRecord = tm->record();
    newRecord.setGenerated("id", true);
    newRecord.setValue("name", QVariant(tmpName.toString()));

    if(tm->insertRecord(-1, newRecord) != true)
    {
        qDebug() << "Nie moge dodac rekordu do bazy danych: tabela(" << tableName << ")";

        QMessageBox mb;
        mb.setText("Nie moge dodac nowego rekordu do bazy danych " + tableName);
        mb.exec();
        mb.setIcon(QMessageBox::Critical);
    }
    else
    {
        tm->setFilter("name='"+tmpName.toString()+"'");     // Tymczasowo ustaw filtr
        tm->select();                                       // Zatwierdz zmiany

        if(tm->rowCount() != 1) //Powinien pasowac tylko jeden rekord
        {
            qDebug() << "Zwrocono wiecej niz jeden rekord. Bald. Powiadom administratora";
        }
        else
        {
            rowNumber = 0;
            newRecord = tm->record(rowNumber);  //Pobierz ostatnio dodany rekord
            newRecord.setValue("name", newRecord.value("id")); //ustaw w polu name wartosc id
            tm->setRecord(rowNumber, newRecord);
        }

        tm->setFilter("");  //Przywroc filter na domyslny
        tm->select();       //Zatwierdz zmiany


    }
}

void Dictionary::on_deleteButton_clicked()
{
    ui->listView->selectionModel()->selectedIndexes();

}

void Dictionary::on_closeButton_clicked()
{
    this->hide();
}

void Dictionary::on_clearFilterButton_clicked()
{
    ui->filterEdit->clear();
}

void Dictionary::on_filterEdit_textChanged(const QString &filter)
{
    tm->setFilter("name like '" + filter + "%'");
    tm->select();
}

