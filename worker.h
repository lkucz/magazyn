#ifndef WORKER_H
#define WORKER_H

#include <QDialog>

namespace Ui {
class Worker;
}

class Worker : public QDialog
{
    Q_OBJECT

public:
    explicit Worker(QWidget *parent = 0);
    ~Worker();

private:
    Ui::Worker *ui;
};

#endif // WORKER_H
