#include "preparework.h"
#include "ui_preparework.h"

PrepareWork::PrepareWork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrepareWork)
{
    ui->setupUi(this);
}

PrepareWork::~PrepareWork()
{
    delete ui;
}
