#ifndef UPDATE_CARFIELD_RENTAL_INFORMATION_H
#define UPDATE_CARFIELD_RENTAL_INFORMATION_H

#include <QWidget>
#include <QString>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTableView>
#include <QDebug>
#include <QSqlError>

namespace Ui {
class update_carfield_rental_information;
}

class update_carfield_rental_information : public QWidget
{
    Q_OBJECT

public:
    explicit update_carfield_rental_information(QWidget *parent = nullptr);
    ~update_carfield_rental_information();

private slots:
    void on_pushButton_clicked();

private:
    Ui::update_carfield_rental_information *ui;
    QString m_ownerName;  // 业主姓名
    QString m_rentalId;   // 订单ID
    QSqlDatabase db;
};

#endif // UPDATE_CARFIELD_RENTAL_INFORMATION_H
