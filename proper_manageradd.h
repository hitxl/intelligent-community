#ifndef PROPER_MANAGERADD_H
#define PROPER_MANAGERADD_H

#include <QMainWindow>
#include <QSqlDatabase>
namespace Ui {
class Proper_Manageradd;
}

class Proper_Manageradd : public QMainWindow
{
    Q_OBJECT

public:
    explicit Proper_Manageradd(QWidget *parent = nullptr,QSqlDatabase *db = nullptr);
    ~Proper_Manageradd();
signals:
    void employeeAdded();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::Proper_Manageradd *ui;
    QSqlDatabase *m_db;
};

#endif // PROPER_MANAGERADD_H
