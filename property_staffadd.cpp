#include "property_staffadd.h"
#include "ui_property_staffadd.h"
#include <QMessageBox>
#include <QSqlQuery>
#include<QSqlError>
Property_Staffadd::Property_Staffadd(QWidget *parent,QSqlDatabase *db ) :
    QMainWindow(parent),
    ui(new Ui::Property_Staffadd)
{
    ui = new Ui::Property_Staffadd;
    m_db = db;
    ui->setupUi(this);

}

Property_Staffadd::~Property_Staffadd()
{
    delete ui;
}

void Property_Staffadd::on_pushButton_clicked()
{
    QString staff_id = ui->lineEdit_3->text().trimmed(); // 假设 lineEdit_3 是用户ID输入框
    QString user_id = ui->lineEdit_2->text().trimmed(); // 假设 lineEdit_2 是用户名输入框
    QString position = ui->lineEdit_4->text().trimmed(); // 假设 lineEdit_4 是密码输入框
    QString department = ui->lineEdit->text().trimmed(); // 需确保界面有电话输入框，命名为 lineEdit_Phone
    QString admin_id = ui->lineEdit_5->text().trimmed(); // 假设 lineEdit_UserType 是身份权限输入框



    // 3. 插入数据库，包含五个必填字段
    QSqlQuery query(*m_db);
    query.prepare("INSERT INTO STAFF (staff_id, user_id, position, department, admin_id) "
                  "VALUES (:staff_id, :user_id, :position, :department, :admin_id)");
    query.bindValue(":staff_id", staff_id);
    query.bindValue(":user_id", user_id);
    query.bindValue(":position", position);
    query.bindValue(":department", department);
    query.bindValue(":admin_id", admin_id);

    if (query.exec()) {
        QMessageBox::information(this, "成功", "员工添加成功！");
        ui->lineEdit_2->clear();
        ui->lineEdit_4->clear();
        ui->lineEdit_3->clear();
        ui->lineEdit_5->clear();
        ui->lineEdit->clear();

        emit staffAdded();
        this->close();
    } else {
        QMessageBox::critical(this, "数据库错误",
                              "无法添加员工：" + query.lastError().text());
        ui->lineEdit_2->clear();
        ui->lineEdit_4->clear();
        ui->lineEdit_3->clear();
        ui->lineEdit_5->clear();
        ui->lineEdit->clear();
    }
}

void Property_Staffadd::on_pushButton_2_clicked()
{
    this->close();
}
