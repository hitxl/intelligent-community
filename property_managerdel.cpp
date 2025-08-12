#include "property_managerdel.h"
#include "ui_property_managerdel.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QCloseEvent>

Property_Managerdel::Property_Managerdel(QWidget *parent, QSqlDatabase *db)
    : QMainWindow(parent)
    , ui(new Ui::Property_Managerdel)
    , m_db(db)
{
    ui->setupUi(this);
    setWindowTitle("删除用户账号");
    clearInputs(); // 初始化时先清空，确保首次打开也干净
}

Property_Managerdel::~Property_Managerdel()
{
    delete ui;
}


void Property_Managerdel::clearInputs()
{
    ui->lineEdit_3->clear();
    ui->lineEdit_5->clear();
}

// 确认删除按钮槽函数
void Property_Managerdel::on_pushButton_clicked()
{
    QString userId = ui->lineEdit_3->text().trimmed();
    if (userId.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入要删除的用户ID！");
        return;
    }

    QSqlQuery query(*m_db);
    query.prepare("DELETE FROM USER_ACCOUNT WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            QMessageBox::information(this, "删除成功", "用户账号已成功删除！");
            emit userDeleted();
            this->close();
            clearInputs(); // 出错时清空
        } else {
            QMessageBox::warning(this, "删除失败", "未找到对应用户ID的记录！");
            clearInputs(); // 删除失败时也清空，避免残留
        }
    } else {
        QMessageBox::critical(this, "数据库错误", "删除失败：" + query.lastError().text());
        clearInputs(); // 出错时清空
    }
}

// 取消按钮槽函数
void Property_Managerdel::on_pushButton_2_clicked()
{
    clearInputs(); // 取消时清空输入框
    this->close();
}
