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
    static QString productionTableName();
    static QString productionListTableName();
    static QString workerListTableName();

    static int workDocumentID();
    static int defaultWorkerID();
    static int defaultUserID();

    static int workDefaultStateID();
    static int workOnStock();
    static int workAtProduction();
    static int workReady();
    static int workCanceled();
    static int workReturned();
};

#endif // SETTINGS_H
