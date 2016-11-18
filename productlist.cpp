#include <QSqlTableModel>
#include <QSqlRecord>
#include <QDebug>
#include <QMessageBox>
#include <QUuid>
#include <QString>
#include <QSqlError>
#include <iostream>
#include "productlist.h"
#include "ui_productlist.h"
#include "settings.h"

ProductList::ProductList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProductList)
{
    ui->setupUi(this);

    tm = 0;     //Zeruj wskaznik do modelu danych
    productWindow = 0;
}

ProductList::~ProductList()
{
    delete ui;

    if(tm) delete tm;    //Usun model danych
    if(productWindow) delete productWindow;
}

void ProductList::setDB(const QSqlDatabase &db)
{
    if(tm) delete tm;           //Usun wskaznik jezeli zostal wczesniej zainnicjalizowany
    tm = new QSqlTableModel(0, db);
    this->db = db;
}

void ProductList::setTable(const QString &tableName)
{
    if(tm)
    {
        this->tableName = tableName;

        tm->setTable(tableName);
        tm->setEditStrategy(QSqlTableModel::OnManualSubmit);
        tm->setSort(0, Qt::AscendingOrder);
        ui->tableView->setModel(tm);
        ui->tableView->setColumnHidden(0, true);
    }
}

void ProductList::setWindowTitle(const QString &title)
{
    windowTitle = title;
    QDialog::setWindowTitle(windowTitle);
}

void ProductList::show()
{
    if(tm) tm->select();    //Wykonaj zapytanie przed otwqarciem okna

    QDialog::show();
}


void ProductList::on_addButton_clicked()
{
    if(!productWindow){
        productWindow = new Product(this);
        productWindow->setDB(db);
        productWindow->setWindowTitle("Nowy produkt");
    }
    productWindow->show();
}

void ProductList::on_deleteButton_clicked()
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
    qDebug() << tm->lastError();
    tm->select();
}
