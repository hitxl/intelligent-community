#ifndef PROPERTY_STAFFADD_H
#define PROPERTY_STAFFADD_H

#include <QMainWindow>
#include <QSqlDatabase>
namespace Ui {
class Property_Staffadd;
}

class Property_Staffadd : public QMainWindow
{
    Q_OBJECT

public:
    explicit Property_Staffadd(QWidget *parent = nullptr,QSqlDatabase *db = nullptr);
    ~Property_Staffadd();
signals:
    void staffAdded();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Property_Staffadd *ui;
    QSqlDatabase *m_db;
};

#endif // PROPERTY_STAFFADD_H
