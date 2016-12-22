#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

class Settings
{
private:

public:
    Settings();

    static QString colorDictTableName();
    static QString documentDictTableName();
    static QString finishDictTableName();
    static QString materialDictTableName();
    static QString productDictTableName();
    static QString typeDictTableName();
    static QString unitDictTableName();
    static QString stateDictTableName();
    static QString productTableName();
    static QString productViewName();
    static QString documentTableName();
    static QString storeTableName();
    static QString storeQtyTableName();
    static QString storeTransTableName();
    static QString documentListTableName();

    static int workDocumentID();
};

#endif // SETTINGS_H
