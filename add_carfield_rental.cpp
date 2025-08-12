#include "add_carfield_rental.h"
#include "ui_add_carfield_rental.h"

add_carfield_rental::add_carfield_rental(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_carfield_rental)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("IntelligentCommunity.db"); // 数据库文件名（或完整路径）

    if (!db.open()) {
        qDebug() << "Error: Failed to open database:" << db.lastError().text();
        return;
    }
}

add_carfield_rental::~add_carfield_rental()
{
    delete ui;
}

void add_carfield_rental::on_submit_pushButton_clicked()
{
    QString input_id = ui->space_idlineEdit_3->text().trimmed();
    QString input_ownername = ui->ownername_lineEdit_2->text().trimmed();
    QDate input_startDate = ui->start_dateEdit->date();
    QString start_date = input_startDate.toString("yyyy-MM-dd");
    QDate input_endDate = ui->end_dateEdit->date();
    QString end_date = input_endDate.toString("yyyy-MM-dd");
    QString input_fee = ui->fee_lineEdit->text().trimmed();
    QString input_payment = ui->payment_status_comboBox->currentText();
    QString input_location = ui->location_lineEdit->text().trimmed();

    int days = input_startDate.daysTo(input_endDate);
    if(days<=0){
        QMessageBox::warning(this, "输入错误", "结束日期必须晚于开始日期！");
        return;
    }

    double fee=0.0;
    if(days<=30){
        fee=500.0;
    }else if (days <= 90) {
        fee = 800.0;
    } else if (days <= 180) {
        fee = 1999.99;
    } else if (days <= 365) {
        fee = 3699.99;
    } else {
        int years = (days + 364) / 365; // 向上取整
        fee = years * 3000.0;
    }
    input_fee = QString::number(fee, 'f', 2);

    if(input_id.isEmpty() || input_ownername.isEmpty() || input_fee.isEmpty() || input_payment.isEmpty()){
        QMessageBox::warning(this, "输入错误", "请输入信息！");
        return;
    }

    db.transaction(); // 开始事务
    QSqlQuery query;

    // 1. 插入租赁记录
    query.prepare("INSERT INTO PARKING_RENTAL_NEW (rental_id, space_id, ownername, start_date, end_date, fee, payment_status) VALUES (NULL, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(input_id);
    query.addBindValue(input_ownername);
    query.addBindValue(start_date);
    query.addBindValue(end_date);
    query.addBindValue(input_fee);
    query.addBindValue(input_payment);

    if(!query.exec()){
        db.rollback();
        qDebug() << "Query error:" << query.lastError().text();
        QMessageBox::critical(this, "插入失败", "租赁信息插入错误：" + query.lastError().text());
        return;
    }

    // 2. 检查车位是否存在
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT space_id FROM PARKING_SPACE_NEW WHERE space_id = ?");
    checkQuery.addBindValue(input_id);

    if(!checkQuery.exec()){
        db.rollback();
        qDebug() << "Check query error:" << checkQuery.lastError().text();
        QMessageBox::critical(this, "查询失败", "车位检查错误：" + checkQuery.lastError().text());
        return;
    }

    // 3. 如果车位不存在，则插入新车位记录
    if(!checkQuery.next()){
        query.clear();
        // 插入车位记录，ownername设为空字符串，status为rented
        query.prepare("INSERT INTO PARKING_SPACE_NEW (space_id, location, ownername, space_status) VALUES (?, ?, '', 'rented')");
        query.addBindValue(input_id);       // 使用输入的space_id
        query.addBindValue(input_location); // 使用输入的location

        if(!query.exec()){
            db.rollback();
            qDebug() << "Insert space error:" << query.lastError().text();
            QMessageBox::critical(this, "插入失败", "车位信息插入错误：" + query.lastError().text());
            return;
        }
    }

    // 4. 所有操作成功，提交事务
    db.commit();
    QMessageBox::information(this, "成功", "车位租赁信息添加成功");
    this->close();
}

