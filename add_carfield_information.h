#ifndef ADD_CARFIELD_INFORMATION_H
#define ADD_CARFIELD_INFORMATION_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTableView>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class Add_carfield_information;
}

class Add_carfield_information : public QWidget
{
    Q_OBJECT

public:
    explicit Add_carfield_information(QWidget *parent = nullptr);
    ~Add_carfield_information();

private slots:
    void on_pushButton_2_clicked();


    void on_addpushButton_clicked();

private:
    Ui::Add_carfield_information *ui;
    QSqlDatabase db;
};

#endif // ADD_CARFIELD_INFORMATION_H
