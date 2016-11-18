#include <QMessageBox>
#include <QDebug>
#include <QSqlRecord>

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

    productTableModel = 0;
}

Product::~Product()
{
    delete ui;

    if(nameTableModel) delete nameTableModel;
    if(typeTableModel) delete typeTableModel;
    if(materialTableModel) delete materialTableModel;
    if(finishTableModel) delete finishTableModel;
    if(colorTableModel) delete colorTableModel;
    if(productTableModel) delete productTableModel;
}

void Product::setDB(const QSqlDatabase &db)
{
    //Usun wskazniki jezeli zostaly wczesniej zainnicjalizowany

    if(nameTableModel) delete nameTableModel;
    if(typeTableModel) delete typeTableModel;
    if(materialTableModel) delete materialTableModel;
    if(finishTableModel) delete finishTableModel;
    if(colorTableModel) delete colorTableModel;
    if(productTableModel) delete productTableModel;

    nameTableModel = new QSqlTableModel(0, db);
    typeTableModel = new QSqlTableModel(0, db);
    materialTableModel = new QSqlTableModel(0, db);
    finishTableModel = new QSqlTableModel(0, db);
    colorTableModel = new QSqlTableModel(0, db);
    productTableModel = new QSqlTableModel(0, db);

    nameTableModel->setTable(Settings::productDictTableName());
    nameTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    nameTableModel->setSort(1, Qt::AscendingOrder);
    ui->productCB->setModel(nameTableModel);
    ui->productCB->setModelColumn(1);

    typeTableModel->setTable(Settings::typeDictTableName());
    typeTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    typeTableModel->setSort(1, Qt::AscendingOrder);
    ui->typeCB->setModel(typeTableModel);
    ui->typeCB->setModelColumn(1);

    materialTableModel->setTable(Settings::materialDictTableName());
    materialTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    materialTableModel->setSort(1, Qt::AscendingOrder);
    ui->materialCB->setModel(materialTableModel);
    ui->materialCB->setModelColumn(1);

    finishTableModel->setTable(Settings::finishDictTableName());
    finishTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    finishTableModel->setSort(1, Qt::AscendingOrder);
    ui->finishCB->setModel(finishTableModel);
    ui->finishCB->setModelColumn(1);

    colorTableModel->setTable(Settings::colorDictTableName());
    colorTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    colorTableModel->setSort(1, Qt::AscendingOrder);
    ui->colorCB->setModel(colorTableModel);
    ui->colorCB->setModelColumn(1);

    productTableModel->setTable(Settings::productTableName());
    productTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    productTableModel->setSort(0, Qt::AscendingOrder);
    productTableModel->select();

}

void Product::setWindowTitle(const QString &title)
{
    windowTitle = title;
    QDialog::setWindowTitle(windowTitle);
}

void Product::show()
{
    if(nameTableModel) nameTableModel->select();
    if(typeTableModel) typeTableModel->select();
    if(materialTableModel) materialTableModel->select();
    if(finishTableModel) finishTableModel->select();
    if(colorTableModel) colorTableModel->select();

    QDialog::show();
}


void Product::accept()
{
    QMessageBox mb;
    QSqlRecord newRecord;

    //Walidacja nazwy produktu
    if(ui->productCB->currentIndex() == -1 || ui->productCB->currentIndex() == 0)
    {
        mb.setText("Nie została wybrana nazwa produktu.");
        mb.setIcon(QMessageBox::Warning);
        mb.setStandardButtons(QMessageBox::Ok);
        mb.setDefaultButton(QMessageBox::Ok);
        mb.exec();

        ui->productCB->setFocus();
        return ;
    }

    //Walidacja symbolu
    if(ui->symbol->text().isEmpty() == true)
    {
        mb.setText("Nie został ustawiony symbol produktu.");
        mb.setIcon(QMessageBox::Warning);
        mb.setStandardButtons(QMessageBox::Ok);
        mb.setDefaultButton(QMessageBox::Ok);
        mb.exec();

        ui->symbol->setFocus();
        return ;

    }
    else
    {
        //Sprawdz czy dany symbol nie istnieje w bazie.

        QString filter = "symbol like '" +  ui->symbol->text() + "'";
        int rowCount = 0;

        productTableModel->setFilter(filter);       // Ustaw filtr
        rowCount = productTableModel->rowCount();   // ... sprawdz ilosc rekordow pasujacych do filtra
        productTableModel->setFilter("");           // ... usun filtr

        if( rowCount!= 0)
        {
            mb.setText("Podany symbol już istnieje w bazie danych.");
            mb.setIcon(QMessageBox::Warning);
            mb.setStandardButtons(QMessageBox::Ok);
            mb.setDefaultButton(QMessageBox::Ok);
            mb.exec();

            ui->symbol->setFocus();
            return ;
        }
    }

    //Walidacja alarmow
    if(ui->minQt->value() == 0 || ui->maxQt->value() == 0)
    {
        mb.setText("Minimalny lub maksymalny stan produktu jest ustawiony na 0." \
                   "Nie będzie możliwe ustawienie alarmów dla danego produktu. " \
                   "Czy chcesz to zmienić?");
        mb.setIcon(QMessageBox::Question);
        mb.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        mb.setDefaultButton(QMessageBox::Yes);

        if(mb.exec() == QMessageBox::Yes)
        {
            if(ui->maxQt->value() == 0) ui->maxQt->setFocus();
            if(ui->minQt->value() == 0) ui->minQt->setFocus();

            return ;
        }
    }
    else
    {
        //Sprawdz czy alarm jest ustawiony poprawnie
        if(ui->minQt->value() > ui->maxQt->value())
        {
            mb.setText("Wartość minimalna jest więkasz niż maksymalna.");
            mb.setIcon(QMessageBox::Warning);
            mb.setStandardButtons(QMessageBox::Ok);
            mb.setDefaultButton(QMessageBox::Ok);
            mb.exec();

            ui->minQt->setFocus();
            return ;
        }
    }

    //Walidacja przelicznika
    if(ui->qtPerKg->text().isEmpty() == true)
    {
        mb.setText("Nie został ustawiony przelicznik kg na szt. Czy chcesz to zmienić?");
        mb.setIcon(QMessageBox::Question);
        mb.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        mb.setDefaultButton(QMessageBox::Yes);
        if(mb.exec() == QMessageBox::Yes)
        {
            ui->qtPerKg->setFocus();
            return ;
        }
    }
    else
    {
        //Sprawdz czy licznik jest ustawiony na wartosc wieksza od 0
        if(ui->qtPerKg->text().toFloat() <= 0)
        {
            mb.setText("Wartość przelicznika kg na szt musi być wartością większą od 0.");
            mb.setIcon(QMessageBox::Warning);
            mb.setStandardButtons(QMessageBox::Ok);
            mb.setDefaultButton(QMessageBox::Ok);
            mb.exec();

            ui->qtPerKg->setFocus();
            return ;
        }
    }

    int row;
    int column = 0; //Numer kolumny dla pola 'id' jest rowny 0 dla  wszystkich tabeli typu Dictionary

    //Utworz nowy, pusty rekord i wygeneruj automatycznie 'id'
    newRecord = productTableModel->record();
    newRecord.setGenerated("id", true);
    newRecord.setValue("symbol", ui->symbol->text());
    newRecord.setValue("alarm_min", ui->minQt->text());
    newRecord.setValue("alarm_max", ui->maxQt->text());
    newRecord.setValue("upw", ui->qtPerKg->text());

    //Odczytaj wartosc combobox nazwy produktu
    row = ui->productCB->currentIndex();
    newRecord.setValue("name", nameTableModel->index(row, column).data());

    //Odczytaj wartosc pola 'typ' jezeli jest ustawione
    if((row = ui->typeCB->currentIndex()) != -1)
        newRecord.setValue("type", typeTableModel->index(row, column).data());
    else
        newRecord.setValue("type", QVariant("0"));

    //Odczytaj wartosc pola 'material' jezeli jest ustawione
    if((row = ui->materialCB->currentIndex()) != -1)
        newRecord.setValue("material", materialTableModel->index(row, column).data());
    else
        newRecord.setValue("material", QVariant("0"));

    //Odczytaj wartosc pola 'wykonczenie' jezeli jest ustawione
    if((row = ui->finishCB->currentIndex()) != -1)
        newRecord.setValue("finish", finishTableModel->index(row, column).data());
    else
        newRecord.setValue("finish", QVariant("0"));

    //Odczytaj wartosc pola 'kolor' jezeli jest ustawione
    if((row = ui->colorCB->currentIndex()) != -1)
        newRecord.setValue("color", colorTableModel->index(row, column).data());
    else
        newRecord.setValue("color", QVariant("0"));

    if( productTableModel->insertRecord(-1, newRecord) != true )
    {
        qDebug() << "Problem z wstawieniem rekordu do tabeli " << Settings::productDictTableName();
        qDebug() << newRecord;
    }

    // Przeslij rekord do bazy
    productTableModel->submitAll();
    this->hide();
}

void Product::reject()
{
    this->hide();
}
