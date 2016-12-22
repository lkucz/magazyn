#include <QModelIndex>
#include <QSqlRecord>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QMap>

#include "settings.h"
#include "storetransactions.h"
#include "ui_storetransactions.h"

StoreTransactions::StoreTransactions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StoreTransactions)
{
    ui->setupUi(this);

    tableModel = 0;         //Ustaw wskaznik na model SQL na 0
}

StoreTransactions::~StoreTransactions()
{
    if(tableModel) delete tableModel;

    delete ui;
}

void StoreTransactions::setDB(const QSqlDatabase &db)
{
    this->db = db;

    if(tableModel) delete tableModel;           //Usun wskaznik jezeli zostal wczesniej zainnicjalizowany
    tableModel = new QSqlTableModel(0, db);

    tableModel->setTable(Settings::storeTransTableName());
    tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tableModel->setSort(4, Qt::AscendingOrder);
    tableModel->select();

    //Zmień nazwy kolumnn
    tableModel->setHeaderData(1, Qt::Horizontal, tr("Produkt"));
    tableModel->setHeaderData(2, Qt::Horizontal, tr("Symbol"));
    tableModel->setHeaderData(3, Qt::Horizontal, tr("Ilość"));
    tableModel->setHeaderData(4, Qt::Horizontal, tr("Data."));
    tableModel->setHeaderData(5, Qt::Horizontal, tr("Numer"));
    tableModel->setHeaderData(6, Qt::Horizontal, tr("Użytkownik"));

    ui->tableView->setModel(tableModel);
    ui->tableView->setColumnHidden(0, true);

    //Przelicz szerokosc kolumn
    float w = ui->tableView->width()/14;
    ui->tableView->setColumnWidth(1, (int)w*4);
    ui->tableView->setColumnWidth(2, (int)w*2.5);
    ui->tableView->setColumnWidth(3, (int)w);
    ui->tableView->setColumnWidth(4, (int)w*2.5);
    ui->tableView->setColumnWidth(5, (int)w*2);
    ui->tableView->setColumnWidth(6, (int)w*2);
}

void StoreTransactions::show()
{
    if(tableModel) tableModel->select();
    QDialog::show();
}
