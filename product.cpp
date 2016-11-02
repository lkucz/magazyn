#include "product.h"
#include "ui_product.h"

Product::Product(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Product)
{
    ui->setupUi(this);
}

void Product::setDB(const QSqlDatabase &db)
{
    if(tm) delete tm;           //Usun wskaznik jezeli zostal wczesniej zainnicjalizowany
    tm = new QSqlTableModel(0, db);
}

void Product::setWindowTitle(const QString &title)
{
    windowTitle = title;
    QDialog::setWindowTitle(windowTitle);
}

void Product::show()
{
//    if(tm)
//    {
//        tm->select();    //Wykonaj zapytanie przed otwqarciem okna
//        ui->listView->setModel(tm);
//        ui->listView->setModelColumn(1);
//    }
    QDialog::show();
}

Product::~Product()
{
    delete ui;
}
