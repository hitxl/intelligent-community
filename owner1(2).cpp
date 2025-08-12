#include "owner1(2).h"
#include "ui_owner1(2).h"
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
#include"centereddelegate.h"
#include "login.h"
#include <QSqlRecord>
#include <QDate>
#include"parking_apply.h"
#include"owner_pay.h"
#include"login.h"
#include"owner_evaluation.h"
Owner::Owner(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Owner)
    ,loginWindow(nullptr)
{
    ui->setupUi(this);
    resize(800, 800);
    ui->stackedWidget->setCurrentIndex(0);
    initDatabase();
    initTableModels();
    setupTableViews();
}
Owner::~Owner()
{
    if (db.isOpen()) {
        db.close();
    }
    delete ui;
    Login::currentUsername = "";
}
// 连接数据库
void Owner::initDatabase()
{
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("IntelligentCommunity.db");
        if (!db.open()) {
            QMessageBox::critical(this, "数据库错误", "无法连接数据库：" + db.lastError().text());
            return;
        }
    }
    qDebug() << "数据库连接成功";
    qDebug() << "Database Name:" << db.databaseName();
    qDebug() << "Database Open:" << db.isOpen();
    qDebug() << "Tables in DB:" << db.tables();
}
void Owner::on_toolButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    calculateAttendance();  // 加载缴费信息到模型
    loadParkingApplicationData();
    loadParkingSpaceData();
    loadmyParkingSpaceData();
}
void Owner::on_toolButton_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    calculateAttendance();
    loadParkingApplicationData();
    loadParkingSpaceData();
    loadmyParkingSpaceData();
}

void Owner::on_toolButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    calculateAttendance();
    loadParkingApplicationData();
    loadParkingSpaceData();
    loadmyParkingSpaceData();
}
void Owner::on_checkbutton_2_clicked()
{
    // 若有车位申请审核窗口，可在此处实例化并显示，示例如下（需补充实际类）
    Parking_Apply *w = new Parking_Apply(this);
    w->show();
}

void Owner::on_payButton_2_clicked()
{
    Owner_Pay *addOwnerInfo = new Owner_Pay();
    addOwnerInfo->show();
}

void Owner::setupTableViews()
{
    ui->tableView_3->setModel(&m_userAccountModel);
    ui->tableView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_3->resizeColumnsToContents();
    ui->carfieldapply_tableView->setModel(&m_parkingApplyModel);
    ui->carfieldapply_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->carfieldapply_tableView->resizeColumnsToContents();
    ui->My_carfiled_tableView->setModel(&m_myparkingSpaceModel);
    ui->My_carfiled_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->My_carfiled_tableView->resizeColumnsToContents();
    ui->carfieldcheck_tableView->setModel(&m_parkingSpaceModel);
    ui->carfieldcheck_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->carfieldcheck_tableView->resizeColumnsToContents(); 
}
void Owner::initTableModels()
{
    calculateAttendance();
    loadParkingApplicationData();
    loadParkingSpaceData();
    loadmyParkingSpaceData();
}

void Owner::calculateAttendance()
{
    int currentUserId = Login::currentUserId;
    if (currentUserId <= 0) {
        QMessageBox::warning(this, "未登录", "请先登录以查看缴费信息", QMessageBox::Ok);
        return;
    }
    qDebug() << Login::currentUserId;

    QTableView *tableView = ui->tableView_3;

    // 新增“缴费类型”列，调整模型列数为6
    QStandardItemModel* model = new QStandardItemModel(0, 7, this);
    QStringList headers;
    headers << "业主ID" << "需缴费用" << "待缴费用"<<"缴费方式" << "缴费状态" << "缴费时间" << "缴费类型"; // 新增“缴费类型”列头
    model->setHorizontalHeaderLabels(headers);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);

    // 三表关联查询，新增查询 payment_type 字段
    QSqlQuery query;
    QString sql = R"(
        SELECT
            pn.owner_id,
            pn.total,
            pn.need_to_pay,
            pn.method,
            pn.payment_status,
            pn.payment_date,
            pn.payment_type  -- 新增查询缴费类型字段
        FROM
            PAYMENT_NEW pn
        JOIN
            OWNER o ON pn.owner_id = o.owner_id
        JOIN
            USER_ACCOUNT ua ON o.user_id = ua.user_id
        WHERE
            ua.user_id = :currentUserId
    )";

    query.prepare(sql);
    query.bindValue(":currentUserId", currentUserId);

    if (!query.exec()) {
        QMessageBox::critical(this, "查询失败", "缴费数据查询错误：" + query.lastError().text());
        qDebug() << "SQL 错误：" << query.lastError().text();
        return;
    }

    qDebug() << "执行的 SQL：" << query.executedQuery();

    int row = 0;
    while (query.next()) {
        row++;
        qDebug() << "Row" << row << ":"
                 << "owner_id=" << query.value("owner_id").toString()
                 << "total=" << query.value("total").toString()
                 << "need_to_pay=" << query.value("need_to_pay").toString()
                 << "payment_status=" << query.value("payment_status").toString()
                 << "payment_date=" << query.value("payment_date").toString()
                 << "payment_type=" << query.value("payment_type").toString() // 打印缴费类型
                 << "user_id=" << currentUserId;

        model->insertRow(row - 1);
        model->setItem(row - 1, 0, new QStandardItem(query.value("owner_id").toString()));
        model->setItem(row - 1, 1, new QStandardItem(query.value("total").toString()));
        model->setItem(row - 1, 2, new QStandardItem(query.value("need_to_pay").toString()));
        model->setItem(row - 1, 3, new QStandardItem(query.value("method").toString()));
        model->setItem(row - 1, 4, new QStandardItem(query.value("payment_status").toString()));
        model->setItem(row - 1, 5, new QStandardItem(query.value("payment_date").toString()));
        model->setItem(row - 1, 6, new QStandardItem(query.value("payment_type").toString())); // 新增：填充缴费类型列
    }

    // 调整列宽，适应新增的“缴费类型”列
    ui->tableView_3->setColumnWidth(0, 130);
    ui->tableView_3->setColumnWidth(1, 130);
    ui->tableView_3->setColumnWidth(2, 130);
    ui->tableView_3->setColumnWidth(3, 130);
    ui->tableView_3->setColumnWidth(4, 130);
    ui->tableView_3->setColumnWidth(5, 130); // 新增列的宽度
    ui->tableView_3->setColumnWidth(6, 130);
    ui->tableView_3->verticalHeader()->setDefaultSectionSize(80);
    ui->tableView_3->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableView_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    QFont font = ui->tableView_3->font();
    font.setPointSize(12);
    ui->tableView_3->setFont(font);

    CenteredDelegate *delegate = new CenteredDelegate(ui->tableView_3);
    ui->tableView_3->setItemDelegate(delegate);
}
void Owner::loadParkingApplicationData()
{

    // 1. 校验登录态（修正 QString 错误判断）
    QString username = Login::currentUsername;
    if (username.isEmpty()) { // 用 isEmpty() 判断字符串是否为空
        QMessageBox::warning(this, "未登录", "请先登录以查看车位申请信息");
        return;
    }

    // 2. 构造 SQL 并绑定参数
    QString sql = R"(
            SELECT
                pa.application_id,
                pa.car_number,
                pa.apply_time,
                pa.status
            FROM
                PARKING_APPLICATION_NEW pa
            WHERE
                pa.applicantname = :username
        )";

    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":username", username);

    // 3. 执行查询
    if (!query.exec()) {
        QMessageBox::critical(this, "数据库错误",
                              "查询执行失败：" + query.lastError().text());
        return;
    }

    // 4. 手动填充 QSqlTableModel（或改用 QSqlQueryModel）
    m_parkingApplyModel.setQuery(std::move(query));

    // 5. 设置表头（与查询结果列顺序对应）
    m_parkingApplyModel.setHeaderData(0, Qt::Horizontal, "申请编号");
    m_parkingApplyModel.setHeaderData(1, Qt::Horizontal, "车牌号");
    m_parkingApplyModel.setHeaderData(2, Qt::Horizontal, "申请时间");
    m_parkingApplyModel.setHeaderData(3, Qt::Horizontal, "申请状态");

    // 6. 绑定模型到视图
    ui->carfieldapply_tableView->setModel(&m_parkingApplyModel);
    CenteredDelegate *delegate = new CenteredDelegate(ui->tableView_3);
    ui->carfieldapply_tableView->setItemDelegate(delegate);
    ui->carfieldapply_tableView->setColumnWidth(0, 200);
    ui->carfieldapply_tableView->setColumnWidth(1, 300);
    ui->carfieldapply_tableView->setColumnWidth(2, 200);
    ui->carfieldapply_tableView->setColumnWidth(3, 200);
    ui->carfieldapply_tableView->verticalHeader()->setDefaultSectionSize(80);
    QFont font1 = ui->carfieldapply_tableView->font();
    font1.setPointSize(12);
    ui->carfieldapply_tableView->setFont(font1);
    ui->carfieldapply_tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->carfieldapply_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 7. 检查模型错误（可选，确保数据加载正常）
    if (m_parkingApplyModel.lastError().isValid()) {
        QMessageBox::critical(this, "数据库错误",
                              "加载车位申请数据失败：" + m_parkingApplyModel.lastError().text());
    }
}
void  Owner::loadParkingSpaceData()
{
    QString sql = "SELECT space_id,location,space_status FROM PARKING_SPACE_NEW WHERE space_status='vacant'";
    m_parkingSpaceModel.setQuery(sql, db);

    // 设置表头（需与表中字段顺序对应）
    m_parkingSpaceModel.setHeaderData(0, Qt::Horizontal, "车位ID");
    m_parkingSpaceModel.setHeaderData(1, Qt::Horizontal, "车位位置");
    m_parkingSpaceModel.setHeaderData(2, Qt::Horizontal, "车位状态");
    // 绑定模型到视图
    ui->carfieldcheck_tableView->setModel(&m_parkingSpaceModel);
    CenteredDelegate *delegate = new CenteredDelegate(ui->tableView_3);
    ui->carfieldcheck_tableView->setItemDelegate(delegate);
    QFont font2 = ui->carfieldcheck_tableView->font();
    font2.setPointSize(12);
    ui->carfieldcheck_tableView->setFont(font2);
    ui->carfieldcheck_tableView->setColumnWidth(0, 200);
    ui->carfieldcheck_tableView->setColumnWidth(1, 300);
    ui->carfieldcheck_tableView->setColumnWidth(2, 200);
    ui->carfieldcheck_tableView->verticalHeader()->setDefaultSectionSize(80);
    ui->carfieldcheck_tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->carfieldcheck_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 检查查询错误
    if (m_parkingSpaceModel.lastError().isValid()) {
        QMessageBox::critical(this, "数据库错误", "加载车位申请数据失败：" + m_parkingSpaceModel.lastError().text());
    }
}
void Owner::loadmyParkingSpaceData()
{
    // 使用全局变量 currentUsername（对应 PARKING_SPACE_NEW.ownername）
    QString username = Login::currentUsername;
    qDebug() << "当前用户名：" << username;

    if (username.isEmpty()) {
        QMessageBox::warning(this, "未登录", "请先登录以查看个人车位信息");
        return;
    }

    // SQL查询：仅选择需要显示的字段（排除 ownername），但用 ownername 作为过滤条件
    QString searchQuery = QString(R"(
        SELECT
            ps.space_id,          -- 车位ID（显示）
            ps.location,          -- 车位位置（显示）
            pr.start_date,        -- 起止时间（显示）
            pr.end_date,          -- 到期时间（显示）
            ps.space_status       -- 车位状态（显示）
        FROM
            PARKING_SPACE_NEW ps
        LEFT JOIN
            PARKING_RENTAL_NEW pr ON ps.space_id = pr.space_id
        WHERE
            ps.ownername = :username  -- 用 ownername 过滤，但不查询该字段
    )");

    QSqlQuery query(db);
    query.prepare(searchQuery);
    query.bindValue(":username", username);  // 绑定全局变量作为过滤条件

    // 调试：打印绑定后的 SQL
    qDebug() << "绑定后的 SQL：" << query.executedQuery();
    if (!query.exec()) {
        qDebug() << "SQL 执行错误：" << query.lastError().text();
        return;
    }

    // 先存储查询结果数据，用于后续检查截止日期
    QList<QSqlRecord> records;
    while (query.next()) {
        records.append(query.record());
    }

    // 检查截止日期，执行自动解绑、更新状态、删除租赁记录逻辑
    QDate currentDate = QDate::currentDate();  // 获取当前日期
    for (const QSqlRecord& record : records) {
        int spaceId = record.value("space_id").toInt();
        QDate endDate = record.value("end_date").toDate();

        // 判断截止日期是否已到
        if (endDate < currentDate) {
            // 1. 更新 PARKING_SPACE_NEW 表中车位状态为 vacant
            QSqlQuery updateSpaceQuery(db);
            updateSpaceQuery.prepare("UPDATE PARKING_SPACE_NEW SET space_status = 'vacant' WHERE space_id = :spaceId AND ownername = :username");
            updateSpaceQuery.bindValue(":spaceId", spaceId);
            updateSpaceQuery.bindValue(":username", username);
            if (!updateSpaceQuery.exec()) {
                qDebug() << "更新 PARKING_SPACE_NEW 状态失败：" << updateSpaceQuery.lastError().text();
            } else {
                qDebug() << "车位ID " << spaceId << " 状态已更新为 vacant";
            }

            // 2. 删除 PARKING_RENTAL_NEW 表中对应的租赁记录
            QSqlQuery deleteRentalQuery(db);
            deleteRentalQuery.prepare("DELETE FROM PARKING_RENTAL_NEW WHERE space_id = :spaceId");
            deleteRentalQuery.bindValue(":spaceId", spaceId);
            if (!deleteRentalQuery.exec()) {
                qDebug() << "删除 PARKING_RENTAL_NEW 记录失败：" << deleteRentalQuery.lastError().text();
            } else {
                qDebug() << "车位ID " << spaceId << " 对应的租赁记录已删除";
            }
        }
    }

    // 重新执行查询并填充模型（因为数据可能已更新，需要刷新显示）
    if (!query.exec()) {
        qDebug() << "重新执行 SQL 错误：" << query.lastError().text();
        return;
    }
    m_myparkingSpaceModel.setQuery(std::move(query));
    // 设置表头（顺序与 SELECT 字段顺序一致）
    m_myparkingSpaceModel.setHeaderData(0, Qt::Horizontal, "车位ID");
    m_myparkingSpaceModel.setHeaderData(1, Qt::Horizontal, "车位位置");
    m_myparkingSpaceModel.setHeaderData(2, Qt::Horizontal, "起止时间");
    m_myparkingSpaceModel.setHeaderData(3, Qt::Horizontal, "到期时间");
    m_myparkingSpaceModel.setHeaderData(4, Qt::Horizontal, "车位状态");
    ui->My_carfiled_tableView->setModel(&m_myparkingSpaceModel);
    ui->My_carfiled_tableView->setColumnWidth(0, 200);
    ui->My_carfiled_tableView->setColumnWidth(1, 300);
    ui->My_carfiled_tableView->setColumnWidth(2, 200);
    ui->My_carfiled_tableView->setColumnWidth(3, 200);
    ui->My_carfiled_tableView->setColumnWidth(4, 200);
    ui->My_carfiled_tableView->verticalHeader()->setDefaultSectionSize(80);
    ui->My_carfiled_tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->My_carfiled_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    QFont font2 = ui->My_carfiled_tableView->font();
    font2.setPointSize(12);
    ui->My_carfiled_tableView->setFont(font2);
    CenteredDelegate *delegate = new CenteredDelegate(ui->tableView_3);
    ui->My_carfiled_tableView->setItemDelegate(delegate);

    // 检查模型错误
    if (m_myparkingSpaceModel.lastError().isValid()) {
        qDebug() << "模型错误：" << m_myparkingSpaceModel.lastError().text();
    }
}
void Owner::on_pushButton_clicked()
{
    // 1. 校验用户是否登录（currentUserId 为整型，0 表示未登录）
    if (Login::currentUserId == 0) {
        QMessageBox::warning(this, "用户未登录", "请先登录账号", QMessageBox::Ok);
        return;
    }

    // 2. 获取故障描述信息
    QString description = ui->textEdit->toPlainText();
    if (description.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入故障描述", QMessageBox::Ok);
        ui->textEdit->setFocus();
        return;
    }

    // 3. 通过 currentUserId 查询关联的 owner_id（从 OWNER 表）
    QSqlQuery ownerQuery;
    ownerQuery.prepare("SELECT owner_id FROM OWNER WHERE user_id = :user_id");
    ownerQuery.bindValue(":user_id", Login::currentUserId);
    if (!ownerQuery.exec() || !ownerQuery.next()) {
        QMessageBox::warning(this, "查询失败", "未找到对应的业主信息，请确认账号关联", QMessageBox::Ok);
        qDebug() << "OWNER 表查询失败:" << ownerQuery.lastError().text();
        return;
    }
    int owner_id = ownerQuery.value(0).toInt();
    qDebug() << "找到对应的 owner_id:" << owner_id;

    // 4. 检查该业主是否有房产记录（关键新增校验）
    QSqlQuery propertyCountQuery;
    propertyCountQuery.prepare("SELECT COUNT(*) FROM PROPERTY_NEW WHERE owner_id = :owner_id");
    propertyCountQuery.bindValue(":owner_id", owner_id);
    if (!propertyCountQuery.exec() || !propertyCountQuery.next()) {
        QMessageBox::warning(this, "查询失败", "无法验证房产信息，请联系管理员", QMessageBox::Ok);
        qDebug() << "验证房产数量失败:" << propertyCountQuery.lastError().text();
        return;
    }
    int propertyCount = propertyCountQuery.value(0).toInt();
    if (propertyCount == 0) {
        QMessageBox::warning(this, "权限不足", "您没有关联的房产，无法提交报修申请", QMessageBox::Ok);
        return;
    }

    // 5. 通过 owner_id 查询关联的 property_id（从 PROPERTY_NEW 表）
    // 注意：如果一个业主有多套房产，这里默认取第一条记录（可根据业务需求调整）
    QSqlQuery propertyQuery;
    propertyQuery.prepare("SELECT property_id FROM PROPERTY_NEW WHERE owner_id = :owner_id LIMIT 1");
    propertyQuery.bindValue(":owner_id", owner_id);
    if (!propertyQuery.exec() || !propertyQuery.next()) {
        QMessageBox::warning(this, "查询失败", "未找到对应的房产信息，请确认业主关联", QMessageBox::Ok);
        qDebug() << "PROPERTY_NEW 表查询失败:" << propertyQuery.lastError().text();
        return;
    }
    int property_id = propertyQuery.value(0).toInt();
    qDebug() << "找到对应的 property_id:" << property_id;

    // 6. 插入维修信息到 REPAIR 表
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO REPAIR "
                        "(owner_id, description, report_time, property_id, status) "
                        "VALUES (:owner_id, :description, CURRENT_TIMESTAMP, :property_id, :status)");
    insertQuery.bindValue(":owner_id", owner_id);
    insertQuery.bindValue(":description", description);
    insertQuery.bindValue(":property_id", property_id);
    insertQuery.bindValue(":status", "reported");

    if (insertQuery.exec()) {
        QMessageBox::information(this, "提交成功", "报修申请已提交，我们将尽快处理", QMessageBox::Ok);
        ui->textEdit->clear();
    } else {
        QMessageBox::critical(this, "数据库错误", "提交失败: " + insertQuery.lastError().text(), QMessageBox::Ok);
        qDebug() << "插入 REPAIR 表失败:" << insertQuery.lastError().text();
    }
}

void Owner::on_pushButton_3_clicked()
{
    QTableView *tableView = ui->tableView;
    QModelIndexList selectedRows = tableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "操作提示", "请先选择一条维修记录", QMessageBox::Ok);
        return;
    }

    // 获取选中行的维修ID（假设维修ID在第0列）
    QModelIndex repairIdIndex = selectedRows.first().siblingAtColumn(0);
    int repairId = repairIdIndex.data().toInt();

    // 打开评价界面，传入维修ID
    Owner_Evaluation *evalWidget = new Owner_Evaluation(repairId, this);
    // 连接评价界面的信号，实现评价提交后刷新表格（可选，若需要实时刷新）
    connect(evalWidget, &Owner_Evaluation::evaluationSubmitted, this, [this]() {
        // 可在此重新调用 on_pushButton_4_clicked() 刷新表格，或其他刷新逻辑
        on_tabWidget_3_tabBarClicked(2);
    });
    evalWidget->resize(800, 600);
    evalWidget->show();
}

void Owner::on_pushButton_6_clicked()
{

    loadParkingApplicationData();
    loadParkingSpaceData();
    loadmyParkingSpaceData();
}

void Owner::on_tabWidget_3_tabBarClicked(int index)
{
    if(index==2)
    {
        // 获取当前登录用户ID（全局变量）
        int currentUserId = Login::currentUserId;
        if (currentUserId <= 0) {
            QMessageBox::warning(this, "未登录", "请先登录以查看维修记录", QMessageBox::Ok);
            return;
        }
        qDebug()<<Login::currentUserId;

        // 初始化表格（使用 tableView_3 ）
        QTableView *tableView = ui->tableView;

        // 选择一种模型方式：这里选择使用 QStandardItemModel
        QStandardItemModel* model = new QStandardItemModel(0, 5, this);
        QStringList headers;
        headers << "维修ID" << "维修详情" << "申请时间" << "维修状态" << "维修评价";
        model->setHorizontalHeaderLabels(headers);

        // 设置表格属性
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);    // 整行选择
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);   // 仅允许选中一行
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);     // 不可编辑
        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tableView->setModel(model);

        // 两表关联查询：REPAIR → OWNER
        QSqlQuery query;
        QString sql = R"(
            SELECT
                r.repair_id,
                r.description,
                r.report_time,
                r.status,
                r.evaluation,
                r.owner_id
            FROM
                REPAIR r
            JOIN
                OWNER o ON r.owner_id = o.owner_id  -- 直接关联 REPAIR 和 OWNER 表
            WHERE
                o.user_id = :currentUserId           -- 通过 OWNER.user_id 筛选当前用户
        )";

        query.prepare(sql);
        query.bindValue(":currentUserId", currentUserId);

        if (!query.exec()) {
            QMessageBox::critical(this, "查询失败", "查询维修记录失败：" + query.lastError().text());
            qDebug() << "SQL 错误：" << query.lastError().text();
            return;
        }

        // 打印实际执行的 SQL（含绑定参数）
        qDebug() << "执行的 SQL：" << query.executedQuery();

        // 验证查询结果并填充模型
        int row = 0;
        while (query.next()) {
            row++;
            // 打印关键字段值，确认是否符合预期
            qDebug() << "Row" << row << ":"
                     << "repair_id=" << query.value("repair_id").toString()
                     << "owner_id=" << query.value("owner_id").toString()
                     << "user_id=" << currentUserId;

            // 填充数据到模型
            model->insertRow(row-1);  // 注意：row 从 1 开始，而 insertRow 索引从 0 开始
            model->setItem(row-1, 0, new QStandardItem(query.value("repair_id").toString()));
            model->setItem(row-1, 1, new QStandardItem(query.value("description").toString()));
            model->setItem(row-1, 2, new QStandardItem(query.value("report_time").toString()));
            model->setItem(row-1, 3, new QStandardItem(query.value("status").toString()));
            model->setItem(row-1, 4, new QStandardItem(query.value("evaluation").toString()));
        }
        ui->tableView->setColumnWidth(0, 200);
        ui->tableView->setColumnWidth(1, 300);
        ui->tableView->setColumnWidth(2, 200);
        ui->tableView->setColumnWidth(3, 200);
        ui->tableView->setColumnWidth(4, 200);
        ui->tableView->verticalHeader()->setDefaultSectionSize(80);
        ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        QFont font2 = ui->tableView->font();
        font2.setPointSize(12);
        ui->tableView->setFont(font2);
        CenteredDelegate *delegate = new CenteredDelegate(ui->tableView);
        ui->tableView->setItemDelegate(delegate);

    }
}
void Owner::on_toolButton_clicked()
{
    this->close();
    if (!loginWindow) {
        loginWindow = new Login();  // 可以安全使用 Login 类
    }
    loginWindow->show();
    Login::currentUserId = -1;
    Login::currentUsername = "";
}
void Owner::on_toolButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    QTableView *tableView = ui->tableView_2;
    QStandardItemModel* model = new QStandardItemModel(0, 3, this);
    QStringList headers;
    headers << "公告ID" << "公告内容" << "发布时间";
    model->setHorizontalHeaderLabels(headers);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setModel(model);
    //查询数据库
    QSqlQuery query;
    query.prepare("SELECT announcement_id,content, release_time FROM ANNOUNCEMENT");
    if (!query.exec()) {
        QMessageBox::critical(this, "查询失败", "查询公告信息失败：" + query.lastError().text());
        return;
    }
    int row = 0;
    while (query.next()) {
        model->insertRow(row);
        model->setData(model->index(row, 0), query.value("announcement_id").toInt());
        model->setData(model->index(row, 1), query.value("content").toString());
        model->setData(model->index(row, 2), query.value("release_time").toString());
        row++;
    }
    model->setRowCount(row);
    model->setColumnCount(3); // 修正为3列，与表头匹配
    //把tableView除了第一行外的行高设置为100px
    tableView->setModel(model);
    tableView->verticalHeader()->setDefaultSectionSize(100);
    CenteredDelegate *delegate = new CenteredDelegate(ui->tableView_2);
    ui->tableView_2->setItemDelegate(delegate);
    // 设置表格不可编辑
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    //当点击某一个公告时，QmessageBox显示公告内容，可以反复点击查看
    connect(tableView->selectionModel(), &QItemSelectionModel::currentChanged, this, [this, tableView]() {
        QModelIndex currentIndex = tableView->currentIndex();
        if (currentIndex.isValid()) {
            QString content = tableView->model()->data(currentIndex.siblingAtColumn(1)).toString();
            QMessageBox::information(this, "公告内容", content);
        }
    });


}



