#include "add_owner_information.h"
#include "ui_add_owner_information.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateEdit>
#include <QCalendarWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTableView>
#include <QSqlError>
#include <QDebug>
#include <QSqlTableModel> // 提供QSqlTableModel类
#include <QString>        // 提供QString类和arg()函数
#include <QLineEdit>


Add_owner_information::Add_owner_information(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Add_owner_information)
{
    ui->setupUi(this);
    this->setFixedSize(2100, 1268);
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
    db.setConnectOptions("QSQLITE_OPEN_URI;QSQLITE_ENABLE_SHARED_CACHE");
    db.setDatabaseName("file:IntelligentCommunity.db?charset=utf8"); // 显式指定UTF-8
}

Add_owner_information::~Add_owner_information()
{
    delete ui;
}

void Add_owner_information::on_pushButton_2_clicked()
{
    //add owner information 窗口关闭
    this->close();

}


void Add_owner_information::on_pushButton_clicked()
{
    //读取所有LineEdit中的内容
    QString userId = ui->lineEdit->text();
    QString ownername = ui->lineEdit_2->text();
    QString building = ui->lineEdit_7->text();
    QString unit = ui->lineEdit_8->text();
    QString room = ui->lineEdit_9->text();

    QString member1 = ui->lineEdit_3->text();
    QString relationship1 = ui->lineEdit_4->text();
    QString phone1 = ui->lineEdit_5->text();

    QString member2 = ui->lineEdit_6->text();
    QString relationship2 = ui->lineEdit_10->text();
    QString phone2 = ui->lineEdit_11->text();

    QString member3 = ui->lineEdit_12->text();
    QString relationship3 = ui->lineEdit_13->text();
    QString phone3 = ui->lineEdit_14->text();


    //分开检查是否为空
    if (!userId.isEmpty()&&!ownername.isEmpty() &&!building.isEmpty() && !unit.isEmpty() && !room.isEmpty() ) {
        // 新增：验证输入业主姓名（username）与USER_ACCOUNT表是否一致
        QSqlQuery nameCheckQuery;
        nameCheckQuery.prepare("SELECT username FROM USER_ACCOUNT WHERE user_id = :userId");
        nameCheckQuery.bindValue(":userId",userId);
        if (!nameCheckQuery.exec()) {
            // QMessageBox::critical(this, "查询失败", "验证业主姓名时数据库错误：" + nameCheckQuery.lastError().text());
            qDebug() << "查询失败:" << nameCheckQuery.lastError().text();
            return; // 或处理错误
        }

        qDebug() << "输入姓名:" << ownername << "数据库姓名:" << nameCheckQuery.value(0).toString();

        if (!nameCheckQuery.next() || nameCheckQuery.value(0).toString().trimmed() != ownername.trimmed()) {
            QMessageBox::warning(this, "姓名错误", "业主姓名输入错误，请核对后重新输入。");
            ui->lineEdit_2->clear(); // 清空姓名输入框
            ui->lineEdit_2->setFocus(); // 聚焦到姓名输入框
            return;
        }
        //在数据库OWNER表user_id字段中添加userId的值
        QSqlQuery query;
        query.prepare("INSERT INTO OWNER (owner_id, user_id, ownername) "
                      "VALUES (NULL, ?, ?)");
        query.addBindValue(userId);
        query.addBindValue(ownername); // 绑定业主姓名
        if (!query.exec()) {
            QMessageBox::critical(this, "添加失败", "添加业主信息失败：" + query.lastError().text());
            return;
        } else {
            QMessageBox::information(this, "添加成功", "业主信息添加成功");
        }

        //查询数据库OWNER表user_id字段所在行对应的owner_id的值
        QSqlQuery query2;
        query2.prepare("SELECT owner_id FROM OWNER WHERE user_id = :userId");
        query2.bindValue(":userId", userId);
        if (!query2.exec()) {
            QMessageBox::critical(this, "查询失败", "查询业主ID失败：" + query2.lastError().text());
            return;
        }
        //把获取的结果添加到数据库PROPERTY表owner_id字段中
        if (query2.next()) {
            int ownerId = query2.value(0).toInt();
            QSqlQuery query3;
            query3.prepare("INSERT INTO PROPERTY_NEW (owner_id,building,unit,room,ownername) "
                           "VALUES (:ownerId,:building,:unit,:room,:ownername)");

            query3.bindValue(":ownerId", ownerId);    // 绑定业主ID
            query3.bindValue(":building", building);  // 绑定楼栋
            query3.bindValue(":unit", unit);          // 绑定单元
            query3.bindValue(":room", room);          // 绑定房间号
            query3.bindValue(":ownername", ownername);  // 绑定用户名
            if (!query3.exec()) {
                QMessageBox::critical(this, "添加失败", "添加业主信息到PROPERTY表失败：" + query3.lastError().text());
                return;
            } else {
                QMessageBox::information(this, "添加成功", "业主信息添加到PROPERTY表成功");
            }
        } else {
            QMessageBox::critical(this, "查询失败", "未找到对应的业主ID");
        }

        // 5. 获取新插入的property_id
        int propertyId = -1;
        query.prepare("SELECT last_insert_rowid()");
        if (!query.exec() || !query.next()) {
            QMessageBox::critical(this, "查询失败", "获取房产ID失败：" + query.lastError().text());
            return;
        }
        propertyId = query.value(0).toInt();

        /******************************************************/
        // 6. 插入家庭成员信息（使用事务确保数据一致性）
        QSqlDatabase::database().transaction();
        bool familyInsertSuccess = true;
        int insertedCount = 0;

        // 定义插入单个成员的函数，仅在name和relation都非空时执行插入
        auto insertMemberIfValid = [&](const QString& name, const QString& relation, const QString& phone) {
            // 核心检查：姓名或关系为空则直接返回，不插入
            if (name.isEmpty() || relation.isEmpty()) {
                return true; // 空数据不插入，视为“成功跳过”
            }

            QSqlQuery query;
            query.prepare("INSERT INTO FAMILY_MEMBER (member_id,property_id, name, relation, phone) "
                          "VALUES (NULL,:propertyId, :name, :relation, :phone)");
            query.bindValue(":propertyId", propertyId);
            query.bindValue(":name", name);
            query.bindValue(":relation", relation);
            query.bindValue(":phone", phone);

            if (!query.exec()) {
                qDebug() << "插入成员失败：" << query.lastError().text();
                return false; // 执行失败返回false
            }
            insertedCount++; // 插入成功则计数+1
            return true;
        };

        // 逐个检查并插入，空数据自动跳过
        familyInsertSuccess &= insertMemberIfValid(member1, relationship1, phone1);
        familyInsertSuccess &= insertMemberIfValid(member2, relationship2, phone2);
        familyInsertSuccess &= insertMemberIfValid(member3, relationship3, phone3);

        // 处理事务结果
        if (familyInsertSuccess) {
            QSqlDatabase::database().commit();
            QString msg = "业主信息添加成功！";
            if (insertedCount > 0) {
                msg += QString("\n成功插入 %1 名家庭成员信息").arg(insertedCount);
            } else {
                msg += "\n未插入任何家庭成员（所有成员信息不完整）";
            }
            QMessageBox::information(this, "操作成功", msg);
        } else {
            QSqlDatabase::database().rollback();
            QMessageBox::critical(this, "操作失败", "插入家庭成员时发生错误");
        }

        //清空对话框
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        ui->lineEdit_4->clear();
        ui->lineEdit_5->clear();
        ui->lineEdit_7->clear();
        ui->lineEdit_8->clear();
        ui->lineEdit_9->clear();
        ui->lineEdit_10->clear();
        ui->lineEdit_11->clear();
        ui->lineEdit_12->clear();
        ui->lineEdit_13->clear();
        ui->lineEdit_14->clear();

    }}




