#include "proper_manageradd.h"
#include "ui_proper_manageradd.h"
#include <QMessageBox>
#include <QSqlQuery>
#include<QSqlError>

Proper_Manageradd::Proper_Manageradd(QWidget *parent, QSqlDatabase *db)
    : QMainWindow(parent)
{
    ui = new Ui::Proper_Manageradd;
    m_db = db;
    ui->setupUi(this);

}

Proper_Manageradd::~Proper_Manageradd()
{
    delete ui;
}

void Proper_Manageradd::on_pushButton_clicked()
{
    QString user_id = ui->lineEdit_3->text().trimmed(); // 假设 lineEdit_3 是用户ID输入框
    QString username = ui->lineEdit_2->text().trimmed(); // 假设 lineEdit_2 是用户名输入框
    QString password = ui->lineEdit_4->text().trimmed(); // 假设 lineEdit_4 是密码输入框
    QString phone = ui->lineEdit->text().trimmed(); // 需确保界面有电话输入框，命名为 lineEdit_Phone
    QString user_type = ui->comboBox_userType->currentText();  // 假设 lineEdit_UserType 是身份权限输入框

    // 2. 非空校验
    if (user_id.isEmpty() || username.isEmpty() || password.isEmpty() || phone.isEmpty() || user_type.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "用户ID、用户名、密码、电话、身份权限为必填项，不能为空！");
        return;
    }

    // 3. 插入数据库，包含五个必填字段
    QSqlQuery query(*m_db);
    query.prepare("INSERT INTO USER_ACCOUNT (user_id, username, password, phone, user_type,register_time) "
                  "VALUES (:user_id, :username, :password, :phone, :user_type,CURRENT_TIMESTAMP)");
    query.bindValue(":user_id", user_id);
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":phone", phone);
    query.bindValue(":user_type", user_type);

    if (query.exec()) {
        QMessageBox::information(this, "成功", "用户添加成功！");
        emit employeeAdded();
        ui->lineEdit_2->clear();
        ui->lineEdit_4->clear();
        ui->lineEdit_3->clear();

        ui->lineEdit->clear();
        this->close();
    } else {
        QMessageBox::critical(this, "数据库错误",
                              "无法添加用户：" + query.lastError().text());
        ui->lineEdit_2->clear();
        ui->lineEdit_4->clear();
        ui->lineEdit_3->clear();

        ui->lineEdit->clear();
    }
}

void Proper_Manageradd::on_pushButton_2_clicked()
{
    this->close();
}
