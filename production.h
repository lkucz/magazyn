#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <QDialog>

namespace Ui {
class Production;
}

class Production : public QDialog
{
    Q_OBJECT

public:
    explicit Production(QWidget *parent = 0);
    ~Production();

private:
    Ui::Production *ui;
};

#endif // PRODUCTION_H
