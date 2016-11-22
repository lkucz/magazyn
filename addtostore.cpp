#include <QDebug>
#include <QMessageBox>
#include <QModelIndexList>
#include <QModelIndex>
#include <QMap>
#include <QList>
#include "addtostore.h"
#include "ui_addtostore.h"

AddToStore::AddToStore(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddToStore)
{
    ui->setupUi(this);

    docTypeTableModel = 0;      //QSqlTableModel dla rodzaju dokumentow
    productListWindow = 0;      //Okno z lista produktów
    productListTableModel = 0;             //Model dla tableView
}

AddToStore::~AddToStore()
{
    //Usuń zaalokowane obiekty
    if(docTypeTableModel) delete docTypeTableModel;
    if(productListWindow) delete productListWindow;
    if(productListTableModel) delete productListTableModel;

    delete ui;
}

void AddToStore::setDB(const QSqlDatabase &db)
{
    this->db = db;

    if(docTypeTableModel) delete docTypeTableModel;           //Usun wskaznik jezeli zostal wczesniej zainnicjalizowany

    docTypeTableModel = new QSqlTableModel(0, db);
    docTypeTableModel->setTable(Settings::documentDictTableName());
    docTypeTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    docTypeTableModel->setSort(1, Qt::AscendingOrder);
    docTypeTableModel->select();

    ui->documentType->setModel(docTypeTableModel);
    ui->documentType->setModelColumn(1);
}

void AddToStore::show()
{
    if(docTypeTableModel) docTypeTableModel->select();     //Wykonaj zapytanie przed otwarciem okna

    if(!productListTableModel)
    {
        productListTableModel = new QStandardItemModel(0, 4, this);
        ui->productList->setModel(productListTableModel);
    }

    productListTableModel->clear();
    productListTableModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Nazwa")));
    productListTableModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Symbol")));
    productListTableModel->setHorizontalHeaderItem(2, new QStandardItem(QString("ilość")));
    productListTableModel->setHorizontalHeaderItem(3, new QStandardItem(QString("jednostka")));

    float w = ui->productList->width()/6; //Przelicz szerokosc kolumn
    ui->productList->setColumnWidth(0, (int)w*2);
    ui->productList->setColumnWidth(1, (int)w*2);
    ui->productList->setColumnWidth(2, w);
    ui->productList->setColumnWidth(2, w);

    products.clear();

    ui->documentID->clear();

    QDialog::show();
}

void AddToStore::addProduct()
{
    if(!productListWindow)
    {
        productListWindow = new ProductList(this);
        productListWindow->setWindowTitle("Lista produktów");
        productListWindow->setDB(this->db);
        productListWindow->setFlags(ProductList::Select);

        connect(productListWindow, &ProductList::dataSelected, this, &AddToStore::dataSelected);
    }

    productListWindow->show();
}

void AddToStore::deleteProduct()
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
        // Wyszukaj numerow rzedow, ktore nalezy usunac
        // utworz hashmap, wybierz unikalne klucze
        QMap<int, int> map;
        QList<int> row;

        foreach (const QModelIndex &index, list) {
            map.insert(index.row(), index.row());
        }

        row = map.uniqueKeys();     // wybierz unikalne klucze
        qSort(row);                 // sortuj

        QListIterator<int> i(row);  // utworz liste
        i.toBack();                 // ustaw iterator na koniec

        // usun elementy od konca
        while(i.hasPrevious())
        {
            int r = i.previous();
            productListTableModel->removeRow(r);
            products.removeAt(r);
        }
    }

}

void AddToStore::dataSelected(QModelIndexList list)
{
    QList<QStandardItem *> row;

    products.append(list); //Zapisz produkt na liscie

    //Dodaj nazwę produktu
    row.append(new QStandardItem(list[1].data().toString()));
    row.last()->setEditable(false);

    //Dodaj symbol produktu
    row.append(new QStandardItem(list[2].data().toString()));
    row.last()->setEditable(false);

    productListTableModel->appendRow(row);
}

void AddToStore::accept()
{
    QListIterator<QModelIndexList> i(products);
    while(i.hasNext())
    {
        foreach (const QModelIndex &index, i.next()) {
            qDebug()<<index.data();
        }
    }

    // this->hide();
}

void AddToStore::reject()
{
    this->hide();
}
