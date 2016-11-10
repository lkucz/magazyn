#include "product.h"
#include "ui_product.h"
#include "settings.h"

Product::Product(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Product)
{
    ui->setupUi(this);

    nameTableModel = 0;
    typeTableModel = 0;
    materialTableModel = 0;
    finishTableModel = 0;
    colorTableModel = 0;
}

void Product::setDB(const QSqlDatabase &db)
{
    //Usun wskazniki jezeli zostaly wczesniej zainnicjalizowany

    if(nameTableModel) delete nameTableModel;
    if(typeTableModel) delete typeTableModel;
    if(materialTableModel) delete materialTableModel;
    if(finishTableModel) delete finishTableModel;
    if(colorTableModel) delete colorTableModel;

    nameTableModel = new QSqlTableModel(0, db);
    typeTableModel = new QSqlTableModel(0, db);
    materialTableModel = new QSqlTableModel(0, db);
    finishTableModel = new QSqlTableModel(0, db);
    colorTableModel = new QSqlTableModel(0, db);

    //nameTableModel->setTable();
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
