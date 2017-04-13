#include <QDebug>
#include "work.h"
#include "ui_work.h"
#include "settings.h"

Work::Work(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Work)
{
    ui->setupUi(this);

    workerTableModel = 0;         //TableModel dla listy pracowników ComboBox
    workDetailsTableModel = 0;      //TableModel dla opisu pracy

    workChanged = false;
    workerChanged = false;
    workerID = -1;
}

Work::~Work()
{
    if(workerTableModel) delete workerTableModel;
    if(workDetailsTableModel) delete workDetailsTableModel;

    delete ui;
}

void Work::setDB(const QSqlDatabase &db)
{
    this->db = db;

    if(workerTableModel) delete workerTableModel;           //Usun wskaznik jezeli zostal wczesniej zainnicjalizowany
    workerTableModel = new QSqlTableModel(0, db);

    workerTableModel->setTable(Settings::workerListTableName());
    workerTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    workerTableModel->setSort(1, Qt::AscendingOrder);
    workerTableModel->select();

    ui->worker->setModel(workerTableModel);
    ui->worker->setModelColumn(1);

    if(workDetailsTableModel) delete workDetailsTableModel;           //Usun wskaznik jezeli zostal wczesniej zainnicjalizowany
    workDetailsTableModel = new QSqlTableModel(0, db);

    workDetailsTableModel->setTable(Settings::storeTransTableName());
    workDetailsTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    workDetailsTableModel->setSort(1, Qt::AscendingOrder);
    workDetailsTableModel->select();

    //Zmień nazwy kolumnn
    workDetailsTableModel->setHeaderData(1, Qt::Horizontal, tr("Nazwa"));
    workDetailsTableModel->setHeaderData(2, Qt::Horizontal, tr("Symbol"));
    workDetailsTableModel->setHeaderData(3, Qt::Horizontal, tr("Ilość"));
    workDetailsTableModel->setHeaderData(4, Qt::Horizontal, tr("Jednostka"));

    ui->workDetails->setModel(workDetailsTableModel);

    // Ukryj niepotrzebne
    ui->workDetails->setColumnHidden(0, true);
    ui->workDetails->setColumnHidden(4, true);
    ui->workDetails->setColumnHidden(5, true);
    ui->workDetails->setColumnHidden(6, true);


    //Przelicz szerokosc kolumn
    float w = ui->workDetails->width()/3;
    ui->workDetails->setColumnWidth(1, (int)w*0.9);
    ui->workDetails->setColumnWidth(2, (int)w*0.9);
    ui->workDetails->setColumnWidth(3, (int)w*0.9);
    ui->workDetails->setColumnWidth(4, (int)w*0.9);
}

void Work::show()
{
    if(workerTableModel) workerTableModel->select();
    if(workDetailsTableModel){

        //Ustaw filtr tylko na prtzeglądaną pracę
        if(!workID.isEmpty()) workDetailsTableModel->setFilter("number like '" + workID + "%'");
        workDetailsTableModel->select();
    }

    //Ustaw pracownika, jeżeli jest przypisany do pracy (-1 -- wartość domyślna, 0 -- brak przypisania)
    if(workerID > 0)
    {
        ui->worker->setCurrentIndex( 1);
    }

    QDialog::show();
}


void Work::setWorkID(QString workID)
{
    this->workID = workID;
    ui->workID->setText(workID);
    workDetailsTableModel->setFilter("number like '" + workID + "%'");
}

void Work::setWorkerID(int workerID)
{
    this->workerID = workerID;
}

void Work::addProduct()
{
    qDebug() << "ADD";
}

void Work::deleteProduct()
{
    qDebug() << "DEL";
}

void Work::accept()
{
    //Zmieniona nazwisko pracownika w ComboBox
    //wprowadz zmiany do bazy danych
    if(workerChanged)
    {
        //Pobierz ID użytkownika ustawionego w ComboBox
        int row = ui->worker->currentIndex();
        int id = workerTableModel->index(row, Settings::DB::workerListTableName::id).data().toInt();

        if(workerID != id)
        {
            qDebug() << "!!!!!";
        }
    }

}

void Work::reject()
{
    this->hide();
}

void Work::on_worker_currentIndexChanged(int index)
{
    //Uległo zmianie nazwisko proacownia
    if(this->isVisible()) workerChanged = true;
}




