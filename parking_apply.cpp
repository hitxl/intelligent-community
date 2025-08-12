#include "parking_apply.h"
#include "ui_parking_apply.h"
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
Parking_Apply::Parking_Apply(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Parking_Apply)
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

Parking_Apply::~Parking_Apply()
{
    delete ui;
}

void Parking_Apply::on_pushButton_2_clicked()
{
    this->close();
}

void Parking_Apply::on_pushButton_clicked()
{
    // 获取输入信息
    QString parkingspace_id = ui->lineEdit->text().trimmed();
    QString car_number = ui->lineEdit_2->text().trimmed().toUpper();
    QString ownername = ui->lineEdit_3->text().trimmed();

    // 获取日期选择器的值
    QDate startDate = ui->dateEdit->date();  // 添加的日期选择器
    QDate endDate = ui->dateEdit_2->date();      // 添加的日期选择器

    // 检查输入是否为空
    if (parkingspace_id.isEmpty() || car_number.isEmpty() || ownername.isEmpty() ||
        startDate.isNull() || endDate.isNull()) {
        QMessageBox::warning(this, "输入错误", "请填写完整申请信息", QMessageBox::Ok);
        return;
    }

    // 检查日期是否有效
    if (startDate >= endDate) {
        QMessageBox::warning(this, "日期错误", "开始日期必须早于结束日期", QMessageBox::Ok);
        return;
    }

    // 检查是否已有相同申请（相同车位ID和车牌号）
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT application_id FROM PARKING_APPLICATION_NEW "
                       "WHERE space_id = :space_id AND car_number = :car_number ");
    // "AND status != 'allocated'");  // 排除已拒绝的申请
    checkQuery.bindValue(":space_id", parkingspace_id);
    checkQuery.bindValue(":car_number", car_number);

    if (checkQuery.exec() && checkQuery.next()) {
        QMessageBox::warning(this, "重复申请", "该车位已存在申请记录", QMessageBox::Ok);
        return;
    } else if (checkQuery.lastError().isValid()) {
        QMessageBox::critical(this, "数据库错误", "查询申请记录失败: " + checkQuery.lastError().text());
        return;
    }

    // 执行插入操作，包含日期字段
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO PARKING_APPLICATION_NEW "
                        "(applicantname, space_id, car_number, start_rent_time, end_rent_time, status) "
                        "VALUES (:applicantname, :space_id, :car_number, :start_time, :end_time, 'pending')");
    insertQuery.bindValue(":space_id", parkingspace_id);
    insertQuery.bindValue(":car_number", car_number);
    insertQuery.bindValue(":applicantname", ownername);
    insertQuery.bindValue(":start_time", startDate.toString("yyyy-MM-dd"));  // 格式化日期
    insertQuery.bindValue(":end_time", endDate.toString("yyyy-MM-dd"));      // 格式化日期

    if (!insertQuery.exec()) {
        QMessageBox::critical(this, "数据库错误", "申请失败: " + insertQuery.lastError().text());
        return;
    } else {
        QMessageBox::information(this, "车位申请", "申请成功", QMessageBox::Ok);
        this->close();
    }
}
