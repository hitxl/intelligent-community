#include "add_carfield_information.h"
#include "ui_add_carfield_information.h"

Add_carfield_information::Add_carfield_information(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Add_carfield_information)
{
    ui->setupUi(this);
    //连接数据库
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("IntelligentCommunity.db"); // 数据库文件名（或完整路径）

    if (!db.open()) {
        qDebug() << "Error: Failed to open database:" << db.lastError().text();
        return;
    }
}

Add_carfield_information::~Add_carfield_information()
{
    delete ui;
}
void Add_carfield_information::on_pushButton_2_clicked()
{
    this->close(); // 关闭窗口
}
void Add_carfield_information::on_addpushButton_clicked()
{
    QString input_location=ui->locationlineEdit->text().trimmed();
    //QString input_ownername=ui->ownerlineEdit->text().trimmed();

    if(input_location.isEmpty()){
        QMessageBox::warning(this, "输入错误", "请输入信息！");
        return;
    }
    db.transaction();
    QSqlQuery query;
    query.prepare("INSERT INTO PARKING_SPACE_NEW (space_id,location,ownername,space_status)VALUES(NULL,?,NULL,'vacant')");
    query.addBindValue(input_location);
    if(!query.exec()){
        db.rollback();
        qDebug() << "Query error:" << query.lastError().text();
        QMessageBox::critical(this, "插入失败", "数据库插入错误：" + query.lastError().text());
        return;
    }
    else{
        db.commit();
        QMessageBox::information(this, "成功", "车位添加成功");
        this->close();
    }
}




