#include <QSqlTableModel>
#include <QSqlRecord>
#include <QDebug>
#include <QMessageBox>
#include <QUuid>
#include <QString>
#include <iostream>
#include "productlist.h"
#include "ui_productlist.h"
#include "settings.h"

ProductList::ProductList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProductList)
{
    ui->setupUi(this);    
    product = new Product(parent);

    tm = 0;     //Zeruj wskaznik do modelu danych
}

void ProductList::setDB(const QSqlDatabase &db)
{
    if(tm) delete tm;           //Usun wskaznik jezeli zostal wczesniej zainnicjalizowany
    tm = new QSqlTableModel(0, db);
    product->setDB(db);
}

void ProductList::setTable(const QString &tableName)
{
    if(tm)
    {
        this->tableName = tableName;

        tm->setTable(tableName);
        tm->setEditStrategy(QSqlTableModel::OnManualSubmit);
        tm->setSort(0, Qt::AscendingOrder);
        tm->select();
    }
}

void ProductList::setWindowTitle(const QString &title)
{
    windowTitle = title;
    QDialog::setWindowTitle(windowTitle);
}

void ProductList::show()
{
    if(tm)
    {
        tm->select();    //Wykonaj zapytanie przed otwqarciem okna
        ui->tableView->setModel(tm);
    }
    QDialog::show();
}

ProductList::~ProductList()
{
    delete ui;
    if(tm) delete tm;    //Usun model danych
}

void ProductList::on_addButton_clicked()
{
    product->show();

}
