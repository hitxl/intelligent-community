#include "owner_pay.h"
#include "ui_owner_pay.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QFont>
#include <QLineEdit>
#include <QDateTime>
#include <QMessageBox>
Owner_Pay::Owner_Pay(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Owner_Pay)
{
    ui->setupUi(this);
    resize(800, 800);
    //链接数据库
    QSqlDatabase db;
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("IntelligentCommunity.db");
    }
    if (!db.open()) {
        QMessageBox::critical(this, "连接失败", "数据库打开失败：" + db.lastError().text());
    } else {
        QMessageBox::information(this, "连接成功", "数据库连接成功");
    }
}

Owner_Pay::~Owner_Pay()
{
    delete ui;
}

void Owner_Pay::on_pushButton_3_clicked()
{
    this->close();

}

void Owner_Pay::on_pushButton_2_clicked()
{
    QString owner_id = ui->lineEdit->text().trimmed(); // 获取输入的业主ID并去除空白

    // 检查输入是否为空
    if (owner_id.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入业主ID", QMessageBox::Ok);
        return;
    }

    // 执行数据库更新操作
    QSqlQuery query;
    query.prepare("UPDATE PAYMENT_NEW SET payment_status = 'paid' WHERE owner_id = :owner_id");
    query.bindValue(":owner_id", owner_id);

    if (query.exec()) {
        int rowsAffected = query.numRowsAffected();
        if (rowsAffected > 0) {
            QMessageBox::information(this, "支付成功", "缴费状态已更新为已支付", QMessageBox::Ok);
        } else {
            QMessageBox::warning(this, "未找到记录", "未找到对应业主ID的缴费记录", QMessageBox::Ok);
        }
    } else {
        QMessageBox::critical(this, "数据库错误", "更新缴费状态失败: " + query.lastError().text(), QMessageBox::Ok);
    }
}
