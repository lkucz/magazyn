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
