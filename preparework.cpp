#include <QMessageBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>
#include <QList>
#include <QMap>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QUuid>
#include "storelist.h"
#include "productlist.h"
#include "preparework.h"
#include "settings.h"
#include "ui_preparework.h"

PrepareWork::PrepareWork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrepareWork)
{
    ui->setupUi(this);

    productList = 0;              //SQLModel do tabeli z produktami -- typy produkow finalnych dla ComboBox
    storeListWindow = 0;         //Okno z lista produktów dostepnych na magazynie
    tableModel = 0;              //Model dla wigdety tableView (lista produktow do dodania)
}

PrepareWork::~PrepareWork()
{
    if(productList) delete productList;                 //Usuń zaalokowane obiekty
    if(storeListWindow) delete storeListWindow;
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
    tableModel->setHorizontalHeaderItem(3, new QStandardItem(QString("jedn.")));

    float w = ui->productList->width()/12;
    ui->productList->setColumnWidth(0, (int)w*5);
    ui->productList->setColumnWidth(1, (int)w*3);
    ui->productList->setColumnWidth(2, (int)w*1.5);
    ui->productList->setColumnWidth(3, (int)w*1.5);

    ui->checkBox->setChecked(true);
    ui->qtyLineEdit->clear();
    ui->workIDLineEdit->clear();
    ui->workIDLineEdit->setReadOnly(true);

    this->genIDstateChanged(0);

    products.clear(); // Wyczyść listę produktów do dodania

    QDialog::show();
}

void PrepareWork::addProduct()
{
    if(!storeListWindow)
    {
        storeListWindow = new StoreList(this);
        storeListWindow->setWindowTitle("Produkty na magazynie");
        storeListWindow->setDB(this->db);

        connect(storeListWindow, &StoreList::dataSelected, this, &PrepareWork::dataSelected);
    }

    storeListWindow->show();
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
    Prod prod;                      //id produktu + stan magazynu
    QList<QStandardItem *> row;     //Nowa linia w tabeli do wyświetlenia

    //Element list zawiera jeden wpis z tabeli storeQty w następującym formacie
    // 0:id, 1:nazwa, 2:symbol, 3:ilosc, 4:jednostka, 5:min, 6:max, 7:komentarz

    //Przygotuj wpis do tabeli
    //Dodaj nazwę produktu
    row.append(new QStandardItem(list[1].data().toString()));
    row.last()->setEditable(false);

    //Dodaj symbol produktu
    row.append(new QStandardItem(list[2].data().toString()));
    row.last()->setEditable(false);

    //Dodaj ilość i ustaw na 0
    row.append(new QStandardItem(QString("0")));

    //Dodaj jednostkę
    row.append(new QStandardItem(list[4].data().toString()));
    row.last()->setEditable(false);

    //Dodaj nowy wpis do tabeli
    tableModel->appendRow(row);

    //Zapamiętaj id + stan magazynu w strukturze
    prod.id = list[0].data().toInt();
    prod.qty = list[3].data().toFloat();
    products.append(prod);
}

void PrepareWork::accept()
{
    QMessageBox mb;
    QSqlRecord newRecord;
    int rows;

    mb.setIcon(QMessageBox::Warning);
    mb.setStandardButtons(QMessageBox::Ok);
    mb.setDefaultButton(QMessageBox::Ok);

    //Czy został wybrany produkt końcowy
    if(ui->finalProductCB->currentIndex() == -1 || ui->finalProductCB->currentIndex() == 0)
    {
        mb.setText("Nie został produkt końcowy.");
        mb.exec();

        ui->finalProductCB->setFocus();
        return ;
    }

    //Czy ilość produktów końcowych jest prawidłowa
    if(ui->qtyLineEdit->text().isEmpty() || ui->qtyLineEdit->text().toFloat() <= 0)
    {
        mb.setText("Nieprawidłowa ilość produktu końcowego");
        mb.exec();
        ui->qtyLineEdit->setFocus();

        return ;
    }

    //Czy numer pracy jest ustawiony
    if(ui->workIDLineEdit->text().isEmpty() == true)
    {
        mb.setText("Brak numeru pracy.");
        mb.exec();

        ui->workIDLineEdit->setFocus();
        return ;
    }

    //Sprawdz w bazie SQL czy numer pracy istnieje
    QSqlQuery docIDQuery(this->db);
    docIDQuery.prepare("SELECT id FROM document WHERE number=:ID and type=:DocID");
    docIDQuery.bindValue(":ID", ui->workIDLineEdit->text());
    docIDQuery.bindValue(":DocID", Settings::workDocumentID());
    docIDQuery.exec();

    qDebug() << docIDQuery.first();

    this->hide();
}

void PrepareWork::reject()
{
    this->hide();
}

void PrepareWork::genIDstateChanged(int )
{
    if(ui->checkBox->isChecked())
    {
        QString wordID = QUuid::createUuid().toString();
        ui->workIDLineEdit->setText(wordID);
        ui->workIDLineEdit->setReadOnly(true);
    }
    else
    {
        ui->workIDLineEdit->clear();
        ui->workIDLineEdit->setReadOnly(false);
    }
}
