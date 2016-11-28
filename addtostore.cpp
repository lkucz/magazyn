#include <QDebug>
#include <QMessageBox>
#include <QModelIndexList>
#include <QModelIndex>
#include <QSqlRecord>
#include "addtostore.h"
#include "ui_addtostore.h"

AddToStore::AddToStore(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddToStore)
{
    ui->setupUi(this);

    docTypeTableModel = 0;          //QSqlTableModel dla rodzaju dokumentow
    documentTableModel = 0;         //Tabela z numerami dokumentow
    productListWindow = 0;          //Okno z lista produktów
    productListTableModel = 0;      //Model dla tableView (tabelka z produktami do wprowadzenia na magazyn)
    productTableModel = 0;          //Model dla tabeli z produktami
}

AddToStore::~AddToStore()
{
    //Usuń zaalokowane obiekty
    if(docTypeTableModel) delete docTypeTableModel;
    if(productListWindow) delete productListWindow;
    if(productListTableModel) delete productListTableModel;
    if(documentTableModel) delete documentTableModel;
    if(productTableModel) delete productTableModel;

    delete ui;
}

void AddToStore::setDB(const QSqlDatabase &db)
{
    this->db = db;

    if(docTypeTableModel) delete docTypeTableModel;           //Usun wskaznik jezeli zostal wczesniej zainnicjalizowany

    // Ustaw paramatry dla ComboBox z rodzajem dokumentow
    docTypeTableModel = new QSqlTableModel(0, db);
    docTypeTableModel->setTable(Settings::documentDictTableName());
    docTypeTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    docTypeTableModel->setSort(1, Qt::AscendingOrder);
    docTypeTableModel->select();

    ui->documentType->setModel(docTypeTableModel);
    ui->documentType->setModelColumn(1);

    // Inicjalizacja tabeli z dokumentami
    if(documentTableModel) delete documentTableModel;

    documentTableModel = new QSqlTableModel(0, db);
    documentTableModel->setTable(Settings::documentTableName());
    documentTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    documentTableModel->setSort(1, Qt::AscendingOrder);
    documentTableModel->select();

    // Inicjalizacja tabeli z produktami
    if(productTableModel) delete productTableModel;

    productTableModel = new QSqlTableModel(0, db);
    productTableModel->setTable(Settings::productTableName());
    productTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    productTableModel->setSort(1, Qt::AscendingOrder);
    productTableModel->select();
}

void AddToStore::show()
{
    if(docTypeTableModel) docTypeTableModel->select();     //Wykonaj zapytanie przed otwarciem okna

    //Utworz dataModel jezeli nie był wcześniej używany
    if(!productListTableModel)
    {
        productListTableModel = new QStandardItemModel(0, 4, this);
        ui->productList->setModel(productListTableModel);
    }

    //Ustaw prawidłowe nazwy kolumn
    productListTableModel->clear();
    productListTableModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Nazwa")));
    productListTableModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Symbol")));
    productListTableModel->setHorizontalHeaderItem(2, new QStandardItem(QString("ilość")));
    productListTableModel->setHorizontalHeaderItem(3, new QStandardItem(QString("jednostka")));

    //Przelicz szerokosc kolumn
    float w = ui->productList->width()/7;
    ui->productList->setColumnWidth(0, (int)w*2);
    ui->productList->setColumnWidth(1, (int)w*2);
    ui->productList->setColumnWidth(2, (int)w*1.2);
    ui->productList->setColumnWidth(2, (int)w*1.2);

    //Usuń wszystkie produkty z listy
    products.clear();

    //Usun numer dokumentu
    ui->documentID->clear();

    //Ustaw prawidłowo zegar
    ui->dateEdit->setDate(QDate::currentDate());

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
        foreach (const QModelIndex &index, list) {
            if(index.column()==0){
                productListTableModel->removeRow(index.row());
                products.removeAt(index.row());
            }
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

    //Dodaj pole ilosc i ustaw na 0
    row.append(new QStandardItem(QString("0")));

    //Dodaj jednostkę
    row.append(new QStandardItem(list[7].data().toString()));
    row.last()->setEditable(false);

    productListTableModel->appendRow(row);
}

void AddToStore::accept()
{
    QMessageBox mb;
    QSqlRecord newRecord;
    int rows;

    //Walidacja dokumentu
    if(ui->documentType->currentIndex() == -1 || ui->documentType->currentIndex() == 0)
    {
        mb.setText("Nie został wybrany typ dokumentu.");
        mb.setIcon(QMessageBox::Warning);
        mb.setStandardButtons(QMessageBox::Ok);
        mb.setDefaultButton(QMessageBox::Ok);
        mb.exec();

        ui->documentType->setFocus();
        return ;
    }

    if(ui->documentID->text().isEmpty() == true)
    {
        mb.setText("Nie został wpisany numer dokumentu.");
        mb.setIcon(QMessageBox::Warning);
        mb.setStandardButtons(QMessageBox::Ok);
        mb.setDefaultButton(QMessageBox::Ok);
        mb.exec();

        ui->documentID->setFocus();
        return ;
    }
    else
    {
        //Sprawdz czy podany numer istnieje w bazie

        //Ustaw filter i sprawdz czy numer dokumentu istnieje w bazie
        documentTableModel->setFilter("number='"+ui->documentID->text()+"'");
        documentTableModel->select();
        rows = documentTableModel->rowCount();

        //Usun filter
        documentTableModel->setFilter(QString());
        docTypeTableModel->select();

        if(rows > 0)
        {
            mb.setText("Podany numer dokumentu istnieje w bazie danych.");
            mb.setIcon(QMessageBox::Warning);
            mb.setStandardButtons(QMessageBox::Ok);
            mb.setDefaultButton(QMessageBox::Ok);
            mb.exec();

            ui->documentID->setFocus();
            return ;
        }
    }

    //Walidacja listy produktów
    rows = productListTableModel->rowCount();

    //Lista produktów jest pusta
    if(rows == 0)
    {
        mb.setText("Tabela nie zawiera produktów.");
        mb.setIcon(QMessageBox::Warning);
        mb.setStandardButtons(QMessageBox::Ok);
        mb.setDefaultButton(QMessageBox::Ok);
        mb.exec();

        return ;
    }

    for(int i=0; i<rows; ++i)
    {
        QModelIndex index = productListTableModel->index(i, 3); //Kolumna z ilosciami produkow
        if(productListTableModel->data(index).toFloat() == 0)
        {
            mb.setText("Przynajmniej jeden z produków w tabeli zawiera zerową ilość.");
            mb.setIcon(QMessageBox::Warning);
            mb.setStandardButtons(QMessageBox::Ok);
            mb.setDefaultButton(QMessageBox::Ok);
            mb.exec();

            return;
        }
    }

    //Dodaj dokument do bazy danych

    //Dodaj produkty do bazy danych


//    QListIterator<QModelIndexList> i(products);
//    while(i.hasNext())
//    {
//        foreach (const QModelIndex &index, i.next()) {
//            qDebug()<<index.data();
//        }
//    }

    // this->hide();
}

void AddToStore::reject()
{
    this->hide();
}
