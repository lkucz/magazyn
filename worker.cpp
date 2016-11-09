#include "worker.h"
#include "ui_worker.h"

Worker::Worker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Worker)
{
    ui->setupUi(this);
}

Worker::~Worker()
{
    delete ui;
}
