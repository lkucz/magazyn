#include "settings.h"

Settings::Settings()
{

}

QString Settings::colorDictTableName()
{
    return QString("colorDict");
}

QString Settings::documentDictTableName()
{
    return QString("documentDict");
}

QString Settings::finishDictTableName()
{
    return QString("finishDict");
}

QString Settings::materialDictTableName()
{
    return QString("materialDict");
}

QString Settings::productDictTableName()
{
    return QString("productDict");
}

QString Settings::typeDictTableName()
{
 return QString("typeDict");
}

QString Settings::unitDictTableName()
{
    return QString("unitDict");
}

QString Settings::stateDictTableName()
{
    return QString("stateDict");
}

QString Settings::productTableName()
{
    return QString("products");
}
