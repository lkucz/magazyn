#include <QSqlRecord>
#include "worker.h"
#include "ui_worker.h"

Worker::Worker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Worker)
{
    ui->setupUi(this);
    tm = 0;
}

Worker::~Worker()
{
    delete ui;
    if(tm) delete tm;
}

void Worker::setDB(const QSqlDatabase &db)
{
    if(tm) delete tm;           //Usun wskaznik jezeli zostal wczesniej zainnicjalizowany
    tm = new QSqlTableModel(0, db);
}

void Worker::setTable(const QString &tableName)
{
    if(tm)
    {
        tm->setTable(tableName);
        tm->setEditStrategy(QSqlTableModel::OnFieldChange);
        tm->select();
    }
}

void Worker::setWindowTitle(const QString &title)
{
    windowTitle = title;
    QDialog::setWindowTitle(windowTitle);
}

void Worker::accept()
{
    QSqlRecord newRecord;

    newRecord = tm->record();
    newRecord.setGenerated("id", true);
    newRecord.setValue("name", QVariant(ui->nameLineEdit->text()));
    newRecord.setValue("surname", QVariant(ui->surnameLineEdit->text()));
    newRecord.setValue("address", QVariant(ui->addressLineEdit->text()));
    newRecord.setValue("city", QVariant(ui->cityLineEdit->text()));
    newRecord.setValue("phone", QVariant(ui->phoneLineEdit->text()));
    //    newRecord.setValue("comment", QVariant(ui->commentTextEdit->text()));


    this->hide();
}

void Worker::reject()
{
    ui->nameLineEdit->clear();
    ui->surnameLineEdit->clear();
    ui->addressLineEdit->clear();
    ui->cityLineEdit->clear();
    ui->phoneLineEdit->clear();
    ui->commentTextEdit->clear();

    this->hide();
}
