#include <QDebug>
#include <QMessageBox>
#include <QModelIndexList>
#include <QModelIndex>
#include <QSqlRecord>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QMap>

#include "colordelegate.h"
#include "storelist.h"
#include "ui_storelist.h"
#include "settings.h"

StoreList::StoreList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StoreList)
{
    ui->setupUi(this);

    //Ustaw obiekt typu delegate, żeby kolorować wiersze
    ui->tableView->setItemDelegate(new ColorDelegate(ui->tableView));

    tableModel = 0;         //Ustaw wskaznik na model SQL na 0
}

StoreList::~StoreList()
{
    if(tableModel) delete tableModel;

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

void StoreList::show()
{
    QMap<int, QColor> colorMap;

    // Wykonaj zapytanie przed otwarciem okna
    if(tableModel) tableModel->select();

    // Pobierz ilość elementów w tableli
    int rowCount = tableModel->rowCount();
    if(rowCount > 0)
    {
        int qtyFI = tableModel->fieldIndex("qty");
        int minFI = tableModel->fieldIndex("alarm_min");
        int maxFI = tableModel->fieldIndex("alarm_max");

        for(int i=0; i<rowCount; ++i)
        {
            QModelIndex qty = tableModel->index(i, qtyFI);
            QModelIndex min = tableModel->index(i, minFI);
            QModelIndex max = tableModel->index(i, maxFI);

            //Sprawdz czy nie są przekroczone alarmy
            if(qty.data().toFloat() < min.data().toFloat() || qty.data().toFloat() > max.data().toFloat())
            {
             colorMap.insert(i, QColor(Qt::red));
            }
        }
    }
    (static_cast<ColorDelegate *> (ui->tableView->itemDelegate()))->setColorMap(colorMap);

    QDialog::show();
}

void StoreList::accept()
{
    QModelIndexList list = ui->tableView->selectionModel()->selectedIndexes();
    if(!list.empty())
    {
        emit dataSelected(list);
    }

    this->hide();
}

void StoreList::reject()
{
    this->hide();
}

void StoreList::on_tableView_doubleClicked(const QModelIndex &index)
{
    QModelIndexList list = ui->tableView->selectionModel()->selectedIndexes();
    emit dataSelected(list);
    this->hide();
}
