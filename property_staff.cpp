#include "property_staff.h"
#include "qsqldatabase.h"
#include "qsqlquery.h"
#include "ui_property_staff.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QEasingCurve>
#include<QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include<QStandardItemModel>
#include "add_owner_information.h"
#include "add_carfield_information.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateEdit>
#include <QDateTime>
#include <QCalendarWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTableView>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QAbstractAnimation>
#include "update_carfield_information.h"
#include "update_carfield_rental_information.h"

#include "add_carfield_rental.h"
#include "vehicle_recognition.h"
#include <QDebug>
#include <QLineEdit>
#include"login.h"
#include <QStandardItemModel>
Property_Staff::Property_Staff(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Property_Staff)
    , loginWindow(nullptr)

{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(8);
    resize(800, 800);
    //链接数据库
    // 在构造函数中修改数据库连接代码
    QSqlDatabase db;
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    if(!db.isOpen()){ // 仅在未打开时尝试打开
        db.setDatabaseName("IntelligentCommunity.db");
        if(!db.open()){
            QMessageBox::critical(this, "错误", "数据库连接失败: " + db.lastError().text());
        }
    }

}

Property_Staff::~Property_Staff()
{
    if (db.isOpen()) db.close();
    delete ui;
}


// 修改按钮点击事件处理函数
void Property_Staff::on_Checkin_Button_clicked()
{

    ui->stackedWidget->setCurrentIndex(0);
    ui->Checkin_Button->setChecked(true);
    ui->Leave_Button_2->setChecked(false);
    ui->Attendence_Button_3->setChecked(false);
    ui->Repair_Button_4->setChecked(false);
    ui->Parking_Button->setChecked(false);
    ui->Feemanagement_Button->setChecked(false);
    ui->caridentify->setChecked(false);
}



void Property_Staff::on_Leave_Button_2_clicked()
{

    ui->stackedWidget->setCurrentIndex(5);
    ui->Checkin_Button->setChecked(false);
    ui->Leave_Button_2->setChecked(true);
    ui->Attendence_Button_3->setChecked(false);
    ui->Repair_Button_4->setChecked(false);
    ui->Parking_Button->setChecked(false);
    ui->Feemanagement_Button->setChecked(false);
    ui->Ownermanagement_Button_2->setChecked(false);
    ui->caridentify->setChecked(false);
}



void Property_Staff::on_pushButton_6_clicked()
{
    //当tableView最后一行不为空时，新建一个空行
    QTableView *tableView = ui->tableView;
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(tableView->model());
    if (model && model->rowCount() > 0) {
        int lastRow = model->rowCount() - 1;
        if (model->item(lastRow, 0) != nullptr && !model->item(lastRow, 0)->text().isEmpty()) {
            model->insertRow(model->rowCount());
        }
    } else {
        model->insertRow(0); // 如果没有数据，插入第一行
    }
}
void Property_Staff::on_Attendence_Button_3_clicked()
{

    ui->stackedWidget->setCurrentIndex(6);

    //renter_line_tableView
    //tableview
    QTableView *tableView = ui->renter_line_tableView;
    QStandardItemModel*model=new QStandardItemModel(0,6,this);
    //设置表头
    QStringList headers;
    headers << "申请编号" << "申请人姓名"<<"车位编号" <<"车牌号" << "申请时间"<<"车位状态";
    model->setHorizontalHeaderLabels(headers);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);
    QSqlQuery query;
    query.prepare("SELECT * FROM PARKING_APPLICATION_NEW ");
    if(query.exec()){
        if(query.size()==0){
            qDebug() << "暂无排队信息";
        }
        else{
            while(query.next()) {
                QString space_id=query.value("space_id").toString();
                QString applicantname = query.value("applicantname").toString();
                QString car_number = query.value("car_number").toString();
                QString application_id = query.value("application_id").toString();
                QString apply_time = query.value("apply_time").toString();
                QString status = query.value("status").toString();
                int row=model->rowCount();
                model->setItem(row, 0, new QStandardItem(application_id));
                model->setItem(row, 1, new QStandardItem(applicantname));
                model->setItem(row, 3, new QStandardItem(car_number));
                model->setItem(row, 2, new QStandardItem(space_id));
                model->setItem(row, 4, new QStandardItem(apply_time));
                model->setItem(row, 5, new QStandardItem(status));
                //设置表格属性
                ui->renter_line_tableView->verticalHeader()->hide();
                ui->renter_line_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
                ui->renter_line_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
                CenteredDelegate1 *delegate2 = new CenteredDelegate1(ui->renter_line_tableView);
                ui->renter_line_tableView->setItemDelegate(delegate2);

                // 设置字体
                QFont font2 = ui->renter_line_tableView->font();
                font2.setPointSize(12);
                ui->renter_line_tableView->setFont(font2);
            }
        }
    }


    QTableView *tableView1 = ui->spaceprice_tableView;
    QStandardItemModel*model1=new QStandardItemModel(0,3,this);
    //设置表头
    QStringList headers1;
    headers1 << "车位ID" << "车位位置" <<"车位价格";
    model1->setHorizontalHeaderLabels(headers1);
    tableView1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView1->setModel(model1);

    QTableView *tableView2 = ui->rent_contract_tableView;
    QStandardItemModel*model2=new QStandardItemModel(0,7,this);
    //设置表头
    QStringList headers2;
    headers2 << "合同id" << "车位id"<<"车牌号"<<"业主id"<<"业主姓名"<<"开始日期"<<"有效日期";
    model2->setHorizontalHeaderLabels(headers2);
    tableView2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView2->setModel(model2);

    QTableView *tableView3 = ui->contract_expiration;
    QStandardItemModel*model3=new QStandardItemModel(0,7,this);
    //设置表头
    QStringList headers3;
    headers3 << "合同id" << "车位id"<<"车牌号"<<"业主id"<<"业主姓名"<<"开始日期"<<"有效日期";
    model3->setHorizontalHeaderLabels(headers3);
    tableView3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView3->setModel(model3);
    tableView1->verticalHeader()->setDefaultSectionSize(80);
    tableView2->verticalHeader()->setDefaultSectionSize(80);
    tableView3->verticalHeader()->setDefaultSectionSize(80);
    CenteredDelegate1 *delegate1 = new CenteredDelegate1(tableView1);
    tableView1->setItemDelegate(delegate1);
    QFont font1 = tableView1->font();
    font1.setPointSize(12);
    tableView1->setFont(font1);
    CenteredDelegate1 *delegate2 = new CenteredDelegate1(tableView2);
    tableView2->setItemDelegate(delegate2);
    QFont font2 = tableView2->font();
    font2.setPointSize(12);
    ui->renter_line_tableView->setFont(font2);
    CenteredDelegate1 *delegat3 = new CenteredDelegate1(tableView3);
    tableView3->setItemDelegate(delegat3);
    QFont font3 = tableView3->font();
    font3.setPointSize(12);
    tableView3->setFont(font3);

}


void Property_Staff::on_Repair_Button_4_clicked()
{

    ui->stackedWidget->setCurrentIndex(1);
    ui->Checkin_Button->setChecked(false);
    ui->Leave_Button_2->setChecked(false);
    ui->Attendence_Button_3->setChecked(false);
    ui->Repair_Button_4->setChecked(true);
    ui->Parking_Button->setChecked(false);
    ui->Feemanagement_Button->setChecked(false);
    ui->Ownermanagement_Button_2->setChecked(false);
    ui->caridentify->setChecked(false);
    // 删除旧模型
    QStandardItemModel* oldModel = qobject_cast<QStandardItemModel*>(ui->repair_tableView->model());
    if (oldModel) delete oldModel;

    // 创建新模型
    QStandardItemModel* model = new QStandardItemModel(0,9,this);
    QTableView *tableView = ui->repair_tableView;
    //设置表头
    QStringList headers;
    headers << "维修编号" << "业主姓名"<< "楼号"<<"单元号"<<"房间"<<"维修问题描述"<<"申请日期"<< "维修分配状态"<<"维修进程推进时间";


    model->setHorizontalHeaderLabels(headers);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);
    tableView->verticalHeader()->setDefaultSectionSize(80);
    CenteredDelegate1 *delegat1 = new CenteredDelegate1(tableView);
    tableView->setItemDelegate(delegat1);
    QFont font3 = tableView->font();
    font3.setPointSize(12);
    tableView->setFont(font3);
    QSqlQuery repairquery;
    repairquery.prepare("SELECT * FROM REPAIR ");
    if(repairquery.exec()){
        if(repairquery.size()==0){
            qDebug() << "暂无维修信息";
        }
        else{
            while(repairquery.next()) {
                //提取REPAIR表数据
                QString repair_id=repairquery.value("repair_id").toString();
                QString owner_id = repairquery.value("owner_id").toString();
                QString status = repairquery.value("status").toString();
                QString property_id = repairquery.value("property_id").toString();
                QString owner_description=repairquery.value("description").toString();
                QString report_time = repairquery.value("report_time").toString();
                //提取相应业主信息
                QSqlQuery ownerquery;
                ownerquery.prepare("SELECT user_id FROM OWNER WHERE owner_id=?");
                ownerquery.addBindValue(owner_id);
                if (!ownerquery.exec() || !ownerquery.next()) {
                    qDebug() << "查询业主信息失败:" << ownerquery.lastError().text();
                    continue; // 跳过当前行处理
                }
                QString user_id = ownerquery.value("user_id").toString();
                QSqlQuery query;
                query.prepare("SELECT username FROM USER_ACCOUNT WHERE user_id=?");
                query.addBindValue(user_id);
                if (!query.exec()) {
                    qDebug() << "用户查询失败:" << query.lastError().text();
                    continue;
                }
                if (!query.next()) {
                    qDebug() << "未找到业主" << owner_id << "的账号信息";
                    continue;
                }
                QString ownername =query.value("username").toString();
                //提取相应房产信息
                query.clear();
                query.prepare("SELECT building,unit,room FROM PROPERTY_NEW WHERE owner_id = ?");
                query.addBindValue(owner_id);
                if (!query.exec()) {
                    qDebug() << "房产查询失败:" << query.lastError().text();
                    continue;
                }
                if (!query.next()) {
                    qDebug() << "未找到业主" << owner_id << "的房产信息";
                    continue;
                }
                QString building=query.value("building").toString();
                QString unit=query.value("unit").toString();
                QString room=query.value("room").toString();
                //插入数据
                int row=model->rowCount();
                model->setItem(row, 0, new QStandardItem(repair_id));
                model->setItem(row, 1, new QStandardItem(ownername));
                model->setItem(row, 2, new QStandardItem(building));
                model->setItem(row, 3, new QStandardItem(unit));
                model->setItem(row, 4, new QStandardItem(room));
                model->setItem(row, 5, new QStandardItem(owner_description));
                model->setItem(row, 6, new QStandardItem(report_time));
                model->setItem(row, 7, new QStandardItem(status));
                model->setItem(row, 8, new QStandardItem(""));

            }
            ui->repair_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->repair_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->repair_tableView->verticalHeader()->setDefaultSectionSize(80);
            CenteredDelegate1 *delegat1 = new CenteredDelegate1(ui->repair_tableView);
           ui->repair_tableView->setItemDelegate(delegat1);
            QFont font3 = ui->repair_tableView->font();
            font3.setPointSize(12);
            ui->repair_tableView->setFont(font3);
        }
    }
    //加载维修人员到下拉框
    QSqlQuery staffQuery;
    staffQuery.prepare("SELECT s.staff_id, u.username FROM USER_ACCOUNT u JOIN STAFF s ON u.user_id = s.user_id WHERE u.user_type = 'staff' AND s.position = '维修工' AND s.department = '维修部'");
    if (staffQuery.exec()) {
        ui->chose_staff_comboBox->clear();
        ui->chose_staff_comboBox->addItem("请选择员工", "");
        while (staffQuery.next()) {
            QString staffId = staffQuery.value("staff_id").toString(); // 从STAFF表获取staff_id
            QString username = staffQuery.value("username").toString();
            ui->chose_staff_comboBox->addItem(username, staffId); // 存储staff_id作为附加数据
        }
    } else {
        qDebug() << "查询维修人员失败:" << staffQuery.lastError().text();
    }
    //维修反馈默认表
    loadFeedbackData("", ""); // 调用通用加载函数显示所有数据
}

void Property_Staff::on_Parking_Button_clicked()
{

    ui->stackedWidget->setCurrentIndex(2);
    ui->Checkin_Button->setChecked(false);
    ui->Leave_Button_2->setChecked(false);
    ui->Attendence_Button_3->setChecked(false);
    ui->Repair_Button_4->setChecked(false);
    ui->Parking_Button->setChecked(true);
    ui->Feemanagement_Button->setChecked(false);
    ui->Ownermanagement_Button_2->setChecked(false);
    ui->caridentify->setChecked(false);
    //tableview
    QTableView *tableView = ui->carfield_manage_tableView_2;
    QStandardItemModel*model=new QStandardItemModel(0,4,this);
    //设置表头
    QStringList headers;
    headers << "车位ID" << "车位位置" <<"车位状态" << "业主姓名";//<< "业主ID"<<"车位有效期";
    model->setHorizontalHeaderLabels(headers);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);
    QSqlQuery query;
    query.prepare("SELECT * FROM PARKING_SPACE_NEW ");
    if(query.exec()){
        if(query.size()==0){
            qDebug() << "暂无车位信息";
        }
        else{
            while(query.next()) {
                QString space_id=query.value("space_id").toString();
                QString ownername = query.value("ownername").toString();
                QString space_status = query.value("space_status").toString();
                QString location = query.value("location").toString();
                int row=model->rowCount();
                model->setItem(row, 0, new QStandardItem(space_id));
                model->setItem(row, 1, new QStandardItem(location));
                model->setItem(row, 3, new QStandardItem(ownername));
                model->setItem(row, 2, new QStandardItem(space_status));
                ui->carfield_manage_tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
                ui->carfield_manage_tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
                tableView->verticalHeader()->setDefaultSectionSize(80);
                CenteredDelegate1 *delegat1 = new CenteredDelegate1(tableView);
                tableView->setItemDelegate(delegat1);
                QFont font3 = tableView->font();
                font3.setPointSize(12);
                tableView->setFont(font3);


            }
        }
    }}

void Property_Staff::on_Feemanagement_Button_clicked()
{

    ui->stackedWidget->setCurrentIndex(3);
    ui->Checkin_Button->setChecked(false);
    ui->Leave_Button_2->setChecked(false);
    ui->Attendence_Button_3->setChecked(false);
    ui->Repair_Button_4->setChecked(false);
    ui->Parking_Button->setChecked(false);
    ui->Feemanagement_Button->setChecked(true);
    ui->Ownermanagement_Button_2->setChecked(false);
    ui->caridentify->setChecked(false);
    QTableView *tableView = ui->tableView;
    QStandardItemModel*model=new QStandardItemModel(0,7,this);
    //设置表头
    QStringList headers;
    headers << "缴费id"<<"业主id" <<"需缴费用"<<"缴费方式"<<"缴费日期"<<"缴费状态"<<"缴费类型";
    model->setHorizontalHeaderLabels(headers);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);
    //查询数据库PAYMENT_NEW表，payment_id,owner_id,total,pay_method,payment_date,payment_status字段中的数据并显示在tableView上
    QSqlQuery query;
   bool success = query.exec(QStringLiteral("SELECT payment_id, owner_id, total, method, payment_date, payment_status, payment_type  FROM PAYMENT_NEW"));
    if (!success) {
        QMessageBox::critical(this, u8"查询失败",
                              u8"缴费信息查询出错：\n" + query.lastError().text());
        return;
    }
    // 填充表格数据
    int row = 0;
    while (query.next()) {
        // 插入一行
        model->insertRow(row);

        // 设置每列数据
        model->setData(model->index(row, 0), query.value(QStringLiteral("payment_id")).toInt());
        model->setData(model->index(row, 1), query.value(QStringLiteral("owner_id")).toInt());
        model->setData(model->index(row, 2), query.value(QStringLiteral("total")).toDouble());
        model->setData(model->index(row, 3), query.value(QStringLiteral("method")).toString());
        model->setData(model->index(row, 4), query.value(QStringLiteral("payment_date")).toDateTime());
        model->setData(model->index(row, 5), query.value(QStringLiteral("payment_status")).toString());
        model->setData(model->index(row, 6), query.value(QStringLiteral("payment_type")).toString());

        row++;
    }
    // 设置表格视图属性
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    // 设置模型到表格
    tableView->setModel(model);
    // 调整表格行数和列数
    model->setRowCount(row);
    // 注意：表头仅8列，此处设置为9列会导致第9列无表头，建议与表头列数一致
    model->setColumnCount(7); // 修正为6列，与表头匹配
    tableView->verticalHeader()->setDefaultSectionSize(80);
    CenteredDelegate1 *delegat1 = new CenteredDelegate1(tableView);
    tableView->setItemDelegate(delegat1);
    QFont font3 = tableView->font();
    font3.setPointSize(12);
    tableView->setFont(font3);
    //设置表格最后有一行空行
}


void Property_Staff::on_Ownermanagement_Button_2_clicked()
{

    ui->stackedWidget->setCurrentIndex(4);
    ui->Checkin_Button->setChecked(false);
    ui->Leave_Button_2->setChecked(false);
    ui->Attendence_Button_3->setChecked(false);
    ui->Repair_Button_4->setChecked(false);
    ui->Parking_Button->setChecked(false);
    ui->Feemanagement_Button->setChecked(false);
    ui->Ownermanagement_Button_2->setChecked(true);
    ui->caridentify->setChecked(false);

    //显示用户信息
    QTableView *tableView = ui->owner_information;
    // 初始化模型（行：0，列：5，父对象：this）
    QStandardItemModel* model = new QStandardItemModel(0, 5, this);
    // 设置表头
    QStringList headers;
    headers << "业主id" << "业主姓名" << "楼号" << "单元号" << "房间号";
    model->setHorizontalHeaderLabels(headers);

    // 从数据库查询数据
    QSqlQuery query;
    // Qt6中QSqlQuery::exec()返回值和行为不变，但错误处理建议更明确
    bool success = query.exec(QStringLiteral("SELECT owner_id, ownername, building, unit, room FROM PROPERTY_NEW"));

    if (!success) {
        // Qt6中QMessageBox的用法不变，但建议使用u8字符串字面量（可选）
        QMessageBox::critical(this, u8"查询失败",
                              u8"业主信息查询出错：\n" + query.lastError().text());
        // 不建议直接关闭窗口，保留窗口便于排查问题
        return;
    }

    // 填充表格数据
    int row = 0;
    while (query.next()) {
        // 插入一行（Qt6中insertRow()用法不变）
        model->insertRow(row);

        // 设置每列数据（Qt6中QModelIndex和setData()用法不变）
        // 第一列：owner_id（int类型）
        model->setData(model->index(row, 0), query.value(QStringLiteral("owner_id")).toInt());
        // 第二列：username（字符串类型）
        model->setData(model->index(row, 1), query.value(QStringLiteral("ownername")).toString());
        // 第三列：building（字符串类型）
        model->setData(model->index(row, 2), query.value(QStringLiteral("building")).toString());
        // 第四列：unit（字符串类型）
        model->setData(model->index(row, 3), query.value(QStringLiteral("unit")).toString());
        // 第五列：room（字符串类型）
        model->setData(model->index(row, 4), query.value(QStringLiteral("room")).toString());

        row++;
    }

    // 设置表格视图属性
    // Qt6中QHeaderView::Stretch枚举值不变，但建议用QHeaderView::ResizeMode枚举类
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    // 设置模型到表格（Qt6中setModel()用法不变）
    tableView->setModel(model);

    // 调整表格行数和列数（根据实际数据量）
    model->setRowCount(row);
    // 注意：表头仅5列，此处设置为6列会导致第6列无表头，建议与表头列数一致
    model->setColumnCount(5); // 修正为5列，与表头匹配
    //设置表格不可编辑
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->owner_information->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->verticalHeader()->setDefaultSectionSize(80);
    CenteredDelegate1 *delegat1 = new CenteredDelegate1(tableView);
    tableView->setItemDelegate(delegat1);
    QFont font3 = tableView->font();
    font3.setPointSize(12);
    tableView->setFont(font3);
}



void Property_Staff::on_reflash_clicked()
{
    QTableView *tableView = ui->renter_line_tableView;
    QStandardItemModel*model=new QStandardItemModel(0,6,this);
    //设置表头
    QStringList headers;
    headers << "申请编号" << "申请人姓名"<<"车位编号" <<"车牌号" << "申请时间"<<"车位状态";//<< "业主ID"<<"车位有效期";
    model->setHorizontalHeaderLabels(headers);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);
    QSqlQuery query;
    query.prepare("SELECT * FROM PARKING_APPLICATION_NEW ");
    if(query.exec()){
        if(query.size()==0){
            qDebug() << "暂无排队信息";
        }
        else{
            while(query.next()) {
                QString space_id=query.value("space_id").toString();
                QString applicantname = query.value("applicantname").toString();
                QString car_number = query.value("car_number").toString();
                QString application_id = query.value("application_id").toString();
                QString apply_time = query.value("apply_time").toString();
                QString status = query.value("status").toString();
                int row=model->rowCount();
                model->setItem(row, 0, new QStandardItem(application_id));
                model->setItem(row, 1, new QStandardItem(applicantname));
                model->setItem(row, 3, new QStandardItem(car_number));
                model->setItem(row, 2, new QStandardItem(space_id));
                model->setItem(row, 4, new QStandardItem(apply_time));
                model->setItem(row, 5, new QStandardItem(status));
                //设置表格属性
                ui->renter_line_tableView->verticalHeader()->hide();
                ui->renter_line_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
                ui->renter_line_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
                tableView->verticalHeader()->setDefaultSectionSize(80);
                CenteredDelegate1 *delegat1 = new CenteredDelegate1(tableView);
                tableView->setItemDelegate(delegat1);
                QFont font3 = tableView->font();
                font3.setPointSize(12);
                tableView->setFont(font3);
            }
        }
    }
}


void Property_Staff::on_reflash2_clicked()
{
    // 清空 tableView 的内容并释放旧模型内存
    QAbstractItemModel *currentModel = ui->spaceprice_tableView->model();
    if (currentModel) {
        delete currentModel; // 删除旧模型，防止内存泄漏
        ui->spaceprice_tableView->setModel(nullptr);
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM PARKING_SPACE_NEW ");
    // bool found = false;
    QStandardItemModel *model = new QStandardItemModel(0, 5, this); // 修正列数为5
    QStringList headers = {"车位ID", "车位位置", "车位价格","租借合同编号", "车位状态"};
    model->setHorizontalHeaderLabels(headers);

    if(query.exec()) {
        int row = 0;
        // 循环读取所有记录
        while(query.next()){
            QString space_id = query.value("space_id").toString();
            QString location = query.value("location").toString();
            //QString ownername = query.value("ownername").toString();
            QString space_status = query.value("space_status").toString();

            // 查询价格信息 - 添加exec()执行查询
            QSqlQuery priceQuery;
            priceQuery.prepare("SELECT fee,rental_id FROM PARKING_RENTAL_NEW WHERE space_id = ?");
            priceQuery.addBindValue(space_id);
            QString price ="";// priceQuery.value("fee").toString();
            QString rental_id="";//
            if(priceQuery.exec() && priceQuery.next()){
                price = priceQuery.value("fee").toString();
                rental_id=priceQuery.value("rental_id").toString();
            }

            // 添加新行
            model->insertRow(row);
            model->setItem(row, 0, new QStandardItem(space_id));
            model->setItem(row, 1, new QStandardItem(location));
            model->setItem(row, 2, new QStandardItem(price));
            model->setItem(row, 3, new QStandardItem(rental_id));
            model->setItem(row, 4, new QStandardItem(space_status));

            // 设置字体样式
            model->item(row, 0)->setFont(QFont("Times", 10, QFont::Black));
            model->item(row, 2)->setFont(QFont("Times", 10, QFont::Black));

            row++;
        }

        // 设置表格属性
        ui->spaceprice_tableView->setModel(model);
        ui->spaceprice_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->spaceprice_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->spaceprice_tableView->verticalHeader()->hide();
        ui->spaceprice_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->spaceprice_tableView->verticalHeader()->setDefaultSectionSize(80);
        CenteredDelegate1 *delegat1 = new CenteredDelegate1(ui->spaceprice_tableView);
        ui->spaceprice_tableView->setItemDelegate(delegat1);
        QFont font3 = ui->spaceprice_tableView->font();
        font3.setPointSize(12);
        ui->spaceprice_tableView->setFont(font3);
        // 处理无数据情况
        if(model->rowCount() == 0){
            QMessageBox::information(this, "查询结果", "未找到任何车位信息");
        }
    } else {
        // 处理查询错误
        qDebug() << "Query error:" << query.lastError().text();
        QMessageBox::critical(this, "查询失败", "数据库查询错误：" + query.lastError().text());
    }

}


void Property_Staff::on_reflash3_clicked()
{
    //rent_contract_tableView
    QTableView *tableView = ui->rent_contract_tableView;
    QStandardItemModel*model=new QStandardItemModel(0,7,this);
    //设置表头
    QStringList headers;
    headers << "合同id" << "车位id"<<"业主姓名"<<"订单金额"<<"订单支付情况"<<"开始日期"<<"有效日期";
    model->setHorizontalHeaderLabels(headers);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);
    QSqlQuery query;
    query.prepare("SELECT *FROM PARKING_RENTAL_NEW");
    if(query.exec()){
        if(query.size()==0){
            qDebug() << "暂无合同信息";
        }
        else{
            while(query.next()) {
                QString rental_id=query.value("rental_id").toString();
                QString space_id=query.value("space_id").toString();
                QString ownername = query.value("ownername").toString();
                QString start_date = query.value("start_date").toString();
                QString end_date = query.value("end_date").toString();
                QString fee=query.value("fee").toString();
                QString payment_status=query.value("payment_status").toString();
                int row=model->rowCount();
                model->setItem(row, 0, new QStandardItem(rental_id));
                model->setItem(row, 1, new QStandardItem(space_id));
                model->setItem(row, 2, new QStandardItem(ownername));
                model->setItem(row, 3, new QStandardItem(fee));
                model->setItem(row, 4, new QStandardItem(payment_status));
                model->setItem(row, 5, new QStandardItem(start_date));
                model->setItem(row, 6, new QStandardItem(end_date));
                ui->rent_contract_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
                ui->rent_contract_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
            }
        }
    }
    //设置表格属性
    ui->rent_contract_tableView->verticalHeader()->hide();
    ui->rent_contract_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->rent_contract_tableView->verticalHeader()->setDefaultSectionSize(80);
    CenteredDelegate1 *delegat1 = new CenteredDelegate1(tableView);
    tableView->setItemDelegate(delegat1);
    QFont font3 = tableView->font();
    font3.setPointSize(12);
    tableView->setFont(font3);
    model->item(0, 2)->setFont(QFont("Times", 10, QFont::Black));
    model->item(0, 4)->setFont(QFont("Times", 10, QFont::Black));
}


void Property_Staff::on_pushButton_18_clicked()
{

    QAbstractItemModel *currentModel = ui->contract_expiration->model();
    if (currentModel) {
        delete currentModel; // 删除旧模型，防止内存泄漏
        ui->contract_expiration->setModel(nullptr);
    }

    //QString input_info = ui->contractid_lineEdit_19->text().trimmed();
    /* if(input_info.isEmpty()){
        QMessageBox::warning(this, "输入错误", "请输入正确的合同信息！");
        return;
    }*/

    QSqlQuery query;

    query.prepare("SELECT * FROM PARKING_RENTAL_NEW");
    if(query.exec() && query.next()){
        // 提取数据-根据ID检索到信息
        QString rental_id = query.value("rental_id").toString();
        QString space_id = query.value("space_id").toString();
        QString ownername = query.value("ownername").toString();
        QString payment_status = query.value("payment_status").toString();
        QString start_date = query.value("start_date").toString();
        QString end_date = query.value("end_date").toString();


        // 设置表头和数据
        QStandardItemModel *model = new QStandardItemModel(1, 6, this);
        QStringList headers = {"合同ID","车位ID", "所有人姓名", "车位付款状态","开始租借时间","结束租借时间"};
        model->setHorizontalHeaderLabels(headers);
        model->setItem(0, 0, new QStandardItem(rental_id));
        model->setItem(0, 1, new QStandardItem(space_id));
        model->setItem(0, 2, new QStandardItem(ownername));
        model->setItem(0, 3, new QStandardItem(payment_status));
        model->setItem(0, 4, new QStandardItem(start_date));
        model->setItem(0, 5, new QStandardItem(end_date));

        // 设置表格属性
        ui->contract_expiration->setModel(model);
        ui->contract_expiration->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->contract_expiration->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->contract_expiration->verticalHeader()->hide();
        ui->contract_expiration->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->contract_expiration->verticalHeader()->setDefaultSectionSize(80);
        CenteredDelegate1 *delegat1 = new CenteredDelegate1(ui->contract_expiration);
        ui->contract_expiration->setItemDelegate(delegat1);
        QFont font3 = ui->contract_expiration->font();
        font3.setPointSize(12);
        ui->contract_expiration->setFont(font3);
        model->item(0, 0)->setFont(QFont("Times", 10, QFont::Black));
        model->item(0, 2)->setFont(QFont("Times", 10, QFont::Black));
        model->item(0, 3)->setFont(QFont("Times", 10, QFont::Black));
    }
    else if(!query.exec()){
        qDebug() << "Query error:" << query.lastError().text();
        QMessageBox::critical(this, "查询失败", "数据库查询错误：" + query.lastError().text());
        return;
    }
}


void Property_Staff::on_datechoose_pushButton_clicked()
{
    // 创建一个新的日期选择对话框
    QDialog *dateDialog = new QDialog(this);
    dateDialog->setWindowTitle("选择日期");

    // 创建日期编辑器
    QDateEdit *dateEdit = new QDateEdit(dateDialog);
    dateEdit->setCalendarPopup(true);
    dateEdit->setDate(QDate(2025, 1, 1));
    // 创建一个确认按钮
    QPushButton *confirmButton = new QPushButton("确认", dateDialog);

    // 布局设置
    QVBoxLayout *layout = new QVBoxLayout(dateDialog);
    layout->addWidget(dateEdit);
    layout->addWidget(confirmButton);

    connect(confirmButton, &QPushButton::clicked, [dateDialog, dateEdit]() {
        QMessageBox::information(dateDialog, "选择的日期", "您选择的日期是: " + dateEdit->date().toString());
        dateDialog->accept();
    });

    dateDialog->setLayout(layout);
    dateDialog->exec();
    //把选择的日期显示到lineEdit_2中
    QString selectedDate = dateEdit->date().toString("yyyy-MM-dd");
    ui->lineEdit_2->setText(selectedDate);
    //lineEdit_2不可编辑
    ui->lineEdit_2->setReadOnly(true);
}


void Property_Staff::on_pushButton_12_clicked()
{
    // 创建一个新的日期选择对话框
    QDialog *dateDialog = new QDialog(this);
    dateDialog->setWindowTitle("选择日期");

    // 创建日期编辑器
    QDateEdit *dateEdit = new QDateEdit(dateDialog);
    dateEdit->setCalendarPopup(true);
    dateEdit->setDate(QDate(2025, 1, 1));
    // 创建一个确认按钮
    QPushButton *confirmButton = new QPushButton("确认", dateDialog);

    // 布局设置
    QVBoxLayout *layout = new QVBoxLayout(dateDialog);
    layout->addWidget(dateEdit);
    layout->addWidget(confirmButton);

    connect(confirmButton, &QPushButton::clicked, [dateDialog, dateEdit]() {
        QMessageBox::information(dateDialog, "选择的日期", "您选择的日期是: " + dateEdit->date().toString());
        dateDialog->accept();
    });

    dateDialog->setLayout(layout);
    dateDialog->exec();
    //把选择的日期显示到lineEdit_3中
    QString selectedDate = dateEdit->date().toString("yyyy-MM-dd");
    ui->lineEdit_3->setText(selectedDate);
    //lineEdit_3不可编辑
    ui->lineEdit_3->setReadOnly(true);
}


void Property_Staff::on_pushButton_clicked()
{
    //add_owner_information界面show
    Add_owner_information *addOwnerInfo = new Add_owner_information();
    addOwnerInfo->show();


}


void Property_Staff::on_pushButton_4_clicked()
{
    Add_carfield_information *addCarinfo = new Add_carfield_information();
    addCarinfo->show();

}


void Property_Staff::on_pushButton_13_clicked()
{
    //如果lineEdit_2和lineEdit_3和textEdit都不为空，则显示申请成功
    QString startDate = ui->lineEdit_2->text();
    QString endDate = ui->lineEdit_3->text();
    QString reason = ui->textEdit->toPlainText();
    QString staffId = ui->staff_id->text();
    QString type = ui->comboBox->currentText();
    QString type1="1";
    if (startDate.isEmpty() || endDate.isEmpty() || reason.isEmpty() || staffId.isEmpty() || type.isEmpty()) {
        QMessageBox::warning(this, "错误", "请确保所有字段都已填写");
        return;
    }
    if(type=="病假"){
        type1="sick";
    }
    else if(type=="个人原因"){
        type1="personal";
    }
    else if(type=="休假"){
        type1="annual";
    }

    //在数据库LEAVE表中插入数据

    QSqlQuery query;
    query.prepare("INSERT INTO LEAVE (start_date, end_date, reason, staff_id, type) "
                  "VALUES (:startDate, :endDate, :reason, :staffId, :type1)");
    query.bindValue(":startDate", startDate);
    query.bindValue(":endDate", endDate);
    query.bindValue(":reason", reason);
    query.bindValue(":staffId", staffId.toInt());
    query.bindValue(":type1", type1);
    if (!query.exec()) {
        QMessageBox::critical(this, "插入失败", "插入请假信息失败：" + query.lastError().text());
        return;
    }
    QMessageBox::information(this, "插入成功", "请假信息插入成功");
    //清空输入框
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->textEdit->clear();
    ui->staff_id->clear();
    ui->comboBox->setCurrentIndex(0); // 重置下拉框到第一个选项
    //刷新tableView
    //QTableView *tableView = ui->leave_tableView;
}


void Property_Staff::on_pushButton_3_clicked()
{
    update_carfield_information *w=new update_carfield_information();
    w->show();
}

void Property_Staff::on_find_space_pushButton_5_clicked()
{
    // 清空 tableView 的内容并释放旧模型内存
    QAbstractItemModel *currentModel = ui->carfield_manage_tableView_2->model();
    if (currentModel) {
        delete currentModel; // 删除旧模型，防止内存泄漏
        ui->carfield_manage_tableView_2->setModel(nullptr);
    }

    QString input_info = ui->space_id_location_lineEdit_27->text().trimmed();
    if(input_info.isEmpty()){
        QMessageBox::warning(this, "输入错误", "请输入正确的车位信息！");
        return;
    }

    QSqlQuery query;
    bool found = false;

    // 1. 尝试按车位ID查询
    query.prepare("SELECT location, ownername, space_status FROM PARKING_SPACE_NEW WHERE space_id = ?");
    query.addBindValue(input_info);
    if(query.exec() && query.next()){
        // 提取数据-根据ID检索到信息
        QString location = query.value("location").toString();
        QString ownername = query.value("ownername").toString();
        QString space_status = query.value("space_status").toString();
        found = true;

        // 设置表头和数据
        QStandardItemModel *model = new QStandardItemModel(1, 4, this);
        QStringList headers = {"车位ID", "车位位置", "所有人姓名", "车位状态"};
        model->setHorizontalHeaderLabels(headers);
        model->setItem(0, 0, new QStandardItem(input_info));
        model->setItem(0, 1, new QStandardItem(location));
        model->setItem(0, 2, new QStandardItem(ownername));
        model->setItem(0, 3, new QStandardItem(space_status));

        // 设置表格属性
        ui->carfield_manage_tableView_2->setModel(model);
        ui->carfield_manage_tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->carfield_manage_tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->carfield_manage_tableView_2->verticalHeader()->hide();
        ui->carfield_manage_tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->carfield_manage_tableView_2->verticalHeader()->setDefaultSectionSize(80);
        CenteredDelegate1 *delegat1 = new CenteredDelegate1( ui->carfield_manage_tableView_2);
        ui->carfield_manage_tableView_2->setItemDelegate(delegat1);
        QFont font3 =  ui->carfield_manage_tableView_2->font();
        font3.setPointSize(12);
        ui->carfield_manage_tableView_2->setFont(font3);
        model->item(0, 0)->setFont(QFont("Times", 10, QFont::Black));
        model->item(0, 2)->setFont(QFont("Times", 10, QFont::Black));
    }
    // 2. 如果ID查询无结果，尝试按位置查询
    else if(!found){
        query.clear(); // 清除之前的查询和绑定
        query.prepare("SELECT space_id, ownername, space_status FROM PARKING_SPACE_NEW WHERE location = ?");
        query.addBindValue(input_info);
        if(query.exec() && query.next()){
            // 提取数据-根据位置检索到信息
            QString space_id = query.value("space_id").toString();
            QString ownername = query.value("ownername").toString();
            QString space_status = query.value("space_status").toString();
            found = true;

            // 设置表头和数据
            QStandardItemModel *model = new QStandardItemModel(1, 4, this);
            QStringList headers = {"车位ID", "车位位置", "所有人姓名", "车位状态"};
            model->setHorizontalHeaderLabels(headers);
            model->setItem(0, 0, new QStandardItem(space_id));
            model->setItem(0, 1, new QStandardItem(input_info));
            model->setItem(0, 2, new QStandardItem(ownername));
            model->setItem(0, 3, new QStandardItem(space_status));

            // 设置表格属性
            ui->carfield_manage_tableView_2->setModel(model);
            ui->carfield_manage_tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->carfield_manage_tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->carfield_manage_tableView_2->verticalHeader()->hide();
            ui->carfield_manage_tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->carfield_manage_tableView_2->verticalHeader()->setDefaultSectionSize(80);
            CenteredDelegate1 *delegat1 = new CenteredDelegate1( ui->carfield_manage_tableView_2);
            ui->carfield_manage_tableView_2->setItemDelegate(delegat1);
            QFont font3 =  ui->carfield_manage_tableView_2->font();
            font3.setPointSize(12);
            ui->carfield_manage_tableView_2->setFont(font3);
            model->item(0, 0)->setFont(QFont("Times", 10, QFont::Black));
            model->item(0, 2)->setFont(QFont("Times", 10, QFont::Black));
        }
        // 查询执行失败
        else if(!query.exec()){
            qDebug() << "Query error:" << query.lastError().text();
            QMessageBox::critical(this, "查询失败", "数据库查询错误：" + query.lastError().text());
            return;
        }
    }

    // 3. 如果两个查询都没有找到结果
    if(!found){
        QMessageBox::information(this, "查询结果", "未找到匹配的车位信息");
    }
}



void Property_Staff::on_spaceprice_findid_pushButton_clicked()
{
    // 清空 tableView 的内容并释放旧模型内存
    QAbstractItemModel *currentModel = ui->spaceprice_tableView->model();
    if (currentModel) {
        delete currentModel; // 删除旧模型，防止内存泄漏
        ui->spaceprice_tableView->setModel(nullptr);
    }

    QString input_info = ui->spaceprice_spaceid_lineEdit->text().trimmed();
    if(input_info.isEmpty()){
        QMessageBox::warning(this, "输入错误", "请输入正确的车位信息！");
        return;
    }

    QSqlQuery query;
    bool found = false;

    // 1. 尝试按车位ID查询
    query.prepare("SELECT location, ownername, space_status FROM PARKING_SPACE_NEW WHERE space_id = ?");
    query.addBindValue(input_info);
    if(query.exec()) {
        if(query.next()){
            // 提取数据-根据ID检索到信息
            QString location = query.value("location").toString();
            QString ownername = query.value("ownername").toString();
            QString space_status = query.value("space_status").toString();

            // 查询价格信息
            query.clear();
            query.prepare("SELECT fee FROM PARKING_RENTAL_NEW WHERE space_id = ?");
            query.addBindValue(input_info);
            if(!query.exec() || !query.next()){
                QMessageBox::warning(this, "数据错误", "未找到车位价格信息");
                return;
            }
            QString price = query.value("fee").toString();
            found = true;

            // 设置表头和数据
            QStandardItemModel *model = new QStandardItemModel(1, 5, this); // 修正列数为5
            QStringList headers = {"车位ID", "车位位置", "车位价格","所有人姓名", "车位状态"};
            model->setHorizontalHeaderLabels(headers);
            model->setItem(0, 0, new QStandardItem(input_info));
            model->setItem(0, 1, new QStandardItem(location));
            model->setItem(0, 2, new QStandardItem(price));
            model->setItem(0, 3, new QStandardItem(ownername));
            model->setItem(0, 4, new QStandardItem(space_status));

            // 设置表格属性
            ui->spaceprice_tableView->setModel(model);
            ui->spaceprice_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->spaceprice_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->spaceprice_tableView->verticalHeader()->hide();
            ui->spaceprice_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->spaceprice_tableView->verticalHeader()->setDefaultSectionSize(80);
            CenteredDelegate1 *delegat1 = new CenteredDelegate1( ui->spaceprice_tableView);
            ui->spaceprice_tableView->setItemDelegate(delegat1);
            QFont font3 = ui->spaceprice_tableView->font();
            font3.setPointSize(12);
            ui->spaceprice_tableView->setFont(font3);
            model->item(0, 0)->setFont(QFont("Times", 10, QFont::Black));
            model->item(0, 2)->setFont(QFont("Times", 10, QFont::Black));
        }
    } else {
        qDebug() << "ID Query error:" << query.lastError().text();
        QMessageBox::critical(this, "查询失败", "数据库查询错误：" + query.lastError().text());
        return;
    }

    // 2. 如果ID查询无结果，尝试按位置查询
    if(!found){
        query.clear(); // 清除之前的查询和绑定
        query.prepare("SELECT space_id, ownername, space_status FROM PARKING_SPACE_NEW WHERE location = ?");
        query.addBindValue(input_info);
        if(query.exec()) {
            if(query.next()){
                // 提取数据-根据位置检索到信息
                QString space_id = query.value("space_id").toString();
                QString ownername = query.value("ownername").toString();
                QString space_status = query.value("space_status").toString();

                // 查询价格信息
                query.clear();
                query.prepare("SELECT fee FROM PARKING_RENTAL_NEW WHERE space_id = ?");
                query.addBindValue(space_id);
                if(!query.exec() || !query.next()){
                    QMessageBox::warning(this, "数据错误", "未找到车位价格信息");
                    return;
                }
                QString price = query.value("fee").toString();
                found = true;

                // 设置表头和数据
                QStandardItemModel *model = new QStandardItemModel(1, 5, this); // 修正列数为5
                QStringList headers = {"车位ID", "车位位置","车位价格", "所有人姓名", "车位状态"};
                model->setHorizontalHeaderLabels(headers);
                model->setItem(0, 0, new QStandardItem(space_id));
                model->setItem(0, 1, new QStandardItem(input_info));
                model->setItem(0, 2, new QStandardItem(price));
                model->setItem(0, 3, new QStandardItem(ownername));
                model->setItem(0, 4, new QStandardItem(space_status));

                // 设置表格属性
                ui->spaceprice_tableView->setModel(model);
                ui->spaceprice_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
                ui->spaceprice_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
                ui->spaceprice_tableView->verticalHeader()->hide();
                ui->spaceprice_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
                ui->spaceprice_tableView->verticalHeader()->setDefaultSectionSize(80);
                CenteredDelegate1 *delegat1 = new CenteredDelegate1( ui->spaceprice_tableView);
                ui->spaceprice_tableView->setItemDelegate(delegat1);
                QFont font3 = ui->spaceprice_tableView->font();
                font3.setPointSize(12);
                ui->spaceprice_tableView->setFont(font3);
                model->item(0, 0)->setFont(QFont("Times", 10, QFont::Black));
                model->item(0, 2)->setFont(QFont("Times", 10, QFont::Black));
            }
        } else {
            qDebug() << "Location Query error:" << query.lastError().text();
            QMessageBox::critical(this, "查询失败", "数据库查询错误：" + query.lastError().text());
            return;
        }
    }

    // 3. 如果两个查询都没有找到结果
    if(!found){
        QMessageBox::information(this, "查询结果", "未找到匹配的车位信息");
    }
}

void Property_Staff::on_spaceprice_change_pushButton_7_clicked()
{
    update_carfield_rental_information *w=new update_carfield_rental_information();
    w->show();
}

void Property_Staff::on_contract_findpushButton_19_clicked()
{
    QAbstractItemModel *currentModel = ui->contract_expiration->model();
    if (currentModel) {
        delete currentModel; // 删除旧模型，防止内存泄漏
        ui->contract_expiration->setModel(nullptr);
    }

    QString input_info = ui->contractid_lineEdit_19->text().trimmed();
    if(input_info.isEmpty()){
        QMessageBox::warning(this, "输入错误", "请输入正确的合同信息！");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT space_id, ownername, start_date,end_date,fee,payment_status FROM PARKING_RENTAL_NEW WHERE rental_id = ?");
    query.addBindValue(input_info);
    if(query.exec() && query.next()){
        // 提取数据-根据ID检索到信息
        QString space_id = query.value("space_id").toString();
        QString ownername = query.value("ownername").toString();
        QString payment_status = query.value("payment_status").toString();
        QString start_date = query.value("start_date").toString();
        QString end_date = query.value("end_date").toString();
        //设置QMessageBox提醒是否逾期
        QDate currentDate = QDate::currentDate();
        QDate endDate=QDate::fromString(end_date,"yyyy-MM-dd");
        if (!endDate.isValid()) {
            QMessageBox::warning(this, "日期格式错误", "租赁结束日期格式无效");
        } else if (currentDate > endDate) {
            // 计算逾期天数
            qint64 daysOverdue = endDate.daysTo(currentDate);
            QMessageBox::warning(this, "租赁逾期提醒",
                                 QString("该租赁已逾期 %1 天！\n\n业主: %2\n车位ID: %3\n到期日期: %4")
                                     .arg(daysOverdue)
                                     .arg(ownername)
                                     .arg(space_id)
                                     .arg(end_date));
        } else {
            // 可选：显示未逾期信息
            qint64 daysRemaining = currentDate.daysTo(endDate);
            QMessageBox::information(this, "租赁状态",
                                     QString("租赁未逾期\n\n业主: %1\n车位ID: %2\n剩余天数: %3 天")
                                         .arg(ownername)
                                         .arg(space_id)
                                         .arg(daysRemaining));
        }
        // 设置表头和数据
        QStandardItemModel *model = new QStandardItemModel(1, 6, this);
        QStringList headers = {"合同ID","车位ID", "所有人姓名", "车位付款状态","开始租借时间","结束租借时间"};
        model->setHorizontalHeaderLabels(headers);
        model->setItem(0, 0, new QStandardItem(input_info));
        model->setItem(0, 1, new QStandardItem(space_id));
        model->setItem(0, 2, new QStandardItem(ownername));
        model->setItem(0, 3, new QStandardItem(payment_status));
        model->setItem(0, 4, new QStandardItem(start_date));
        model->setItem(0, 5, new QStandardItem(end_date));

        // 设置表格属性
        ui->contract_expiration->setModel(model);
        ui->contract_expiration->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->contract_expiration->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->contract_expiration->verticalHeader()->hide();
        ui->contract_expiration->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->contract_expiration->verticalHeader()->setDefaultSectionSize(80);
        CenteredDelegate1 *delegat1 = new CenteredDelegate1(ui->contract_expiration);
       ui->contract_expiration->setItemDelegate(delegat1);
        QFont font3 = ui->contract_expiration->font();
        font3.setPointSize(12);
        ui->contract_expiration->setFont(font3);

        model->item(0, 0)->setFont(QFont("Times", 10, QFont::Black));
        model->item(0, 2)->setFont(QFont("Times", 10, QFont::Black));
        model->item(0, 3)->setFont(QFont("Times", 10, QFont::Black));
    }
    else if(!query.exec()){
        qDebug() << "Query error:" << query.lastError().text();
        QMessageBox::critical(this, "查询失败", "数据库查询错误：" + query.lastError().text());
        return;
    }
}


void Property_Staff::on_contractid_findpushButton_11_clicked()
{
    QAbstractItemModel *currentModel = ui->rent_contract_tableView->model();
    if (currentModel) {
        delete currentModel; // 删除旧模型，防止内存泄漏
        ui->rent_contract_tableView->setModel(nullptr);
    }

    QString input_info = ui->contractid_lineEdit_16->text().trimmed();
    if(input_info.isEmpty()){
        QMessageBox::warning(this, "输入错误", "请输入正确的合同信息！");
        return;
    }

    QSqlQuery query;


    query.prepare("SELECT space_id, ownername, start_date,end_date,fee,payment_status FROM PARKING_RENTAL_NEW WHERE rental_id = ?");
    query.addBindValue(input_info);
    if(query.exec() && query.next()){
        // 提取数据-根据ID检索到信息
        QString space_id = query.value("space_id").toString();
        QString ownername = query.value("ownername").toString();
        QString payment_status = query.value("payment_status").toString();
        QString start_date = query.value("start_date").toString();
        QString end_date = query.value("end_date").toString();
        // 设置表头和数据
        QStandardItemModel *model = new QStandardItemModel(1, 6, this);
        QStringList headers = {"合同ID","车位ID", "租借人姓名", "合同付款状态", "开始租借日期","结束租借日期"};
        model->setHorizontalHeaderLabels(headers);
        model->setItem(0, 0, new QStandardItem(input_info));
        model->setItem(0, 1, new QStandardItem(space_id));
        model->setItem(0, 2, new QStandardItem(ownername));
        model->setItem(0, 3, new QStandardItem(payment_status));
        model->setItem(0, 4, new QStandardItem(start_date));
        model->setItem(0, 5, new QStandardItem(end_date));

        // 设置表格属性
        ui->rent_contract_tableView->setModel(model);
        ui->rent_contract_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->rent_contract_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->rent_contract_tableView->verticalHeader()->hide();
        ui->rent_contract_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->rent_contract_tableView->verticalHeader()->setDefaultSectionSize(80);
        CenteredDelegate1 *delegat1 = new CenteredDelegate1(ui->rent_contract_tableView);
        ui->rent_contract_tableView->setItemDelegate(delegat1);
        QFont font3 = ui->rent_contract_tableView->font();
        font3.setPointSize(12);
        ui->rent_contract_tableView->setFont(font3);

        model->item(0, 0)->setFont(QFont("Times", 10, QFont::Black));
        model->item(0, 2)->setFont(QFont("Times", 10, QFont::Black));
        model->item(0, 3)->setFont(QFont("Times", 10, QFont::Black));
    }
    else if(!query.exec()){
        qDebug() << "Query error:" << query.lastError().text();
        QMessageBox::critical(this, "查询失败", "数据库查询错误：" + query.lastError().text());
        return;
    }
}

void Property_Staff::on_contract_changepushButton_10_clicked()
{

    update_carfield_rental_information *w=new update_carfield_rental_information();
    w->show();
}

void Property_Staff::on_filter_pushButton_5_clicked()
{
    //carfield_manage_tableView_2
    // 清空 tableView 的内容并释放旧模型内存
    QAbstractItemModel *currentModel = ui->carfield_manage_tableView_2->model();
    if (currentModel) {
        delete currentModel; // 删除旧模型，防止内存泄漏
        ui->carfield_manage_tableView_2->setModel(nullptr);
    }
    QSqlQuery query;
    // 查询所有状态为vacant的车位
    query.prepare("SELECT space_id, location,  space_status FROM PARKING_SPACE_NEW WHERE space_status = 'vacant'");

    if (!query.exec()) {
        qDebug() << "查询错误:" << query.lastError().text();
        QMessageBox::critical(this, "查询失败", "数据库错误:" + query.lastError().text());
        return;
    }

    // 创建表格模型
    QStandardItemModel *model = new QStandardItemModel(0, 3, this);
    QStringList headers = {"车位ID", "车位位置", "车位状态"};
    model->setHorizontalHeaderLabels(headers);

    // 填充查询结果
    int row = 0;
    while (query.next()) {
        model->setItem(row, 0, new QStandardItem(query.value("space_id").toString()));
        model->setItem(row, 1, new QStandardItem(query.value("location").toString()));
        // model->setItem(row, 2, new QStandardItem(query.value("ownername").toString()));
        model->setItem(row, 2, new QStandardItem(query.value("space_status").toString()));
        row++;
    }

    // 设置表格属性
    ui->carfield_manage_tableView_2->setModel(model);
    ui->carfield_manage_tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->carfield_manage_tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->carfield_manage_tableView_2->verticalHeader()->hide();
    ui->carfield_manage_tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->carfield_manage_tableView_2->verticalHeader()->setDefaultSectionSize(80);
    CenteredDelegate1 *delegat1 = new CenteredDelegate1(ui->carfield_manage_tableView_2);
    ui->carfield_manage_tableView_2->setItemDelegate(delegat1);
    QFont font3 = ui->carfield_manage_tableView_2->font();
    font3.setPointSize(12);
    ui->carfield_manage_tableView_2->setFont(font3);


    // 设置字体样式
    for (int i = 0; i < row; i++) {
        model->item(i, 0)->setFont(QFont("Times", 10, QFont::Black));
        model->item(i, 1)->setFont(QFont("Times", 10, QFont::Black));
    }

    // 显示结果状态
    if (row == 0) {
        QMessageBox::information(this, "查询结果", "未找到空闲车位信息");
    }

}

void Property_Staff::on_contract_addpushButton_9_clicked()
{
    add_carfield_rental *w=new add_carfield_rental();
    w->show();
}

void Property_Staff::on_approveButton_clicked()
{
    // 获取选中的申请记录
    QModelIndexList selection = ui->renter_line_tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "操作提示", "请先选择一行申请记录");
        return;
    }

    // 获取申请ID
    QString applicationId = ui->renter_line_tableView->model()
                                ->index(selection.first().row(), 0).data().toString();

    QSqlQuery query;

    try {
        // 1. 更新申请状态为已分配
        query.prepare("UPDATE PARKING_APPLICATION_NEW SET status = 'allocated' WHERE application_id = ?");
        query.addBindValue(applicationId);
        if (!query.exec()) throw query.lastError().text();

        // 2. 查询申请详情
        query.prepare("SELECT space_id, applicantname, start_rent_time, end_rent_time FROM PARKING_APPLICATION_NEW WHERE application_id = ?");
        query.addBindValue(applicationId);
        if (!query.exec() || !query.next()) throw "申请记录不存在";

        QString spaceId = query.value("space_id").toString();
        QString applicantName = query.value("applicantname").toString();
        QString startDateStr = query.value("start_rent_time").toString();
        QString endDateStr = query.value("end_rent_time").toString();

        QDate startDate = QDate::fromString(startDateStr, "yyyy-MM-dd");
        QDate endDate = QDate::fromString(endDateStr, "yyyy-MM-dd");

        if (!startDate.isValid() || !endDate.isValid()) throw "日期格式错误";
        if (startDate >= endDate) throw "结束日期必须晚于开始日期";

        // 3. 计算租赁费用
        qint64 days = startDate.daysTo(endDate);
        double fee = 0.0;

        if (days <= 30) fee = 500.0;
        else if (days <= 90) fee = 800.0;
        else if (days <= 180) fee = 1999.99;
        else if (days <= 365) fee = 3699.99;
        else fee = ((days + 364) / 365) * 3000.0;

        QString feeStr = QString::number(fee, 'f', 2);

        // 4. 插入租赁记录
        query.prepare("INSERT INTO PARKING_RENTAL_NEW (space_id, ownername, start_date, end_date, fee, payment_status) VALUES (?, ?, ?, ?, ?, ?)");
        query.addBindValue(spaceId);
        query.addBindValue(applicantName);
        query.addBindValue(startDateStr);
        query.addBindValue(endDateStr);
        query.addBindValue(feeStr);
        query.addBindValue("unpaid");
        if (!query.exec()) throw query.lastError().text();

        // 5. 更新车位状态
        query.prepare("UPDATE PARKING_SPACE_NEW SET space_status = 'rented', ownername = ? WHERE space_id = ?");
        query.addBindValue(applicantName);
        query.addBindValue(spaceId);
        if (!query.exec()) throw query.lastError().text();

        // 6. 查询业主ID
        query.prepare("SELECT owner_id FROM OWNER WHERE ownername = ?");
        query.addBindValue(applicantName);
        if (!query.exec() || !query.next()) throw "未找到匹配的业主信息";

        int ownerId = query.value("owner_id").toInt();

        // 7. 插入缴费记录
        query.prepare("INSERT INTO PAYMENT_NEW (owner_id, total, payment_date, method, payment_status, payment_type) VALUES (?, ?, ?, ?, ?, ?)");
        query.addBindValue(ownerId);
        query.addBindValue(feeStr);
        query.addBindValue("");  // payment_date 为空
        query.addBindValue("offline");  // 设置为合法的 'offline'，也可根据实际改 'online'
        query.addBindValue("unpaid");
        query.addBindValue("carspace_fee");
        if (!query.exec()) throw query.lastError().text();

        // 提交事务
        if (!db.commit()) throw db.lastError().text();

        QMessageBox::information(this, "操作成功", "申请已成功分配车位");
    }
    catch (const QString& error) {
        // 回滚事务
        db.rollback();
        QMessageBox::critical(this, "操作失败", "处理申请时发生错误：\n" + error);
    }
}

void Property_Staff::on_pushButton_15_clicked()
{

    QTableView *tableView = ui->repair_tableView;
    // 删除旧模型防止内存泄漏
    QStandardItemModel* oldModel = qobject_cast<QStandardItemModel*>(tableView->model());
    if (oldModel) delete oldModel;

    QStandardItemModel* model = new QStandardItemModel(0,9,this);
    QStringList headers;
    headers << "维修编号" << "业主姓名"<< "楼号"<<"单元号"<<"房间"<<"维修问题描述"<<"申请日期"<< "维修分配状态"<<"维修进程推进时间";
    model->setHorizontalHeaderLabels(headers);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);
    // 表格属性设置移到此处
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->verticalHeader()->setDefaultSectionSize(80);
    CenteredDelegate1 *delegat1 = new CenteredDelegate1(tableView);
    tableView->setItemDelegate(delegat1);
    QFont font3 = tableView->font();
    font3.setPointSize(12);
    tableView->setFont(font3);
    QString repair_id=ui->lineEdit_4->text().trimmed();
    QSqlQuery query;
    query.prepare("SELECT owner_id,status,description,report_time FROM REPAIR WHERE repair_id=?");
    query.addBindValue(repair_id);
    if(!query.exec()){
        qDebug() << "维修信息查询失败:" << query.lastError().text();
        return;
    }

    if(!query.next()){
        QMessageBox::information(this, "提示", "未找到维修编号为" + repair_id + "的记录");
        return;
    }
    QString owner_id = query.value("owner_id").toString();
    QString status = query.value("status").toString();
    QString owner_description = query.value("description").toString();
    QString report_time = query.value("report_time").toString();

    // 查询业主
    QSqlQuery userQuery;
    QString user_id;
    userQuery.prepare("SELECT user_id FROM OWNER WHERE owner_id=?");
    userQuery.addBindValue(owner_id);
    if (!userQuery.exec() || !userQuery.next()) {
        qDebug() << "查询业主账号失败:" << userQuery.lastError().text();
    }
    else{
        user_id=userQuery.value("user_id").toString();
    }
    QSqlQuery nameQuery;
    nameQuery.prepare("SELECT username FROM USER_ACCOUNT WHERE user_id=?");
    nameQuery.addBindValue(user_id);
    if (!nameQuery.exec() || !nameQuery.next()) {
        qDebug() << "查询业主姓名失败:" << nameQuery.lastError().text();
    }
    QString ownername = nameQuery.value("username").toString();

    // 查询房产信息
    QSqlQuery propertyQuery;
    propertyQuery.prepare("SELECT building, unit, room FROM PROPERTY_NEW WHERE owner_id = ?");
    propertyQuery.addBindValue(owner_id);
    if (!propertyQuery.exec() || !propertyQuery.next()) {
        qDebug() << "查询房产信息失败:" << propertyQuery.lastError().text();
    }
    QString building = propertyQuery.value("building").toString();
    QString unit = propertyQuery.value("unit").toString();
    QString room = propertyQuery.value("room").toString();
    // 添加到表格
    int row = model->rowCount();
    model->setItem(row, 0, new QStandardItem(repair_id));
    model->setItem(row, 1, new QStandardItem(ownername));
    model->setItem(row, 2, new QStandardItem(building));
    model->setItem(row, 3, new QStandardItem(unit));
    model->setItem(row, 4, new QStandardItem(room));
    model->setItem(row, 5, new QStandardItem(owner_description));
    model->setItem(row, 6, new QStandardItem(report_time));
    model->setItem(row, 7, new QStandardItem(status));
    model->setItem(row, 8, new QStandardItem(""));
    CenteredDelegate1 *delegat2 = new CenteredDelegate1(tableView);
    tableView->setItemDelegate(delegat2);
    QFont font2 = tableView->font();
    font3.setPointSize(12);
    tableView->setFont(font3);
}

void Property_Staff::on_chose_pushButton_5_clicked()
{
    QModelIndexList selectedIndexes = ui->repair_tableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "提示", "请选择一个维修订单");
        return;
    }
    QModelIndex index = selectedIndexes.first();
    QString repairId = index.sibling(index.row(), 0).data().toString(); // 维修编号在第0列

    // 获取选中的员工
    int currentIndex = ui->chose_staff_comboBox->currentIndex();
    if (currentIndex <= 0) { // 跳过"请选择员工"选项
        QMessageBox::warning(this, "提示", "请选择要分配的员工");
        return;
    }
    QString staffId = ui->chose_staff_comboBox->itemData(currentIndex).toString();
    QString staffName = ui->chose_staff_comboBox->currentText();

    // 更新维修订单状态和分配员工
    QSqlQuery query;
    query.prepare("UPDATE REPAIR SET status = 'processing' WHERE repair_id = ?");

    query.addBindValue(repairId);
    if (query.exec()) {
        // 记录维修进度
        query.clear();
        query.prepare("INSERT INTO REPAIR_PROGRESS (repair_id, progress_time, status, description,staff_id) VALUES (?, CURRENT_TIMESTAMP, 'assigned', ?,?)");
        query.addBindValue(repairId);
        query.addBindValue(QString("已分配给%1").arg(staffName));
        query.addBindValue(staffId);
        if (query.exec()) {
            QMessageBox::information(this, "成功", "维修订单已成功分配给" + staffName);
            // 刷新维修表格
            on_Repair_Button_4_clicked();
        } else {
            QMessageBox::critical(this, "错误", "记录维修进度失败: " + query.lastError().text());
        }
    } else {
        QMessageBox::critical(this, "错误", "分配维修订单失败: " + query.lastError().text());
    }

}

void Property_Staff::on_find_my_repairpushButton_5_clicked() {
    // 1. 初始化表格模型和表头
    QStandardItemModel *model = new QStandardItemModel(0, 8, this);
    QStringList headers;
    headers << "维修编号" << "业主姓名" << "联系电话" << "维修地址"
            << "维修内容" << "报修时间" << "维修状态" << "进度时间";
    model->setHorizontalHeaderLabels(headers);
    ui->My_repair_tableView->setModel(model);
    ui->My_repair_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 2. 获取当前员工ID
    QString staffId = ui->staff_id_lineEdit_6->text().trimmed();
    if (staffId.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入员工ID");
        return;
    }

    // 3. 查询维修进度表（主表）
    QSqlQuery rpQuery;
    rpQuery.prepare("SELECT repair_id, status, progress_time FROM REPAIR_PROGRESS WHERE staff_id = ?");
    rpQuery.addBindValue(staffId);
    if (!rpQuery.exec()) {
        qDebug() << "维修进度查询失败:" << rpQuery.lastError().text();
        QMessageBox::critical(this, "错误", "无法获取维修订单数据");
        return;
    }

    // 4. 遍历结果并关联查询
    while (rpQuery.next()) {
        // 4.1 提取维修进度表基本信息
        QString repair_id = rpQuery.value("repair_id").toString();
        QString status = rpQuery.value("status").toString();
        QString progress_time = rpQuery.value("progress_time").toString();

        // 4.2 查询维修表获取业主ID和维修详情
        QSqlQuery repairQuery;
        repairQuery.prepare("SELECT owner_id, description, report_time FROM REPAIR WHERE repair_id = ?");
        repairQuery.addBindValue(repair_id);
        if (!repairQuery.exec() || !repairQuery.next()) {
            qDebug() << "维修信息查询失败(repair_id:" << repair_id << "):" << repairQuery.lastError().text();
            continue; // 跳过当前记录继续处理下一条
        }
        QString owner_id = repairQuery.value("owner_id").toString();
        QString description = repairQuery.value("description").toString();
        QString report_time = repairQuery.value("report_time").toString();

        // 4.3 查询业主表获取用户ID
        QSqlQuery ownerQuery;
        ownerQuery.prepare("SELECT user_id FROM OWNER WHERE owner_id = ?");
        ownerQuery.addBindValue(owner_id);
        if (!ownerQuery.exec() || !ownerQuery.next()) {
            qDebug() << "业主信息查询失败(owner_id:" << owner_id << "):" << ownerQuery.lastError().text();
            continue;
        }
        QString user_id = ownerQuery.value("user_id").toString();

        // 4.4 查询用户表获取联系方式
        QSqlQuery userQuery;
        userQuery.prepare("SELECT username, phone FROM USER_ACCOUNT WHERE user_id = ?");
        userQuery.addBindValue(user_id);
        if (!userQuery.exec() || !userQuery.next()) {
            qDebug() << "用户信息查询失败(user_id:" << user_id << "):" << userQuery.lastError().text();
            continue;
        }
        QString username = userQuery.value("username").toString();
        QString phone = userQuery.value("phone").toString();

        // 4.5 查询房产表获取地址信息
        QSqlQuery propertyQuery;
        propertyQuery.prepare("SELECT building, unit, room FROM PROPERTY_NEW WHERE owner_id = ?");
        propertyQuery.addBindValue(owner_id);
        if (!propertyQuery.exec() || !propertyQuery.next()) {
            qDebug() << "房产信息查询失败(owner_id:" << owner_id << "):" << propertyQuery.lastError().text();
            continue;
        }
        QString address = QString("%1-%2-%3").arg(
            propertyQuery.value("building").toString(),
            propertyQuery.value("unit").toString(),
            propertyQuery.value("room").toString()
            );

        // 5. 添加数据到表格模型
        int row = model->rowCount();
        model->setItem(row, 0, new QStandardItem(repair_id));
        model->setItem(row, 1, new QStandardItem(username));
        model->setItem(row, 2, new QStandardItem(phone));
        model->setItem(row, 3, new QStandardItem(address));
        model->setItem(row, 4, new QStandardItem(description));
        model->setItem(row, 5, new QStandardItem(report_time));
        model->setItem(row, 6, new QStandardItem(status));
        model->setItem(row, 7, new QStandardItem(progress_time));
    }

    // 6. 设置表格属性
    ui->My_repair_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->My_repair_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->My_repair_tableView->verticalHeader()->setDefaultSectionSize(80);
    CenteredDelegate1 *delegat1 = new CenteredDelegate1(ui->My_repair_tableView);
    ui->My_repair_tableView->setItemDelegate(delegat1);
    QFont font3 = ui->My_repair_tableView->font();
    font3.setPointSize(12);
    ui->My_repair_tableView->setFont(font3);


    // 7. 空数据提示
    if (model->rowCount() == 0) {
        QMessageBox::information(this, "提示", "未查询到您负责的维修订单");
    }
}

void Property_Staff::on_confirm_progress_changepushButton_5_clicked()
{
    qDebug() << "=== 开始处理维修进度更新 ===";

    QModelIndexList selectedIndexes = ui->My_repair_tableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        qDebug() << "未选择维修订单，操作取消";
        QMessageBox::warning(this, "提示", "请选择一个您的维修订单");
        return;
    }

    QModelIndex index = selectedIndexes.first();
    QString repairId = index.sibling(index.row(), 0).data().toString();
    qDebug() << "选中的维修ID:" << repairId;

    int currentIndex = ui->repair_progress_comboBox->currentIndex();
    if (currentIndex < 0) {
        qDebug() << "未选择进度，当前索引:" << currentIndex;
        QMessageBox::warning(this, "提示", "请选择当前进度");
        return;
    }

    QString new_progress = ui->repair_progress_comboBox->currentText();
    qDebug() << "选择的新进度:" << new_progress;

    // 开启数据库事务确保操作原子性
    QSqlDatabase db = QSqlDatabase::database();
    qDebug() << "数据库连接状态:" << db.isOpen();
    qDebug() << "数据库连接名称:" << db.connectionName();

    if (!db.transaction()) {
        qDebug() << "开启事务失败:" << db.lastError().text();
        QMessageBox::critical(this, "错误", "无法开启事务: " + db.lastError().text());
        return;
    }

    qDebug() << "=== 开始更新 REPAIR_PROGRESS 表 ===";
    QSqlQuery query;
    // 更新维修进度表
    query.prepare("UPDATE REPAIR_PROGRESS SET status = ? WHERE repair_id = ?");
    query.addBindValue(new_progress);
    query.addBindValue(repairId);

    qDebug() << "执行SQL:" << query.lastQuery();
    qDebug() << "绑定值1:" << new_progress;
    qDebug() << "绑定值2:" << repairId;

    if (!query.exec()) {
        qDebug() << "更新REPAIR_PROGRESS失败:" << query.lastError().text();
        db.rollback();
        QMessageBox::critical(this, "错误", "修改维修订单失败: " + query.lastError().text());
        return;
    }

    qDebug() << "REPAIR_PROGRESS更新成功，受影响行数:" << query.numRowsAffected();
    query.clear();

    // 更新主维修表状态
    qDebug() << "=== 开始更新 REPAIR 表 ===";
    qDebug() << "进度值判断:" << new_progress << (new_progress == "completed" ? "是completed" : "不是completed");

    if(new_progress == "completed") {
        query.prepare("UPDATE REPAIR SET  status = ? WHERE repair_id = ?");
        QString progress = "completed";
        query.addBindValue(progress);
        query.addBindValue(repairId);

        qDebug() << "执行SQL (completed):" << query.lastQuery();
        qDebug() << "绑定值1:" << progress;
        qDebug() << "绑定值2:" << repairId;

        if (!query.exec()) {
            qDebug() << "更新REPAIR表(completed)失败:" << query.lastError().text();
            db.rollback();
            QMessageBox::critical(this, "错误", "更新维修状态失败: " + query.lastError().text());
            return;
        }
    }
    else {
        query.prepare("UPDATE REPAIR SET status = ? WHERE repair_id = ?");
        QString progress = "processing";
        query.addBindValue(progress);
        query.addBindValue(repairId);

        qDebug() << "执行SQL (processing):" << query.lastQuery();
        qDebug() << "绑定值1:" << progress;
        qDebug() << "绑定值2:" << repairId;

        if (!query.exec()) {
            qDebug() << "更新REPAIR表(processing)失败:" << query.lastError().text();
            db.rollback();
            QMessageBox::critical(this, "错误", "更新维修状态失败: " + query.lastError().text());
            return;
        }
    }

    // 提交事务
    qDebug() << "准备提交事务...";
    if (db.commit()) {
        qDebug() << "事务提交成功";
        QMessageBox::information(this, "成功", "维修状态已成功更新为" + new_progress);
        on_find_my_repairpushButton_5_clicked(); // 刷新表格
    } else {
        qDebug() << "事务提交失败:" << db.lastError().text();
        db.rollback();
        QMessageBox::critical(this, "错误", "事务提交失败: " + db.lastError().text());
    }

    qDebug() << "=== 维修进度更新处理结束 ===";
}

void Property_Staff::on_repair_id_pushButton_6_clicked()
{
    QString repairId=ui->repair_id_lineEdit_6->text().trimmed();
    if(repairId.isEmpty()){
        QMessageBox::warning(this,"输入提示","请输入维修订单编号");
        return;
    }
    loadFeedbackData(repairId,"");
}

void Property_Staff::on_staff_id_pushButton_5_clicked()
{
    QString staffId = ui->staff_id_lineEdit_7->text().trimmed();
    if (staffId.isEmpty()) {
        QMessageBox::warning(this, "输入提示", "请输入员工编号");
        return;
    }
    loadFeedbackData("", staffId);
}

void Property_Staff::loadFeedbackData(const QString &repairId, const QString &staffId)
{
    // 删除旧模型防止内存泄漏
    QAbstractItemModel *oldModel = ui->repair_feedback_tableView_2->model();
    if (oldModel) delete oldModel;

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels(QStringList() << "维修编号" << "员工编号" << "维修时间" << "维修评价");
    ui->repair_feedback_tableView_2->setModel(model);
    //ui->repair_feedback_tableView_2->resizeColumnsToContents();
    ui->repair_feedback_tableView_2->update();

    QString sql = "SELECT r.repair_id, rp.staff_id, rp.progress_time, r.evaluation "
                  "FROM REPAIR r INNER JOIN REPAIR_PROGRESS rp ON r.repair_id = rp.repair_id WHERE 1=1 ";
    QSqlQuery query;
    QList<QVariant> params;

    if (!repairId.isEmpty()) { sql += "AND r.repair_id = ? "; params << repairId; }
    if (!staffId.isEmpty())  { sql += "AND rp.staff_id = ? "; params << staffId; }

    query.prepare(sql);
    for (int i = 0; i < params.size(); ++i) query.addBindValue(params[i]);

    if (!query.exec()) {
        QMessageBox::critical(this, "查询失败", "无法获取维修反馈数据: " + query.lastError().text());
        return;
    }

    int row = 0;
    while (query.next()) {
        model->setItem(row, 0, new QStandardItem(query.value(0).toString()));
        model->setItem(row, 1, new QStandardItem(query.value(1).toString()));
        model->setItem(row, 2, new QStandardItem(query.value(2).toString()));
        model->setItem(row, 3, new QStandardItem(query.value(3).toString()));
        row++;
    }
    ui->repair_feedback_tableView_2->verticalHeader()->setDefaultSectionSize(80);
    CenteredDelegate1 *delegat1 = new CenteredDelegate1(ui->repair_feedback_tableView_2);
    ui->repair_feedback_tableView_2->setItemDelegate(delegat1);
    QFont font3 = ui->repair_feedback_tableView_2->font();
    font3.setPointSize(12);
    ui->repair_feedback_tableView_2->setFont(font3);

}

void Property_Staff::on_pushButton_16_clicked()
{

    //读取lineEdit_5中的值
    QString ownerId = ui->lineEdit_5->text();
    if (ownerId.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入业主ID");
        return;
    }
    //在数据库PAYMENT_NEW表中查询业主ID为ownerId的记录，并显示在tableView上
    QTableView *tableView = ui->tableView;
    QStandardItemModel* model = new QStandardItemModel(0, 6, this);
    QStringList headers;
    headers << "缴费id" << "业主id" << "需缴费用" << "缴费方式" << "缴费日期" << "缴费状态"<<"缴费类型";
    model->setHorizontalHeaderLabels(headers);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);
    //查询数据库
    QSqlQuery query;
   query.prepare("SELECT payment_id, owner_id, total, method, payment_date, payment_status, payment_type FROM PAYMENT_NEW WHERE owner_id = :ownerId");
    query.bindValue(":ownerId", ownerId.toInt());
    if (!query.exec()) {
        QMessageBox::critical(this, "查询失败", "查询缴费信息失败：" + query.lastError().text());
        return;
    }
    int row = 0;
    while (query.next()) {
        model->insertRow(row);
        model->setData(model->index(row, 0), query.value("payment_id").toInt());
        model->setData(model->index(row, 1), query.value("owner_id").toInt());
        model->setData(model->index(row, 2), query.value("total").toDouble());
        model->setData(model->index(row, 3), query.value("method").toString());
        model->setData(model->index(row, 4), query.value("payment_date").toDateTime());
        model->setData(model->index(row, 5), query.value("payment_status").toString());
         model->setData(model->index(row, 6), query.value("payment_type").toString());
        row++;
    }
    model->setRowCount(row);
    model->setColumnCount(7); // 修正为6列，与表头匹配
    tableView->setModel(model);
    tableView->verticalHeader()->setDefaultSectionSize(80);
    CenteredDelegate1 *delegat1 = new CenteredDelegate1(tableView);
    tableView->setItemDelegate(delegat1);
    QFont font3 = tableView->font();
    font3.setPointSize(12);
    tableView->setFont(font3);
    if (row == 0) {
        QMessageBox::information(this, "查询结果", "未找到对应的缴费信息");
    } else {
        QMessageBox::information(this, "查询结果", QString("找到 %1 条缴费信息").arg(row));
    }
    //清空lineEdit_5
    ui->lineEdit_5->clear();



}


void Property_Staff::on_pushButton_20_clicked()
{
    //读取lineEdit中的内容
    QString ownerId = ui->lineEdit->text();
    if (ownerId.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入业主ID");
        return;
    }
    //清除owner_information中的内容
    QTableView *tableView = ui->owner_information;
    QStandardItemModel* model = new QStandardItemModel(0, 5, this);
    QStringList headers;
    headers << "用户id" << "用户姓名" << "楼号" << "单元号" << "房间号";
    model->setHorizontalHeaderLabels(headers);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);
    //查询数据库
    QSqlQuery query;
    query.prepare("SELECT owner_id, ownername, building, unit, room FROM PROPERTY_NEW WHERE owner_id = :ownerId");
    query.bindValue(":ownerId", ownerId);
    if (!query.exec()) {
        QMessageBox::critical(this, "查询失败", "查询业主信息失败：" + query.lastError().text());
        return;
    }
    int row = 0;
    while (query.next()) {
        model->insertRow(row);
        model->setData(model->index(row, 0), query.value("owner_id").toInt());
        model->setData(model->index(row, 1), query.value("ownername").toString());
        model->setData(model->index(row, 2), query.value("building").toString());
        model->setData(model->index(row, 3), query.value("unit").toString());
        model->setData(model->index(row, 4), query.value("room").toString());
        row++;
    }
    model->setRowCount(row);
    model->setColumnCount(5); // 修正为5列，与表头匹配
    tableView->setModel(model);
    if (row == 0) {
        QMessageBox::information(this, "查询结果", "未找到对应的业主信息");
    } else {
        QMessageBox::information(this, "查询结果", QString("找到 %1 条业主信息").arg(row));
    }
    //清空lineEdit
    ui->lineEdit->clear();
    //设置表格不可编辑
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->verticalHeader()->setDefaultSectionSize(80);
    CenteredDelegate1 *delegat1 = new CenteredDelegate1(tableView);
    tableView->setItemDelegate(delegat1);
    QFont font3 = tableView->font();
    font3.setPointSize(12);
    tableView->setFont(font3);

}

void Property_Staff::on_pushButton_14_clicked()
{
    QTableView *tableView = ui->tableView;
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(tableView->model());
    if (!model) {
        QMessageBox::warning(this, "错误", "表格模型未正确设置");
        return;
    }
    int lastRow = model->rowCount() - 1;
    if (lastRow < 0) {
        QMessageBox::warning(this, "错误", "表格中没有数据");
        return;
    }

    QStandardItem* firstColumnItem = model->item(lastRow, 0);
    if (firstColumnItem && !firstColumnItem->text().isEmpty()) {
        QMessageBox::warning(this, "错误", "最后一行第一列必须为空才能插入数据！");
        return;
    }


    QString ownerIdStr = model->item(lastRow, 1)->text();
    QString total = model->item(lastRow, 2)->text();
    QString method = model->item(lastRow, 3)->text();
    QString paymentDate = model->item(lastRow, 4)->text();
    QString paymentStatus = model->item(lastRow, 5)->text();

    if (ownerIdStr.isEmpty() || total.isEmpty()) {
        QMessageBox::warning(this, "错误", "请确保除第一列外所有字段都已填写！");
        return;
    }

    QStringList validStatuses = {"paid", "unpaid", "unfinished"};
    if (!validStatuses.contains(paymentStatus.toLower())) {
        QMessageBox::warning(this, "错误", "支付状态必须是 'paid', 'unpaid' 或 'unfinished' 之一！");
        return;
    }

    // 根据 owner_id 查询 ownername
    int ownerId = ownerIdStr.toInt();
    QSqlQuery ownerQuery;
    ownerQuery.prepare("SELECT ownername FROM OWNER WHERE owner_id = :ownerId");
    ownerQuery.bindValue(":ownerId", ownerId);
    if (!ownerQuery.exec() ||!ownerQuery.next()) {
        QMessageBox::critical(this, "查询失败", "查询业主姓名失败：" + ownerQuery.lastError().text());
        return;
    }
    QString ownername = ownerQuery.value("ownername").toString();

    QSqlQuery query;
    query.prepare("INSERT INTO PAYMENT_NEW (owner_id, ownername, total, method, payment_date, payment_status, payment_type) "
                  "VALUES (:ownerId, :ownername, :total, :method, :paymentDate, :paymentStatus, :paymentType)");
    query.bindValue(":ownerId", ownerId);
    query.bindValue(":ownername", ownername);
    query.bindValue(":total", total.toDouble());
    query.bindValue(":method", method);
    query.bindValue(":paymentDate", paymentDate);
    query.bindValue(":paymentStatus", paymentStatus);
    query.bindValue(":paymentType", "property_fee"); // 标记为物业费
    if (!query.exec()) {
        QMessageBox::critical(this, "插入失败", "插入缴费信息失败：" + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "插入成功", "缴费信息插入成功");

    QStandardItemModel*model2=new QStandardItemModel(0,7,this);
    QStringList headers2;
    headers2 << "缴费id"<<"业主id" <<"需缴费用"<<"缴费方式"<<"缴费日期"<<"缴费状态"<<"缴费类型";
    model2->setHorizontalHeaderLabels(headers2);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model2);

    bool success = query.exec(QStringLiteral("SELECT payment_id, owner_id, total, method, payment_date, payment_status, payment_type FROM PAYMENT_NEW"));
    if (!success) {
        QMessageBox::critical(this, u8"查询失败",
                              u8"缴费信息查询出错：\n" + query.lastError().text());
        return;
    }
    int row = 0;
    while (query.next()) {
        model2->insertRow(row);
        model2->setData(model2->index(row, 0), query.value(QStringLiteral("payment_id")).toInt());
        model2->setData(model2->index(row, 1), query.value(QStringLiteral("owner_id")).toInt());
        model2->setData(model2->index(row, 2), query.value(QStringLiteral("total")).toDouble());
        model2->setData(model2->index(row, 3), query.value(QStringLiteral("method")).toString());
        model2->setData(model2->index(row, 4), query.value(QStringLiteral("payment_date")).toDateTime());
        model2->setData(model2->index(row, 5), query.value(QStringLiteral("payment_status")).toString());
        model2->setData(model2->index(row, 6), query.value(QStringLiteral("payment_type")).toString());
        row++;
    }
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    tableView->setModel(model2);
    model2->setRowCount(row);
    model2->setColumnCount(7);
    ui->tableView->verticalHeader()->setDefaultSectionSize(80);
    CenteredDelegate1 *delegat1 = new CenteredDelegate1(tableView);
    tableView->setItemDelegate(delegat1);
    QFont font3 = tableView->font();
    font3.setPointSize(12);
    tableView->setFont(font3);
}
void Property_Staff::on_go_to_work_2_clicked()
{

    // 获取当前员工ID
    QString staffIdStr = ui->lineEdit_8->text();
    if(staffIdStr.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先输入员工ID");
        return;
    }

    int staffId = staffIdStr.toInt();
    if(staffId == 0 && staffIdStr != "0") {
        QMessageBox::warning(this, "错误", "无效的员工ID");
        return;
    }

    // 获取当前时间
    QDateTime currentTime = QDateTime::currentDateTime();
    QString formattedTime = currentTime.toString("yyyy-MM-dd hh:mm:ss");
    QString currentDate = currentTime.date().toString("yyyy-MM-dd");
    QTime currentHourMinute = currentTime.time();

    // 显示打卡时间（临时，若重复打卡会清空）
    ui->on_time->setText(formattedTime);

    // --------------------------
    // 核心：检查当天是否已有上班打卡记录
    // --------------------------
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT sign_in, sign_out FROM ATTENDANCE "
                       "WHERE staff_id = :staffId AND date = :date");
    checkQuery.bindValue(":staffId", staffId);
    checkQuery.bindValue(":date", currentDate);

    if(checkQuery.exec() && checkQuery.next()) {
        // 若查询到记录，判断是否已有上班打卡
        QString existingSignIn = checkQuery.value("sign_in").toString();
        if(!existingSignIn.isEmpty()) {
            // 已有上班打卡记录，禁止重复打卡
            //ui->on_time->clear(); // 清空打卡时间显示
            QMessageBox::warning(this, "打卡失败",
                                 QString("今日已在 %1 完成上班打卡，请勿重复操作")
                                     .arg(existingSignIn));
            //查找数据库ATTENDENCE表，staff_id字段中staffIdStr对应的那一行中，的sign_up字段中的值
            // 并显示到on_time lineEdit里面
            ui->on_time->setText(existingSignIn);
            // 禁用上班打卡按钮
            ui->go_to_work_2->setEnabled(false);
            // 启用下班打卡按钮
            ui->go_off_work_2->setEnabled(true);

            return;
        }
    } else if(!checkQuery.exec()) {
        // 数据库查询失败处理
        QMessageBox::critical(this, "数据库错误", "检查打卡记录失败: " + checkQuery.lastError().text());
        return;
    }

    // 判断考勤状态
    QString status = "normal";
    ui->status->clear(); // 先清空状态显示
    if(currentHourMinute > QTime(22, 0) && currentHourMinute <= QTime(23, 0)) {
        status = "late";
        ui->status->setText("迟到");
    } else if(currentHourMinute > QTime(23, 0)) {
        status = "absent";
        ui->status->setText("缺席");
    } else {
        ui->status->setText("正常");
    }

    // 插入打卡记录
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO ATTENDANCE (staff_id, date, sign_in, status) "
                        "VALUES (:staffId, :date, :signIn, :status)");
    insertQuery.bindValue(":staffId", staffId);
    insertQuery.bindValue(":date", currentDate);
    insertQuery.bindValue(":signIn", formattedTime);
    insertQuery.bindValue(":status", status);

    if(!insertQuery.exec()) {
        QMessageBox::critical(this, "数据库错误", "打卡失败: " + insertQuery.lastError().text());
        return;
    }

    // 提示信息（包含状态）
    QString statusText = (status == "late") ? "迟到" : (status == "absent" ? "缺席" : "正常");
    QMessageBox::information(this, "打卡成功",
                             QString("上班打卡时间: %1\n状态：%2")
                                 .arg(formattedTime)
                                 .arg(statusText));

    // 状态切换
    ui->go_to_work_2->setEnabled(false);
    ui->go_off_work_2->setEnabled(true);

}


void Property_Staff::on_go_off_work_2_clicked()
{
    // 获取当前员工ID
    QString staffIdStr = ui->lineEdit_8->text();
    if(staffIdStr.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先输入员工ID");
        return;
    }

    int staffId = staffIdStr.toInt();
    if(staffId == 0 && staffIdStr != "0") {
        QMessageBox::warning(this, "错误", "无效的员工ID");
        return;
    }

    // 获取当前时间
    QDateTime currentTime = QDateTime::currentDateTime();
    QString formattedTime = currentTime.toString("yyyy-MM-dd hh:mm:ss");
    QString currentDate = currentTime.date().toString("yyyy-MM-dd");

    // 显示打卡时间

    // 检查今日是否已打卡
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT id FROM ATTENDANCE WHERE staff_id = :staffId AND date = :date");
    checkQuery.bindValue(":staffId", staffId);
    checkQuery.bindValue(":date", currentDate);

    if(checkQuery.exec() && checkQuery.next()) {
        QMessageBox::warning(this, "打卡失败", "今日已打卡，请勿重复操作");
        return;
    }

    // 检查是否已经下班打卡
    QSqlQuery checkSignOutQuery;
    checkSignOutQuery.prepare("SELECT sign_out FROM ATTENDANCE WHERE staff_id = :staffId AND date = :date");
    checkSignOutQuery.bindValue(":staffId", staffId);
    checkSignOutQuery.bindValue(":date", currentDate);

    if(checkSignOutQuery.exec() && checkSignOutQuery.next()) {
        QString signOutTime = checkSignOutQuery.value(0).toString();
        if(!signOutTime.isEmpty()) {
            QMessageBox::warning(this, "打卡失败", "今日已下班打卡，请勿重复操作");
            ui->off_time->setText(signOutTime);
            return;
        }

    }


    // 把打卡时间插入到数据库ATTENDENCE表，staff_id字段中staffIdStr对应的那一行中，的sign_out字段中
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE ATTENDANCE SET sign_out = :signOut WHERE staff_id = :staffId AND date = :date");
    updateQuery.bindValue(":signOut", formattedTime);
    updateQuery.bindValue(":staffId", staffId);
    updateQuery.bindValue(":date", currentDate);
    ui->off_time->setText(formattedTime);


    if(!updateQuery.exec()) {
        QMessageBox::critical(this, "数据库错误", "打卡失败: " + updateQuery.lastError().text());
        return;
    }

    QMessageBox::information(this, "打卡成功", "下班打卡时间: " + formattedTime);


}

void Property_Staff::on_pushButton_19_clicked()
{
    // 清空员工ID输入框
    ui->lineEdit_8->clear();

    // 清空打卡时间显示
    ui->on_time->clear();
    ui->off_time->clear();

    // 清空状态显示（如果有status标签）
    if (ui->status) {
        ui->status->clear();
    }

    // 恢复按钮初始状态：启用上班打卡，禁用下班打卡
    ui->go_to_work_2->setEnabled(true);
    ui->go_off_work_2->setEnabled(false);

    // 提示用户界面已重置
    QMessageBox::information(this, "重置成功", "界面已恢复到初始状态");
}


void Property_Staff::on_pushButton_22_clicked()
{
    QString staff_id= ui->lineEdit_6->text();
    //在数据库LEAVE表中查询staff_id为staff_id的记录，并显示在tableView_3上
    QTableView *tableView = ui->tableView_3;
    QStandardItemModel* model = new QStandardItemModel(0, 7, this);
    QStringList headers;
    headers << "请假ID" << "员工ID" << "开始日期" << "结束日期" << "请假原因" << "请假类型"<<"请假状态";
    model->setHorizontalHeaderLabels(headers);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);
    //查询数据库
    QSqlQuery query;
    query.prepare("SELECT leave_id, staff_id, start_date, end_date, reason, type ,status FROM LEAVE WHERE staff_id =?");
    query.addBindValue( staff_id);
    if (!query.exec()) {
        QMessageBox::critical(this, "查询失败", "查询请假信息失败：" + query.lastError().text());
        return;
    }
    int row = 0;
    while (query.next()) {
        model->insertRow(row);
        model->setData(model->index(row, 0), query.value("leave_id").toInt());
        model->setData(model->index(row, 1), query.value("staff_id").toInt());
        model->setData(model->index(row, 2), query.value("start_date").toDate());
        model->setData(model->index(row, 3), query.value("end_date").toDate());
        model->setData(model->index(row, 4), query.value("reason").toString());
        model->setData(model->index(row, 5), query.value("type").toString());
        model->setData(model->index(row, 6), query.value("status").toString());
        row++;
    }
    model->setRowCount(row);
    model->setColumnCount(7); // 修正为6列，与表头匹配
    tableView->setModel(model);
    if (row == 0) {
        QMessageBox::information(this, "查询结果", "未找到对应的请假信息");
    } else {
        QMessageBox::information(this, "查询结果", QString("找到 %1 条请假信息").arg(row));
    }
    // 清空lineEdit
    ui->lineEdit_6->clear();
    // 设置表格不可编辑
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->verticalHeader()->setDefaultSectionSize(80);
    CenteredDelegate1 *delegat1 = new CenteredDelegate1(tableView);
    tableView->setItemDelegate(delegat1);
    QFont font3 = tableView->font();
    font3.setPointSize(12);
    tableView->setFont(font3);

}


void Property_Staff::on_pushButton_23_clicked()
{
    //pushButton删除选中行业主信息设置
    QModelIndexList selection = ui->owner_information->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "操作提示", "请先选择一行业主房产信息");
        return;
    }
    int row = selection.first().row();
    QString ownerId = ui->owner_information->model()->index(row, 0).data().toString();
    deleteowner(ownerId);
    // });
}
bool Property_Staff::deleteOwnerInformation(const QString &ownerId) {
    QSqlDatabase db = QSqlDatabase::database();

    // 1. 开启事务
    if (!db.transaction()) {
        qDebug() << "事务开启失败:" << db.lastError().text();
        return false;
    }

    try {
        // 2. 查询该业主所有的property_id
        QSqlQuery query;
        query.prepare("SELECT property_id FROM PROPERTY WHERE owner_id = ?");
        query.addBindValue(ownerId);
        if (!query.exec()) {
            throw QString("查询property_id失败: %1").arg(query.lastError().text());
        }

        // 3. 收集所有property_id并删除关联数据
        QVariantList propertyIds;
        while (query.next()) {
            QString propertyId = query.value(0).toString();
            propertyIds << propertyId;

            // 3.1 删除FAMILY_MEMBER表中相关记录
            QSqlQuery deleteFamilyQuery;
            deleteFamilyQuery.prepare("DELETE FROM FAMILY_MEMBER WHERE property_id = ?");
            deleteFamilyQuery.addBindValue(propertyId);
            if (!deleteFamilyQuery.exec()) {
                throw QString("删除家庭成员失败: %1").arg(deleteFamilyQuery.lastError().text());
            }
            qDebug() << "删除家庭成员成功，property_id:" << propertyId;

            // 3.2 级联删除维修相关记录
            // 3.2.1 查询当前房产的所有repair_id
            QSqlQuery repairQuery;
            repairQuery.prepare("SELECT repair_id FROM REPAIR WHERE property_id = ?");
            repairQuery.addBindValue(propertyId);
            if (!repairQuery.exec()) {
                throw QString("查询维修记录失败: %1").arg(repairQuery.lastError().text());
            }

            // 3.2.2 收集repair_id并删除REPAIR_PROGRESS表记录
            QVariantList repairIds;
            while (repairQuery.next()) {
                QString repairId = repairQuery.value(0).toString();
                repairIds << repairId;
                // 删除维修进度记录
                QSqlQuery deleteProgressQuery;
                deleteProgressQuery.prepare("DELETE FROM REPAIR_PROGRESS WHERE repair_id = ?");
                deleteProgressQuery.addBindValue(repairId);
                if (!deleteProgressQuery.exec()) {
                    throw QString("删除维修进度失败: %1").arg(deleteProgressQuery.lastError().text());
                }
                qDebug() << "删除维修进度成功，repair_id:" << repairId;
            }

            // 3.2.3 删除REPAIR表中当前房产的记录
            QSqlQuery deleteRepairQuery;
            deleteRepairQuery.prepare("DELETE FROM REPAIR WHERE property_id = ?");
            deleteRepairQuery.addBindValue(propertyId);
            if (!deleteRepairQuery.exec()) {
                throw QString("删除维修信息失败: %1").arg(deleteRepairQuery.lastError().text());
            }
            qDebug() << "删除维修信息成功，共删除" << repairIds.size() << "条记录，property_id:" << propertyId;
        }

        // 4. 删除PROPERTY表中相关记录
        if (!propertyIds.isEmpty()) {
            QSqlQuery deletePropertyQuery;
            deletePropertyQuery.prepare("DELETE FROM PROPERTY WHERE owner_id = ?");
            deletePropertyQuery.addBindValue(ownerId);
            if (!deletePropertyQuery.exec()) {
                throw QString("删除房产信息失败: %1").arg(deletePropertyQuery.lastError().text());
            }
            qDebug() << "删除房产信息成功，共删除" << propertyIds.size() << "条记录";
        }

        // 5. 最后删除OWNER表中记录
        QSqlQuery deleteOwnerQuery;
        deleteOwnerQuery.prepare("DELETE FROM OWNER WHERE owner_id = ?");
        deleteOwnerQuery.addBindValue(ownerId);
        if (!deleteOwnerQuery.exec()) {
            throw QString("删除业主信息失败: %1").arg(deleteOwnerQuery.lastError().text());
        }

        // 6. 提交事务
        if (!db.commit()) {
            throw QString("事务提交失败: %1").arg(db.lastError().text());
        }

        qDebug() << "成功删除业主所有信息，owner_id:" << ownerId;
        return true;
    }
    catch (const QString &error) {
        // 发生错误时回滚事务
        db.rollback();
        qDebug() << "删除操作失败:" << error;
        return false;
    }
}
void Property_Staff::deleteowner(const QString &ownerId) {
    if (deleteOwnerInformation(ownerId)) {
        QMessageBox::information(this, "操作成功", "已删除该业主信息");
        on_reflash_clicked();
    } else {
        QMessageBox::critical(this, "操作失败", "删除业主失败，请检查数据库连接");
    }
}

void Property_Staff::on_tabWidget_2_tabBarClicked(int index1)
{
    if(index1 == 1)
    {
        // 获取当前登录用户的 user_id
        int currentUserId1 = Login::currentUserId;
        if (currentUserId1 <= 0) {
            QMessageBox::warning(this, "未登录", "请先登录以查看请假信息");
            return;
        }

        // 创建预处理查询（跨三表关联）
        QSqlQuery query;
        query.prepare(
            "SELECT l.leave_id, l.staff_id, l.start_date, l.end_date, l.reason, l.type, l.status "
            "FROM LEAVE l "
            "JOIN STAFF s ON l.staff_id = s.staff_id "  // LEAVE表与STAFF表关联
            "JOIN USER_ACCOUNT ua ON s.user_id = ua.user_id "  // STAFF表与USER_ACCOUNT表关联
            "WHERE ua.user_id = :user_id");  // 通过 user_id 过滤

        query.bindValue(":user_id", currentUserId1);  // 绑定全局变量

        // 执行查询
        if (!query.exec()) {
            QMessageBox::critical(this, "查询失败",
                                  "查询请假信息失败：" + query.lastError().text());
            return;
        }

        // 创建数据模型并设置表头
        QStandardItemModel* model = new QStandardItemModel(0, 7, this);
        model->setHorizontalHeaderLabels({
            "请假ID", "员工ID", "开始日期", "结束日期", "请假原因", "请假类型", "请假状态"
        });

        // 填充数据到模型
        int row = 0;
        while (query.next()) {
            model->insertRow(row);
            model->setData(model->index(row, 0), query.value("leave_id"));
            model->setData(model->index(row, 1), query.value("staff_id"));
            model->setData(model->index(row, 2), query.value("start_date").toString());
            model->setData(model->index(row, 3), query.value("end_date").toString());
            model->setData(model->index(row, 4), query.value("reason"));
            model->setData(model->index(row, 5), query.value("type"));
            model->setData(model->index(row, 6), query.value("status"));
            row++;
        }

        // 设置模型到表格视图
        ui->tableView_3->setModel(model);
        ui->tableView_3->resizeColumnsToContents();
        ui->tableView_3->verticalHeader()->setDefaultSectionSize(80);
        CenteredDelegate1 *delegat1 = new CenteredDelegate1(ui->tableView_3);
        ui->tableView_3->setItemDelegate(delegat1);
        QFont font3 = ui->tableView_3->font();
        font3.setPointSize(12);
        ui->tableView_3->setFont(font3);

        // 提示查询结果
        if (row == 0) {
            QMessageBox::information(this, "查询结果", "未找到对应的请假信息");
        } else {
            QMessageBox::information(this, "查询结果", QString("找到 %1 条请假信息").arg(row));
        }
    }

}

void Property_Staff::on_caridentify_clicked()
{

    ui->stackedWidget->setCurrentIndex(7);

    ui->Checkin_Button->setChecked(false);
    ui->Leave_Button_2->setChecked(false);
    ui->Attendence_Button_3->setChecked(false);
    ui->Repair_Button_4->setChecked(false);
    ui->Parking_Button->setChecked(false);
    ui->Feemanagement_Button->setChecked(false);
    ui->Ownermanagement_Button_2->setChecked(false);
    ui->caridentify->setChecked(true);
}

void Property_Staff::on_Vehicle_Recognition_clicked()
{
    Vehicle_Recognition *w=new Vehicle_Recognition();
    w->show();
}

void Property_Staff::on_toolButton_clicked()
{
    this->close();
    if (!loginWindow) {
        loginWindow = new Login();  // 可以安全使用 Login 类
    }
    loginWindow->show();
    Login::currentUserId = -1;
    Login::currentUsername = "";

}
