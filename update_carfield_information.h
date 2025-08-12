#ifndef UPDATE_CARFIELD_INFORMATION_H
#define UPDATE_CARFIELD_INFORMATION_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTableView>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
namespace Ui {
class update_carfield_information;
}

class update_carfield_information : public QWidget
{
    Q_OBJECT

public:
    explicit update_carfield_information(QWidget *parent = nullptr);
    ~update_carfield_information();

private slots:
    void on_pushButton_clicked();

private:
    Ui::update_carfield_information *ui;
    QSqlDatabase db;
};

#endif // UPDATE_CARFIELD_INFORMATION_H
