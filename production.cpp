#include <QDebug>
#include <QMessageBox>
#include <QModelIndexList>
#include <QModelIndex>
#include <QSqlRecord>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>

#include "production.h"
#include "ui_production.h"
#include "settings.h"

Production::Production(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Production)
{
    ui->setupUi(this);

    tableModel = 0;         //Table model dla widgetu z listą prac
    workWindow = 0;         //Ustaw wskaźnik okna z opisem pracy
}

Production::~Production()
{
    if(tableModel) delete tableModel;
    if(workWindow) delete workWindow;

    delete ui;
}

void Production::setDB(const QSqlDatabase &db)
{
    this->db = db;

    if(tableModel) delete tableModel;           //Usun wskaznik jezeli zostal wczesniej zainnicjalizowany
    tableModel = new QSqlTableModel(0, db);

    tableModel->setTable(Settings::productionListTableName());
    tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tableModel->setSort(1, Qt::AscendingOrder);
    tableModel->select();

    //Zmień nazwy kolumnn
    tableModel->setHeaderData(1, Qt::Horizontal, tr("Numer pracy"));
    tableModel->setHeaderData(2, Qt::Horizontal, tr("Stan"));
    tableModel->setHeaderData(3, Qt::Horizontal, tr("Pracownik"));

    ui->tableView->setModel(tableModel);
    ui->tableView->setColumnHidden(Settings::DB::productionList::id, true);
    ui->tableView->setColumnHidden(Settings::DB::productionList::WorkedID, true);

    //Przelicz szerokosc kolumn
    float w = ui->tableView->width()/3;
    ui->tableView->setColumnWidth(1, (int)w*0.9);
    ui->tableView->setColumnWidth(2, (int)w*0.9);
    ui->tableView->setColumnWidth(3, (int)w*0.9);

}

void Production::show()
{
    if(tableModel) tableModel->select();
    QDialog::show();
}

void Production::accept()
{
    this->hide();
}

void Production::reject()
{
    this->hide();
}

void Production::on_tableView_doubleClicked(const QModelIndex &index)
{
    // Utwórz obiekt okna, jeżeli nie było jeszcze używane
    if(!workWindow)
    {
        workWindow = new Work(this);
        workWindow->setWindowTitle("Szczegóły pracy");
        workWindow->setDB(this->db);
    }

    //W definicji tabeli w kolumnie nr 1 jest numer pracy
    int row = index.row();
    workWindow->setWorkID(QVariant(tableModel->index(row, Settings::DB::productionList::number).data()).toString());
    workWindow->setWorkerID(QVariant(tableModel->index(row, Settings::DB::productionList::WorkedID).data()).toInt());

    workWindow->show();
}
