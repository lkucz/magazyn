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
    this->setWindowTitle("Magazyn");

    // Nie inicjalizuj nowego okna users do momentu, kiedy nie bedzie potrzebne
    // zerowanie wskaźników do obiektów okien
    productDictWindow = 0;
    typeDictWindow = 0;
    materialDictWindow = 0;
    finishDictWindow = 0;
    colorDictWindow = 0;
    unitDictWindow = 0;
    documentDictWindow = 0;

    usersWindow = 0;
    workersWindow = 0;
    productListWindow = 0;
    prepareWorkWindow = 0;
    addToStoreWindow = 0;
}

MainWindow::~MainWindow()
{
    if(productDictWindow) delete productDictWindow;
    if(typeDictWindow) delete productDictWindow;
    if(materialDictWindow) delete materialDictWindow;
    if(finishDictWindow) delete finishDictWindow;
    if(colorDictWindow) delete colorDictWindow;
    if(unitDictWindow) delete unitDictWindow;
    if(documentDictWindow) delete documentDictWindow;

    if(usersWindow) delete usersWindow;
    if(workersWindow) delete workersWindow;
    if(productListWindow) delete productListWindow;
    if(prepareWorkWindow) delete prepareWorkWindow;
    if(addToStoreWindow) delete addToStoreWindow;

    delete ui;
}

void MainWindow::openProductDictWindow()
{
    if(!productDictWindow){
        productDictWindow = new Dictionary(this);
        productDictWindow->setDB(db);
        productDictWindow->setTable("productDict");
        productDictWindow->setWindowTitle("Słownik półproduktów");
    }
    productDictWindow->show();
}

void MainWindow::openTypeDictWindow()
{
    if(!typeDictWindow){
        typeDictWindow = new Dictionary(this);
        typeDictWindow->setDB(db);
        typeDictWindow->setTable("typeDict");
        typeDictWindow->setWindowTitle("Słownik typów");
    }
    typeDictWindow->show();
}

void MainWindow::openMaterialDictWindow()
{
    if(!materialDictWindow){
        materialDictWindow = new Dictionary(this);
        materialDictWindow->setDB(db);
        materialDictWindow->setTable("materialDict");
        materialDictWindow->setWindowTitle("Słownik materiałów");
    }
    materialDictWindow->show();
}

void MainWindow::openFinishDictWindow()
{
    if(!finishDictWindow){
        finishDictWindow = new Dictionary(this);
        finishDictWindow->setDB(db);
        finishDictWindow->setTable("finishDict");
        finishDictWindow->setWindowTitle("Słownik rodzajów wykończenia");
    }
    finishDictWindow->show();
}

void MainWindow::openColorDictWindow()
{
    if(!colorDictWindow){
        colorDictWindow = new Dictionary(this);
        colorDictWindow->setDB(db);
        colorDictWindow->setTable("colorDict");
        colorDictWindow->setWindowTitle("Słownik kolorów");
    }
    colorDictWindow->show();
}

void MainWindow::openUnitDictWindow()
{
    if(!unitDictWindow){
        unitDictWindow = new Dictionary(this);
        unitDictWindow->setDB(db);
        unitDictWindow->setTable("unitDict");
        unitDictWindow->setWindowTitle("Słownik jednostek");
    }
    unitDictWindow->show();
}

void MainWindow::openDocumentDictWindow()
{
    if(!documentDictWindow){
        documentDictWindow = new Dictionary(this);
        documentDictWindow->setDB(db);
        documentDictWindow->setTable("documentDict");
        documentDictWindow->setWindowTitle("Słownik rodzajów dokumentów");
    }
    documentDictWindow->show();
}

void MainWindow::openUsersWindow()
{
    if(!usersWindow){
        usersWindow = new Users(this);
        usersWindow->setDB(db);
        usersWindow->setTable("user");
        usersWindow->setWindowTitle("Użytkownicy systemu");
    }
    usersWindow->show();
}

void MainWindow::openWorkersWindow()
{
    if(!workersWindow){
        workersWindow = new Workers(this);
        workersWindow->setDB(db);
        workersWindow->setTable("worker_view");
        workersWindow->setWindowTitle("Lista pracowników");
    }
    workersWindow->show();
}

void MainWindow::openProductListWindow()
{
    if(!productListWindow){
        productListWindow = new ProductList(this);
        productListWindow->setDB(db);
        productListWindow->setWindowTitle("Słownik produktów");
    }
    productListWindow->show();
}

void MainWindow::openPrepareWorkWindow()
{
    if(!prepareWorkWindow){
        prepareWorkWindow = new PrepareWork(this);
        prepareWorkWindow->setDB(db);
        prepareWorkWindow->setWindowTitle("Przygotowanie pracy");
    }
    prepareWorkWindow->show();

}

void MainWindow::openAddToStoreWindow()
{
    if(!addToStoreWindow){
        addToStoreWindow = new AddToStore(this);
        addToStoreWindow->setDB(db);
        addToStoreWindow->setWindowTitle("Wprowadź produkty na magazyn");
    }
    addToStoreWindow->show();

}
