#include <QMessageBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>
#include <QList>
#include <QMap>
#include "productlist.h"
#include "preparework.h"
#include "settings.h"
#include "ui_preparework.h"

PrepareWork::PrepareWork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrepareWork)
{
    ui->setupUi(this);

    productList = 0;            //SQLModel do tabeli z produktami
    productListWindow = 0;      //Okno z lista produktów
    tableModel = 0;
}

PrepareWork::~PrepareWork()
{
    if(productList) delete productList;                 //Usuń zaalokowane obiekty
    if(productListWindow) delete productListWindow;
    if(tableModel) delete tableModel;

    delete ui;
}

void PrepareWork::setDB(const QSqlDatabase &db)
{
    this->db = db;

    if(productList) delete productList;           //Usun wskaznik jezeli zostal wczesniej zainnicjalizowany

    productList = new QSqlTableModel(0, db);
    productList->setTable(Settings::productViewName());
    productList->setEditStrategy(QSqlTableModel::OnManualSubmit);
    productList->setSort(1, Qt::AscendingOrder);
    productList->select();

    ui->finalProductCB->setModel(productList);
    ui->finalProductCB->setModelColumn(1);

}

void PrepareWork::show()
{
    if(productList)  productList->select();     //Wykonaj zapytanie przed otwqarciem okna

    if(!tableModel)
    {
        tableModel = new QStandardItemModel(0, 3, this);
        ui->productList->setModel(tableModel);
    }

    tableModel->clear();
    tableModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Nazwa")));
    tableModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Symbol")));
    tableModel->setHorizontalHeaderItem(2, new QStandardItem(QString("ilość")));

    products.clear();

    ui->qtLineEdit->clear();
    ui->workIDLineEdit->clear();

    QDialog::show();
}

void PrepareWork::addProduct()
{
    if(!productListWindow)
    {
        productListWindow = new ProductList(this);
        productListWindow->setWindowTitle("Lista produktów");
        productListWindow->setDB(this->db);
        productListWindow->setFlags(ProductList::Select);

        connect(productListWindow, &ProductList::dataSelected, this, &PrepareWork::dataSelected);
    }

    productListWindow->show();
}

void PrepareWork::deleteProduct()
{
    QMessageBox mb;
    QModelIndexList list;

    list = ui->productList->selectionModel()->selectedIndexes(); //Pobierz listę zaznaczonych elementow
    if( list.empty() ) return; //Brak zaznaczenia

    mb.setText("Czy napewno chcesz usunąć zaznaczone rekordy?");
    mb.setIcon(QMessageBox::Question);
    mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    mb.setDefaultButton(QMessageBox::No);

    if( mb.exec() == QMessageBox::Yes )
    {
        foreach (const QModelIndex &index, list) {
            if(index.column()==0){
                tableModel->removeRow(index.row());
                products.removeAt(index.row());
            }
        }
    }
}

void PrepareWork::dataSelected(QModelIndexList list)
{
    QList<QStandardItem *> row;

    products.append(list); //Zapisz produkt na liscie

    //Dodaj nazwę produktu
    row.append(new QStandardItem(list[1].data().toString()));
    row.last()->setEditable(false);

    //Dodaj symbol produktu
    row.append(new QStandardItem(list[2].data().toString()));
    row.last()->setEditable(false);

    tableModel->appendRow(row);
}

void PrepareWork::accept()
{
    QListIterator<QModelIndexList> i(products);
    while(i.hasNext())
    {
        foreach (const QModelIndex &index, i.next()) {
            qDebug()<<index.data();
        }
    }
  //  this->hide();
}

void PrepareWork::reject()
{
    this->hide();
}


