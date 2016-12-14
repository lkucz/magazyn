#include <QModelIndex>
#include <QSqlRecord>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QMap>

#include "documents.h"
#include "ui_documents.h"
#include "settings.h"

Documents::Documents(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Documents)
{
    ui->setupUi(this);

    tableModel = 0;         //Ustaw wskaznik na model SQL na 0
}

Documents::~Documents()
{
    if(tableModel) delete tableModel;

    delete ui;
}

void Documents::setDB(const QSqlDatabase &db)
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
    tableModel->setHeaderData(3, Qt::Horizontal, tr("Ilość"));
    tableModel->setHeaderData(4, Qt::Horizontal, tr("Jedn."));
    tableModel->setHeaderData(5, Qt::Horizontal, tr("MIN"));
    tableModel->setHeaderData(6, Qt::Horizontal, tr("MAX"));
    tableModel->setHeaderData(7, Qt::Horizontal, tr("Uwagi"));

    ui->tableView->setModel(tableModel);
    ui->tableView->setColumnHidden(0, true);

    //Przelicz szerokosc kolumn
    float w = ui->tableView->width()/14;
    ui->tableView->setColumnWidth(1, (int)w*3);
    ui->tableView->setColumnWidth(2, (int)w*3);
    ui->tableView->setColumnWidth(3, (int)w);
    ui->tableView->setColumnWidth(4, (int)w);
    ui->tableView->setColumnWidth(5, (int)w);
    ui->tableView->setColumnWidth(6, (int)w);
    ui->tableView->setColumnWidth(7, (int)w*3.2);
}
