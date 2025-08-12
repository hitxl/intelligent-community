#ifndef ANNOUNCE1_H
#define ANNOUNCE1_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMainWindow>

namespace Ui {
class announce1;
}

class announce1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit announce1(QWidget *parent = nullptr);
    ~announce1();

private slots:
    void on_pushButton_clicked();

private:
    Ui::announce1 *ui;
    QSqlDatabase db;
};

#endif // ANNOUNCE1_H
