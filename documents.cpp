#include "documents.h"
#include "ui_documents.h"

Documents::Documents(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Documents)
{
    ui->setupUi(this);
}

Documents::~Documents()
{
    delete ui;
}
