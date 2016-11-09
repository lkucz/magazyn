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

    productList = new ProductList(parent);
    productList->setDB(db);
    productList->setWindowTitle("Słownik produktów");
    productList->setTable("productsList");
    productList->show();

    //    productDef = new Product(parent);
    //    productDef->setDB(db);
    //    productDef->setWindowTitle("Definicja produktu");
    //    productDef->show();

    users = 0;      //Nie inicjalizuj nowego okna users do momentu, kiedy nie bedzie potrzebne
    workers = 0;
}

MainWindow::~MainWindow()
{
    if(users) delete users;
    if(workers) delete workers;

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

void MainWindow::openUsersWindow()
{
    if(!users){
        users = new Users(this);
        users->setDB(db);
        users->setTable("user");
        users->setWindowTitle("Użytkownicy systemu");
    }
    users->show();
}

void MainWindow::openWorkersWindow()
{
    if(!workers){
        workers = new Workers(this);
        workers->setDB(db);
        workers->setTable("worker_view");
        workers->setWindowTitle("Lista pracowników");
    }
    workers->show();
}

