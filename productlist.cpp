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

    //Zeruj wskaznik do modelu danych, wskaznik okna i flagi
    flags = 0;
    tm = 0;
    productWindow = 0;
    productTableModel = 0;
}

ProductList::~ProductList()
{
    //Usun zaalokowane obiekty
    if(tm) delete tm;
    if(productWindow) delete productWindow;
    if(productTableModel) delete productTableModel;

    delete ui;
}

void ProductList::setDB(const QSqlDatabase &db)
{
    this->db = db;

    if(tm) delete tm;           //Usun wskaznik jezeli zostal wczesniej zainnicjalizowany
    tm = new QSqlTableModel(0, db);

    tm->setTable(Settings::productViewName());
    tm->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tm->setSort(1, Qt::AscendingOrder);
    tm->select();

    //Zmień nazwy kolumnn
    tm->setHeaderData(1, Qt::Horizontal, tr("Nazwa"));
    tm->setHeaderData(2, Qt::Horizontal, tr("Symbol"));
    tm->setHeaderData(3, Qt::Horizontal, tr("MIN"));
    tm->setHeaderData(4, Qt::Horizontal, tr("MAX"));
    tm->setHeaderData(5, Qt::Horizontal, tr("Uwagi"));
    tm->setHeaderData(7, Qt::Horizontal, tr("Jednostka"));

    ui->tableView->setModel(tm);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(6, true);

    //Przelicz szerokosc kolumn
    float w = ui->tableView->width()/12;
    ui->tableView->setColumnWidth(1, (int)w*3);
    ui->tableView->setColumnWidth(2, (int)w*2);
    ui->tableView->setColumnWidth(3, (int)w);
    ui->tableView->setColumnWidth(4, (int)w);
    ui->tableView->setColumnWidth(5, (int)w*3.2);
    ui->tableView->setColumnWidth(7, (int)w*1.5);
}

void ProductList::show()
{
    if(tm) tm->select();    //Wykonaj zapytanie przed otwqarciem okna

    //Jezeli okno jest ustawione w trybie Select przestaw mozliwosc na pojedynczy wybor
    if(flags == ProductList::Select) ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    QDialog::show();
}

void ProductList::refreshTable()
{
    tm->select();
}

void ProductList::on_addButton_clicked()
{
    if(!productWindow){
        productWindow = new Product(this);
        productWindow->setDB(db);
        productWindow->setWindowTitle("Nowy produkt");
        connect(productWindow, &Product::dataChanged, this, &ProductList::refreshTable);
    }
    productWindow->show();
}

void ProductList::on_deleteButton_clicked()
{
    QMessageBox mb;
    QModelIndexList list;

    //Utworz obiekt tylko w przypadku usuwania
    if(!productTableModel)
    {
        productTableModel = new QSqlTableModel(0, this->db);
        productTableModel->setTable(Settings::productTableName());
        productTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        productTableModel->setSort(1, Qt::AscendingOrder);
        productTableModel->select();
    }

    list = ui->tableView->selectionModel()->selectedIndexes(); //Pobierz listę zaznaczonych elementow

    if( list.empty() ) return; //Brak zaznaczenia

    mb.setText("Czy napewno chcesz usunąć zaznaczone rekordy?");
    mb.setIcon(QMessageBox::Question);
    mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    mb.setDefaultButton(QMessageBox::No);

    if( mb.exec() == QMessageBox::Yes )
    {
        foreach (const QModelIndex &index, list) {
            if(index.column() == 0){
                productTableModel->setFilter("id='" + index.data().toString() + "'");
                productTableModel->select();
                productTableModel->removeRow(1);
                qDebug() << productTableModel->lastError();
                productTableModel->setFilter("");
                productTableModel->select();
            }
        }
    }

    productTableModel->submitAll();
    qDebug() << productTableModel->lastError();

    productTableModel->select();
    tm->select();
}

void ProductList::accept()
{
    if(flags == ProductList::Select)        //Sprawdz czy okno jest otwarte w trybie wyboru
    {
        QModelIndexList list;

        list = ui->tableView->selectionModel()->selectedIndexes(); //Pobierz liste zaznaczonych elementow
        if(!list.empty())
        {
            emit dataSelected(list);    //Przeslij sygnalem tylko pierwszy z zaznaczonych
        }
    }

    this->hide();
}

void ProductList::reject()
{
    this->hide();
}


void ProductList::on_tableView_doubleClicked(const QModelIndex &index)
{
    if(flags == ProductList::Select){
        QModelIndexList list;

        list = ui->tableView->selectionModel()->selectedIndexes();
        emit dataSelected(list);
        this->hide();
    }
}

void ProductList::setFlags(unsigned int flags)
{
    this->flags = flags;
}

unsigned int ProductList::getFlags()
{
    return flags;
}


