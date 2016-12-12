#include <QDebug>
#include <QMessageBox>
#include <QModelIndexList>
#include <QModelIndex>
#include <QSqlRecord>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include "storelist.h"
#include "ui_storelist.h"
#include "settings.h"

StoreList::StoreList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StoreList)
{
    ui->setupUi(this);
}

StoreList::~StoreList()
{
    delete ui;
}

void StoreList::setDB(const QSqlDatabase &db)
{
    this->db = db;

    if(tableModel) delete tableModel;           //Usun wskaznik jezeli zostal wczesniej zainnicjalizowany
    tableModel = new QSqlTableModel(0, db);

    tableModel->setTable(Settings::storeQtyTableName());
    tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tableModel->setSort(1, Qt::AscendingOrder);
    tableModel->select();

    //Zmień nazwy kolumnn
    tableModel->setHeaderData(1, Qt::Horizontal, tr("Nazwa"));
    tableModel->setHeaderData(2, Qt::Horizontal, tr("Symbol"));
    tableModel->setHeaderData(3, Qt::Horizontal, tr("MIN"));
    tableModel->setHeaderData(4, Qt::Horizontal, tr("MAX"));
    tableModel->setHeaderData(5, Qt::Horizontal, tr("Uwagi"));
    tableModel->setHeaderData(6, Qt::Horizontal, tr("Ilość"));
    tableModel->setHeaderData(7, Qt::Horizontal, tr("Jednostka"));

    ui->tableView->setModel(tableModel);
    ui->tableView->setColumnHidden(0, true);

    //Przelicz szerokosc kolumn
    float w = ui->tableView->width()/12;
    ui->tableView->setColumnWidth(1, (int)w*3);
    ui->tableView->setColumnWidth(2, (int)w*2);
    ui->tableView->setColumnWidth(3, (int)w);
    ui->tableView->setColumnWidth(4, (int)w);
    ui->tableView->setColumnWidth(5, (int)w*3.2);
    ui->tableView->setColumnWidth(7, (int)w*1.5);
}

void StoreList::show()
{
    QDialog::show();
}

void StoreList::accept()
{
    this->hide();
}

void StoreList::reject()
{
    this->hide();
}
