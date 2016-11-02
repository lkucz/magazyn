#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>

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
    ProductList *productList;

    Dictionary *productDict;
    Dictionary *typeDict;
    Dictionary *materialDict;
    Dictionary *finishDict;
    Dictionary *colorDict;
    Dictionary *unitDict;
    Dictionary *documentDict;

    QSqlDatabase db;

private slots:
    void openProductDictWindow();
    void openTypeDictWindow();
    void openMaterialDictWindow();
    void openFinishDictWindow();
    void openColorDictWindow();
    void openUnitDictWindow();
    void openDocumentDictWindow();

};

#endif // MAINWINDOW_H
