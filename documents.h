#ifndef DOCUMENTS_H
#define DOCUMENTS_H

#include <QDialog>

namespace Ui {
class Documents;
}

class Documents : public QDialog
{
    Q_OBJECT

public:
    explicit Documents(QWidget *parent = 0);
    ~Documents();

private:
    Ui::Documents *ui;
};

#endif // DOCUMENTS_H
