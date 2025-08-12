#include "property_staffdel.h"
#include "ui_property_staffdel.h"
#include <QMessageBox>
#include <QSqlQuery>
#include<QSqlError>
Property_Staffdel::Property_Staffdel(QWidget *parent, QSqlDatabase *db) :
    QMainWindow(parent),
    ui(new Ui::Property_Staffdel),
    m_db(db)
{
    ui->setupUi(this);
    setWindowTitle("删除员工");

}

Property_Staffdel::~Property_Staffdel()
{
    delete ui;
}

void Property_Staffdel::on_pushButton_clicked()  // 确认按钮点击
{
    QString staffId = ui->lineEdit_3->text().trimmed();
    if (staffId.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入员工ID！");
        return;
    }



    QSqlQuery query(*m_db);
    query.prepare("DELETE FROM STAFF WHERE staff_id = :staff_id");
    query.bindValue(":staff_id", staffId);

    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            QMessageBox::information(this, "删除成功", "员工信息已成功删除！");
            emit staffDeleted();  // 发送删除成功信号
            ui->lineEdit_3->clear();
            ui->lineEdit_5->clear();
            this->close();
        } else {
            QMessageBox::warning(this, "删除失败", "未找到对应员工ID的记录！");
            ui->lineEdit_3->clear();
            ui->lineEdit_5->clear();
        }
    } else {
        QMessageBox::critical(this, "数据库错误", "删除失败：" + query.lastError().text());
        ui->lineEdit_3->clear();
        ui->lineEdit_5->clear();
    }
}

void Property_Staffdel::on_pushButton_2_clicked()  // 取消按钮点击
{
    ui->lineEdit_3->clear();
    ui->lineEdit_5->clear();
    this->close();
}
