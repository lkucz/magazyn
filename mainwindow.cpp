#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dict = new Dictionary(parent);

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

    dict->setDB(db);
    dict->setTable("color");
    dict->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
