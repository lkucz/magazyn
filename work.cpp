#include "work.h"
#include "ui_work.h"
#include "settings.h"

Work::Work(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Work)
{
    ui->setupUi(this);

    tableModel = 0;         //TableModel dla listy pracowników ComboBox
}

Work::~Work()
{
    if(tableModel) delete tableModel;

    delete ui;
}

void Work::setDB(const QSqlDatabase &db)
{
    this->db = db;

    if(tableModel) delete tableModel;           //Usun wskaznik jezeli zostal wczesniej zainnicjalizowany
    tableModel = new QSqlTableModel(0, db);

    tableModel->setTable(Settings::workerListTableName());
    tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tableModel->setSort(1, Qt::AscendingOrder);
    tableModel->select();

//    ui->tableView->setModel(tableModel);
//    ui->tableView->setColumnHidden(0, true);

    //Przelicz szerokosc kolumn
 /*   float w = ui->tableView->width()/3;
    ui->tableView->setColumnWidth(1, (int)w*0.9);
    ui->tableView->setColumnWidth(2, (int)w*0.9);
    ui->tableView->setColumnWidth(3, (int)w*0.9);
    */
}

void Work::show()
{
    if(tableModel) tableModel->select();
    QDialog::show();
}
