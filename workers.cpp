#include "workers.h"
#include "ui_workers.h"

Workers::Workers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Workers)
{
    ui->setupUi(this);
}

Workers::~Workers()
{
    delete ui;
}
