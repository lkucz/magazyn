#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>

#include "users.h"
#include "workers.h"
#include "dictionary.h"
#include "product.h"
#include "productlist.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Product *productDef;

    Dictionary *productDictWindow;
    Dictionary *typeDictWindow;
    Dictionary *materialDictWindow;
    Dictionary *finishDictWindow;
    Dictionary *colorDictWindow;
    Dictionary *unitDictWindow;
    Dictionary *documentDictWindow;

    QSqlDatabase db;

    Users *usersWindow;
    Workers *workersWindow;
    ProductList *productListWindow;


private slots:
    void openProductDictWindow();
    void openTypeDictWindow();
    void openMaterialDictWindow();
    void openFinishDictWindow();
    void openColorDictWindow();
    void openUnitDictWindow();
    void openDocumentDictWindow();
    void openUsersWindow();
    void openWorkersWindow();
    void openProductListWindow();
};

#endif // MAINWINDOW_H
