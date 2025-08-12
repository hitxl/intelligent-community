#ifndef PROPERTY_STAFFDEL_H
#define PROPERTY_STAFFDEL_H

#include <QMainWindow>
#include <QSqlDatabase>

namespace Ui {
class Property_Staffdel;
}

class Property_Staffdel : public QMainWindow
{
    Q_OBJECT

public:
    explicit Property_Staffdel(QWidget *parent = nullptr, QSqlDatabase *db = nullptr);
    ~Property_Staffdel();

signals:
    void staffDeleted();

private slots:
    void on_pushButton_clicked();    // 确认按钮
    void on_pushButton_2_clicked();  // 取消按钮

private:
    Ui::Property_Staffdel *ui;
    QSqlDatabase *m_db;
};

#endif // PROPERTY_STAFFDEL_H
