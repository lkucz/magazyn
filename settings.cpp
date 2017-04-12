#include "settings.h"

Settings::Settings()
{

}

QString Settings::colorDictTableName()
{
    return QString("colorDict");                //slownik kolorow
}

QString Settings::documentDictTableName()
{
    return QString("documentDict");             //slownik dokumentow
}

QString Settings::finishDictTableName()
{
    return QString("finishDict");               //slownik rodzajow wykonczenia
}

QString Settings::materialDictTableName()
{
    return QString("materialDict");            //slownik materialow
}

QString Settings::productDictTableName()
{
    return QString("productDict");              //slownik polprodukow
}

QString Settings::typeDictTableName()
{
 return QString("typeDict");                    //slownik typow
}

QString Settings::unitDictTableName()
{
    return QString("unitDict");                 //slownik jednostek
}

QString Settings::stateDictTableName()
{
    return QString("stateDict");                //slownik stanow
}

QString Settings::productTableName()
{
    return QString("product");             //tabela z listą produktów
}

QString Settings::productViewName()
{
    return QString("productList");         //widok z lista produkow, zawiera nazwy zamiast ID

}

QString Settings::documentTableName()
{
    return QString("document");             //tabela z wprowadzonymi dokumentami FV, WZ, itd
}

QString Settings::storeTableName()
{
    return QString("store");            //główna tabela magazynu (wszystkie ruchy magazynu)
}

QString Settings::storeQtyTableName()
{
    return QString("storeQty");         //view z tabeli z podliczoną liczbą produktów (jeden produkt == jeden wpis)
}

QString Settings::storeTransTableName()
{
    return QString("storeTransaction"); //view z transakcjami na magazynie
}

QString Settings::documentListTableName()
{
    return QString("documentList");     // tabela z dokumentami -- wszytkie wprowadzone dokumenty (główna tabela z dokumentami)
}

QString Settings::productionTableName()
{
    return QString("production");     // tabela z produkcją -- główna tabela produkcji (prace przygotoweane, wydane, zakończone , anulowan)
}


QString Settings::productionListTableName()
{
    return QString("productionList");     // tabela z produkcją(view) id, numer pracy, stan, pracownik
}

QString Settings::workerListTableName()
{
    return QString("workerList");     // tabela z listą pracowników (view)
}



int Settings::workDocumentID(){ return 5; }         // Numer ID dokumentu. Potrzebny przy przygotowaniu/wydawaniu/odbieraniu prac !!! ZAKODOWANY NA SZTYWNO !!!
int Settings::defaultWorkerID(){ return 0; }         // Numer ID pracownika domyślnego. Potrzebne przy przygotowaniu prac bez wydawania  !!! ZAKODOWANY NA SZTYWNO !!!
int Settings::defaultUserID(){ return 0; }         // Numer ID użytkownika systemu !!! ZAKODOWANY NA SZTYWNO !!!

int Settings::workDefaultStateID(){ return 0; }
int Settings::workOnStock(){ return 1; }
int Settings::workAtProduction(){ return 2; }
int Settings::workReady(){ return 3; }
int Settings::workCanceled(){ return 4; }
int Settings::workReturned(){ return 5; }
