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
    return QString("products");             //tabela z listą produktów
}

QString Settings::productViewName()
{
    return QString("productsList");         //widok z lista produkow, zawiera nazwy zamiast ID

}
