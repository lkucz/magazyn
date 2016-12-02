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

Production::Production(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Production)
{
    ui->setupUi(this);
}

Production::~Production()
{
    delete ui;
}

void Production::setDB(const QSqlDatabase &db)
{
    this->db = db;
}

void Production::show()
{
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
