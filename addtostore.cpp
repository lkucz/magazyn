#include <QDebug>
#include <QMessageBox>
#include <QModelIndexList>
#include <QModelIndex>
#include <QSqlRecord>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
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
    storeTableModel = 0;          //Model dla magazynu w bazie danych
}

AddToStore::~AddToStore()
{
    //Usuń zaalokowane obiekty
    if(docTypeTableModel) delete docTypeTableModel;
    if(productListWindow) delete productListWindow;
    if(productListTableModel) delete productListTableModel;
    if(documentTableModel) delete documentTableModel;
    if(storeTableModel) delete storeTableModel;

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
    if(storeTableModel) delete storeTableModel;

    storeTableModel = new QSqlTableModel(0, db);
    storeTableModel->setTable(Settings::storeTableName());
    storeTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    storeTableModel->setSort(1, Qt::AscendingOrder);
    storeTableModel->select();
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
    ui->productList->setColumnWidth(0, (int)w*2.2);
    ui->productList->setColumnWidth(1, (int)w*2.2);
    ui->productList->setColumnWidth(2, (int)w*1.2);
    ui->productList->setColumnWidth(3, (int)w*1.2);

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

    products.append(list[0].data().toInt()); //Zapisz produkt na liscie

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

    // Ustaw domyślne wartości dla okienka ostrzeżenia
    mb.setIcon(QMessageBox::Warning);
    mb.setStandardButtons(QMessageBox::Ok);
    mb.setDefaultButton(QMessageBox::Ok);

    //Walidacja dokumentu
    if(ui->documentType->currentIndex() == -1 || ui->documentType->currentIndex() == 0)
    {
        mb.setText("Nie został wybrany typ dokumentu.");
        mb.exec();

        ui->documentType->setFocus();
        return ;
    }

    if(ui->documentID->text().isEmpty() == true)
    {
        mb.setText("Nie został wpisany numer dokumentu.");
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
        documentTableModel->select();

        if(rows > 0)
        {
            mb.setText("Podany numer dokumentu istnieje w bazie danych.");
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
        mb.exec();

        return ;
    }

    for(int i=0; i<rows; ++i)
    {
        QModelIndex index = productListTableModel->index(i, 2); //Kolumna z ilosciami produkow
        if(productListTableModel->data(index).toFloat() <= 0)
        {
            mb.setText("Przynajmniej jeden z produków w tabeli zawiera zerową ilość.");
            mb.exec();

            return;
        }
    }

    //Dodaj dokument do bazy danych
    QSqlRecord docNewRecord;
    int r=0, c=0;

    docNewRecord = documentTableModel->record();
    docNewRecord.setGenerated("id", true);
    docNewRecord.setValue("number", ui->documentID->text());
    docNewRecord.setValue("date", QVariant(ui->dateEdit->text()));
    r = ui->documentType->currentIndex();   // Pobierz index z ComboBox - rodzaj dokumentu
    docNewRecord.setValue("type", QVariant(docTypeTableModel->index(r, c).data()));   // c-kolumna ustawiona na 0, w każdej tabeli pole ID==0

    if(documentTableModel->insertRecord(-1, docNewRecord) != true)
    {
        //Zapis do bazy danych sie nie udal
        qDebug() << "Problem z wstawieniem rekordu do tabeli " << Settings::productTableName();
        qDebug() << docNewRecord;

        return;
    }

    //Prześlij zaminy do bazy danych
    documentTableModel->submitAll();


    //Pobierz numer id dla nowego dokumentu
    QSqlQuery docIDQuery(this->db);
    docIDQuery.prepare("SELECT id FROM document WHERE number=:ID");
    docIDQuery.bindValue(":ID", docNewRecord.value("number"));
    docIDQuery.exec();

    if(docIDQuery.size() != 1)
    {
        qDebug() << "Problem z wynikiem zapytania do bazy danych";
        qDebug() << "docIDQuery.size() " << docIDQuery.size();

        return ;
    }

    //Dodaj produkty do bazy danych
    for(int i=0; i<rows; ++i)
    {
        QModelIndex index = productListTableModel->index(i, 2);     //Kolumna w view zawierająca ilość == 2
        float quantity = productListTableModel->data(index).toFloat();

        QSqlRecord productNewRecord;
        productNewRecord = storeTableModel->record();
        productNewRecord.setGenerated("id", true);
        productNewRecord.setValue("product", products[i]);
        productNewRecord.setValue("quantity", quantity);
        productNewRecord.setValue("date", QDateTime(QDateTime::currentDateTime()));
        productNewRecord.setValue("document", docIDQuery.value(0).toInt());
    }

    storeTableModel->submitAll();

    this->hide();
}

void AddToStore::reject()
{
    this->hide();
}
