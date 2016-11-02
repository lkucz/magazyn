#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("serwer1420936.home.pl");
    db.setUserName("14455493_0000003");
    db.setPassword("Produkcja123");
    db.setDatabaseName("14455493_0000003");
    db.setPort(3306);

    if(!db.open())
    {
        QMessageBox mb;
        mb.setText("Nie mogę otworzyć bazy danych !");
        mb.exec();
        mb.setIcon(QMessageBox::Critical);
    }

    ui->setupUi(this);

    productDict = new Dictionary(parent);
    typeDict = new Dictionary(parent);
    materialDict = new Dictionary(parent);
    finishDict = new Dictionary(parent);
    colorDict = new Dictionary(parent);
    unitDict = new Dictionary(parent);
    documentDict = new Dictionary(parent);

    productDef = new Product(parent);
    productDef->show();

    productDict->setDB(db);
    productDict->setTable("productDict");
    productDict->setWindowTitle("Słownik półproduktów");

    typeDict->setDB(db);
    typeDict->setTable("typeDict");
    typeDict->setWindowTitle("Słownik typów");

    materialDict->setDB(db);
    materialDict->setTable("materialDict");
    materialDict->setWindowTitle("Słownik materiałów");

    finishDict->setDB(db);
    finishDict->setTable("finishDict");
    finishDict->setWindowTitle("Słownik rodzajów wykończenia");

    colorDict->setDB(db);
    colorDict->setTable("colorDict");
    colorDict->setWindowTitle("Słownik kolorów");

    unitDict->setDB(db);
    unitDict->setTable("unitDict");
    unitDict->setWindowTitle("Słownik jednostek");

    documentDict->setDB(db);
    documentDict->setTable("documentDict");
    documentDict->setWindowTitle("Słownik rodzajów dokumentów");



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openProductDictWindow()
{
    productDict->show();
}

void MainWindow::openTypeDictWindow()
{
    typeDict->show();
}

void MainWindow::openMaterialDictWindow()
{
    materialDict->show();
}

void MainWindow::openFinishDictWindow()
{
    finishDict->show();
}

void MainWindow::openColorDictWindow()
{
    colorDict->show();
}

void MainWindow::openUnitDictWindow()
{
    unitDict->show();
}

void MainWindow::openDocumentDictWindow()
{
    documentDict->show();
}

