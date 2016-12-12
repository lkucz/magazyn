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
