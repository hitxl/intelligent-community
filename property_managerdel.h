#ifndef PROPERTY_MANAGERDEL_H
#define PROPERTY_MANAGERDEL_H

#include <QMainWindow>
#include "ui_property_managerdel.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>

class Property_Managerdel : public QMainWindow
{
    Q_OBJECT

public:
    explicit Property_Managerdel(QWidget *parent = nullptr, QSqlDatabase *db = nullptr);
    ~Property_Managerdel();

signals:
    void userDeleted(); // 用于通知主窗口刷新数据

private slots:
    void on_pushButton_clicked();   // 确认删除按钮
    void on_pushButton_2_clicked(); // 取消按钮
    void clearInputs();             // 清空输入框方法
private:
    Ui::Property_Managerdel *ui;
    QSqlDatabase *m_db;
};

#endif // PROPERTY_MANAGERDEL_H
