#ifndef ADD_CARFIELD_RENTAL_H
#define ADD_CARFIELD_RENTAL_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTableView>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QDateEdit>

namespace Ui {
class add_carfield_rental;
}

class add_carfield_rental : public QWidget
{
    Q_OBJECT

public:
    explicit add_carfield_rental(QWidget *parent = nullptr);
    ~add_carfield_rental();

private slots:
    void on_submit_pushButton_clicked();

private:
    Ui::add_carfield_rental *ui;
    QSqlDatabase db;
};

#endif // ADD_CARFIELD_RENTAL_H
