#include "property_manager.h"
#include "ui_property_manager.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include "login.h"
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>
#include"CenteredDelegate1.h"
Property_Manager::Property_Manager(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::Property_Manager),
    loginWindow(nullptr),
    m_model1(this, "qt_sql_default_connection"),
    m_userModel(this, "qt_sql_default_connection"),
    leaveModel(new LeaveSqlModel(this, "qt_sql_default_connection")),
    feeModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    // 初始化按钮状态
    ui->pushButton_EditFinish->setVisible(false);
    ui->pushButton_17->setVisible(false);
    ui->stackedWidget->setCurrentIndex(0);
    resize(800, 1000);
    // 状态栏初始化
    QStatusBar *statusBar = this->statusBar();
    statusLabel = new QLabel("您当前的身份为：物业管理员", this);
    statusBar->addWidget(statusLabel);

    // 初始化数据库和界面
    initDatabase();
    initTableModels();
    setupTableViews();
    // 信号关联（重点：确保用户删除后触发刷新）
    connect(ui->pushButton_5, &QPushButton::clicked, this, &Property_Manager::on_pushButton_5_clicked);
    connect(ui->pushButton_16, &QPushButton::clicked, this, &Property_Manager::on_pushButton_16_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Property_Manager::on_pushButton_2_clicked);
    connect(ui->pushButton_EditFinish, &QPushButton::clicked, this, &Property_Manager::on_pushButton_EditFinish_clicked);
    connect(ui->pushButton_10, &QPushButton::clicked, this, &Property_Manager::on_pushButton_10_clicked);
    connect(ui->pushButton_17, &QPushButton::clicked, this, &Property_Manager::on_pushButton_17_clicked);
    connect(ui->pushButton_14, &QPushButton::clicked, this, &Property_Manager::on_pushButton_14_clicked);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &Property_Manager::on_pushButton_9_clicked);


}

Property_Manager::~Property_Manager()
{
    if (db.isOpen()) db.close();
    delete ui;
    delete leaveModel;
}

// 数据库初始化
void Property_Manager::initDatabase()
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
    qDebug() << "Database Name:" << db.databaseName();
    qDebug() << "Database Open:" << db.isOpen();
    qDebug() << "Tables in DB:" << db.tables();

}

// 表格模型初始化
void Property_Manager::initTableModels()
{
    calculateAttendance(); // 员工表初始化
    loadUserAccountData();
    loadStaffLeaveData();
    setupChartAndTable();
}

// 表格视图设置
void Property_Manager::setupTableViews()
{
    // 员工表设置
    ui->tableView->setModel(&m_model1);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->resizeColumnsToContents();
    CenteredDelegate1 *delegate1 = new CenteredDelegate1(ui->tableView);
    ui->tableView->setItemDelegate(delegate1);
    QFont font = ui->tableView->font();
    font.setPointSize(12);
    ui->tableView->setFont(font);
    ui->tableView->setColumnWidth(0, 200);
    ui->tableView->setColumnWidth(1, 300);
    ui->tableView->setColumnWidth(2, 200);
    ui->tableView->setColumnWidth(3, 200);
    ui->tableView->setColumnWidth(4, 200);
    ui->tableView->setColumnWidth(5, 200);
    ui->tableView->verticalHeader()->setDefaultSectionSize(80);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 用户表设置
    ui->tableView_4->setModel(&m_userModel);
    ui->tableView_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_4->resizeColumnsToContents();
    CenteredDelegate1 *delegate = new CenteredDelegate1(ui->tableView_4);
    ui->tableView_4->setItemDelegate(delegate);
    QFont font1 = ui->tableView_4->font();
    font1.setPointSize(12);
    ui->tableView_4->setFont(font1);
    ui->tableView_4->setColumnWidth(0, 100);
    ui->tableView_4->setColumnWidth(1, 290);
    ui->tableView_4->setColumnWidth(2, 230);
    ui->tableView_4->setColumnWidth(3, 250);
    ui->tableView_4->setColumnWidth(4, 350);
    ui->tableView_4->setColumnWidth(5, 350);
    ui->tableView_4->setColumnWidth(6, 350);
    ui->tableView_4->setColumnWidth(7, 230);
    ui->tableView_4->verticalHeader()->setDefaultSectionSize(80);
    ui->tableView_4->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableView_4->setSelectionBehavior(QAbstractItemView::SelectRows);
    //请假表设置
    ui->tableView_3->setModel(leaveModel);
    ui->tableView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_3->resizeColumnsToContents();

    // 文字居中委托
    CenteredDelegate1 *delegate2 = new CenteredDelegate1(ui->tableView_3);
    ui->tableView_3->setItemDelegate(delegate2);

    // 设置字体
    QFont font2 = ui->tableView_3->font();
    font2.setPointSize(12);
    ui->tableView_3->setFont(font2);

    // 设置列宽（均为 350）
    for (int i = 0; i < leaveModel->columnCount(); ++i) {
        ui->tableView_3->setColumnWidth(i, 350);
    }

    // 行高、对齐方式
    ui->tableView_3->verticalHeader()->setDefaultSectionSize(80);
    ui->tableView_3->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableView_3->setSelectionBehavior(QAbstractItemView::SelectRows);
}

// 员工表数据加载（带出勤统计）
void Property_Manager::calculateAttendance()
{
    QString sql = R"(SELECT s.staff_id,s.user_id,s.position, s.department, s.admin_id,
                  COUNT(a.status) AS attendance_count
                  FROM staff s
                  LEFT JOIN attendance a
                  ON s.staff_id = a.staff_id
                  AND a.status = 'normal'
                  GROUP BY
                  s.staff_id,
                  s.user_id,
                  s.position,
                  s.department,
                  s.admin_id
                  ORDER BY s.staff_id ASC;
                  )";
    m_model1.setQuery(sql, db);
    m_model1.setHeaderData(0, Qt::Horizontal, "员工ID");
    m_model1.setHeaderData(1, Qt::Horizontal, "用户ID");
    m_model1.setHeaderData(2, Qt::Horizontal, "职位");
    m_model1.setHeaderData(3, Qt::Horizontal, "部门");
    m_model1.setHeaderData(4, Qt::Horizontal, "管理员ID");
    m_model1.setHeaderData(5, Qt::Horizontal, "出勤次数");
}

// 用户表数据加载（统一使用m_userModel）
void Property_Manager::loadUserAccountData()
{
    QString sql = "SELECT user_id, username, password, phone, email, register_time, last_login, user_type FROM USER_ACCOUNT";
    m_userModel.setQuery(sql, db);
    m_userModel.setHeaderData(0, Qt::Horizontal, "用户ID");
    m_userModel.setHeaderData(1, Qt::Horizontal, "用户名");
    m_userModel.setHeaderData(2, Qt::Horizontal, "密码");
    m_userModel.setHeaderData(3, Qt::Horizontal, "电话");
    m_userModel.setHeaderData(4, Qt::Horizontal, "邮箱");
    m_userModel.setHeaderData(5, Qt::Horizontal, "注册时间");
    m_userModel.setHeaderData(6, Qt::Horizontal, "最后登录时间");
    m_userModel.setHeaderData(7, Qt::Horizontal, "身份权限");

    if (m_userModel.lastError().isValid()) {
        QMessageBox::critical(this, "数据库错误", "加载用户数据失败：" + m_userModel.lastError().text());
    }
}
void Property_Manager::loadStaffLeaveData()
{
    QString sql = "SELECT staff_id, type, start_date, end_date, reason, status, actual_end_date "
                  "FROM LEAVE";
    leaveModel->setQuery(sql, db);

    // 设置表头（与需求对应）
    leaveModel->setHeaderData(0, Qt::Horizontal, "员工ID");
    leaveModel->setHeaderData(1, Qt::Horizontal, "请假类型");
    leaveModel->setHeaderData(2, Qt::Horizontal, "开始时间");
    leaveModel->setHeaderData(3, Qt::Horizontal, "结束时间");
    leaveModel->setHeaderData(4, Qt::Horizontal, "请假原因");
    leaveModel->setHeaderData(5, Qt::Horizontal, "审批状态");
    leaveModel->setHeaderData(6, Qt::Horizontal, "实际销假时间");

    if (leaveModel->lastError().isValid()) {
        QMessageBox::critical(this, "数据库错误", "加载请假数据失败：" + leaveModel->lastError().text());
    }
}

// 界面切换槽函数（保持原有代码不变）
void Property_Manager::on_toolButton_5_clicked()
{ ui->stackedWidget->setCurrentIndex(5); }

void Property_Manager::on_toolButton_10_clicked()
{ ui->stackedWidget->setCurrentIndex(1); }

void Property_Manager::on_toolButton_11_clicked()
{ ui->stackedWidget->setCurrentIndex(3); }
void Property_Manager::on_toolButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    setupChartAndTable();

}
void Property_Manager::on_toolButton_15_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);

    QTableView *tableView = ui->tableView_5;
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
    // 设置表格不可编辑
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_5->setSelectionBehavior(QAbstractItemView::SelectRows);
    CenteredDelegate1 *delegate2 = new CenteredDelegate1(ui->tableView_5);
    ui->tableView_5->setItemDelegate(delegate2);
    //当点击某一个公告时，QmessageBox显示公告内容，可以反复点击查看
    connect(tableView->selectionModel(), &QItemSelectionModel::currentChanged, this, [this, tableView]() {
        QModelIndex currentIndex = tableView->currentIndex();
        if (currentIndex.isValid()) {
            QString content = tableView->model()->data(currentIndex.siblingAtColumn(1)).toString();
            QMessageBox::information(this, "公告内容", content);
        }
    });


}

// 员工添加后刷新
void Property_Manager::handleEmployeeAdded()
{
    calculateAttendance();
    loadUserAccountData(); // 同时刷新用户表，确保关联数据更新
}

// 员工删除后刷新
void Property_Manager::handleStaffDeleted()
{
    calculateAttendance();
    loadUserAccountData();
}

// 用户删除后刷新（使用与员工相同的刷新逻辑）
void Property_Manager::handleUserDeleted()
{
    loadUserAccountData(); // 直接调用统一的加载函数
}

// 员工添加后刷新
void Property_Manager::handleStaffAdded()
{
    calculateAttendance();
    loadUserAccountData();
}

// 对话框关闭后指针清理（保持原有代码不变）
void Property_Manager::on_proper_manageradd_closed()
{ proper_manageradd = nullptr; }

void Property_Manager::on_property_staffadd_closed()
{ property_staffadd = nullptr; }

void Property_Manager::on_property_staffdel_closed()
{ property_staffdel = nullptr; }

void Property_Manager::on_property_managerdel_closed()
{ property_managerdel = nullptr; }

// 用户添加按钮（修改：确保添加后正确触发刷新）
void Property_Manager::on_pushButton_6_clicked()
{
    if (!proper_manageradd) {
        proper_manageradd = new Proper_Manageradd(this, &db);
        connect(proper_manageradd, &Proper_Manageradd::employeeAdded,
                this, &Property_Manager::handleEmployeeAdded);
        connect(proper_manageradd, &Proper_Manageradd::destroyed,
                this, &Property_Manager::on_proper_manageradd_closed);
    }
    proper_manageradd->show();
    proper_manageradd->raise();
    proper_manageradd->activateWindow();
}

// 员工添加按钮（保持原有代码不变）
void Property_Manager::on_pushButton_clicked()
{
    if (!property_staffadd) {
        property_staffadd = new Property_Staffadd(this, &db);
        connect(property_staffadd, &Property_Staffadd::staffAdded,
                this, &Property_Manager::handleStaffAdded);
        connect(property_staffadd, &Property_Staffadd::destroyed,
                this, &Property_Manager::on_property_staffadd_closed);
    }
    property_staffadd->show();
}

// 员工删除按钮（保持原有代码不变）
void Property_Manager::on_pushButton_3_clicked()
{
    if (!property_staffdel) {
        property_staffdel = new Property_Staffdel(this, &db);
        connect(property_staffdel, &Property_Staffdel::staffDeleted,
                this, &Property_Manager::handleStaffDeleted);
        connect(property_staffdel, &Property_Staffdel::destroyed,
                this, &Property_Manager::on_property_staffdel_closed);
    }
    property_staffdel->show();
}

// 用户删除按钮（保持原有代码不变）
void Property_Manager::on_pushButton_11_clicked()
{
    if (!property_managerdel) {
        property_managerdel = new Property_Managerdel(this, &db);
        connect(property_managerdel, &Property_Managerdel::userDeleted,
                this, &Property_Manager::handleUserDeleted);
        connect(property_managerdel, &Property_Managerdel::destroyed,
                this, &Property_Manager::on_property_managerdel_closed);
    }
    property_managerdel->show();
}

// 用户搜索功能（修改：使用m_userModel并完善逻辑）
void Property_Manager::on_pushButton_5_clicked()
{
    QString searchText = ui->lineEdit_4->text().trimmed();
    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "搜索提示", "请输入要搜索的用户ID");
        return;
    }

    QString searchQuery = QString(
                              "SELECT user_id, username, password, phone, email, register_time, last_login, user_type "
                              "FROM USER_ACCOUNT WHERE user_id = '%1'").arg(searchText);

    m_userModel.setQuery(searchQuery, db); // 使用统一的m_userModel

    if (m_userModel.lastError().isValid()) {
        QMessageBox::critical(this, "数据库错误", "搜索失败：" + m_userModel.lastError().text());
        loadUserAccountData(); // 出错时恢复原始数据
        return;
    }

    if (m_userModel.rowCount() == 0) {
        QMessageBox::information(this, "搜索结果", "未查询到数据");
        loadUserAccountData(); // 无结果时恢复原始数据
    }
}

// 用户搜索重置（修改：使用m_userModel）
void Property_Manager::on_pushButton_16_clicked()
{
    loadUserAccountData(); // 直接调用统一的加载函数
    ui->lineEdit_4->clear();
}

// 员工搜索功能（保持原有代码不变）
void Property_Manager::on_pushButton_12_clicked()
{
    QString searchText1 = ui->lineEdit->text().trimmed();
    if (searchText1.isEmpty()) {
        QMessageBox::warning(this, "搜索提示", "请输入要搜索的员工ID");
        return;
    }

    QString searchQuery1 = QString(R"(
        SELECT s.staff_id AS '员工ID',
               s.user_id AS '用户ID',
               s.position AS '职位',
               s.department AS '部门',
               s.admin_id AS '管理员ID',
               COUNT(a.status) AS '出勤次数'
        FROM staff s
        LEFT JOIN attendance a ON s.staff_id = a.staff_id AND a.status = 'normal'
        WHERE s.staff_id = %1
        GROUP BY s.staff_id, s.user_id, s.position, s.department, s.admin_id
    )").arg(searchText1);

    m_model1.setQuery(searchQuery1, db);

    if (m_model1.lastError().isValid()) {
        QMessageBox::critical(this, "数据库错误", "搜索失败：" + m_model1.lastError().text());
        calculateAttendance();
        return;
    }

    if (m_model1.rowCount() == 0) {
        QMessageBox::information(this, "搜索结果", "未查询到数据");
        calculateAttendance();
    }
}

// 员工搜索重置（保持原有代码不变）
void Property_Manager::on_pushButton_13_clicked()
{
    calculateAttendance();
    ui->lineEdit->clear();
}


// 员工修改功能
void Property_Manager::on_pushButton_2_clicked()
{
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) return;

    if (!isEmployeeEditing) {
        ui->tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
        QModelIndex currentIndex = ui->tableView->currentIndex();
        if (currentIndex.isValid()) ui->tableView->selectRow(currentIndex.row());
        isEmployeeEditing = true;
        ui->pushButton_2->setVisible(false);
        ui->pushButton_EditFinish->setVisible(true);
    }
}

// 员工修改完成
void Property_Manager::on_pushButton_EditFinish_clicked()
{
    if (isEmployeeEditing) {
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        calculateAttendance();
        QMessageBox::information(this, "提示", "编辑已完成");
        isEmployeeEditing = false;
        ui->pushButton_2->setVisible(true);
        ui->pushButton_EditFinish->setVisible(false);
    }
}

// 用户修改功能
void Property_Manager::on_pushButton_10_clicked()
{
    QModelIndexList selectedRows = ui->tableView_4->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) return;

    if (!isUserEditing) {
        ui->tableView_4->setEditTriggers(QAbstractItemView::AllEditTriggers);
        QModelIndex currentIndex = ui->tableView_4->currentIndex();
        if (currentIndex.isValid()) ui->tableView_4->selectRow(currentIndex.row());
        isUserEditing = true;
        ui->pushButton_10->setVisible(false);
        ui->pushButton_17->setVisible(true);
    }
}

// 用户修改完成
void Property_Manager::on_pushButton_17_clicked()
{
    if (isUserEditing) {
        ui->tableView_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
        QString userSql = "SELECT user_id, username, password, phone, email, register_time, last_login, user_type FROM USER_ACCOUNT";
        m_userModel.setQuery(userSql, db);
        QMessageBox::information(this, "提示", "编辑已完成");
        isUserEditing = false;
        ui->pushButton_10->setVisible(true);
        ui->pushButton_17->setVisible(false);
    }
}

// 状态栏更新
void Property_Manager::updateStatus(const QString &status)
{
    if (statusLabel) statusLabel->setText(status);
}

void Property_Manager::on_pushButton_14_clicked()
{
    QString searchText = ui->lineEdit_3->text().trimmed();
    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "搜索提示", "请输入要搜索的员工ID");
        return;
    }

    QString searchQuery = QString("SELECT staff_id, type, start_date, end_date, reason, status, actual_end_date "
                                  "FROM LEAVE "
                                  "WHERE staff_id = '%1'")
                              .arg(searchText);
    leaveModel->setQuery(searchQuery, db);
    if (leaveModel->lastError().isValid()) {
        QMessageBox::critical(this, "数据库错误", "搜索失败：" + leaveModel->lastError().text());
        loadStaffLeaveData(); // 出错时恢复原始数据
        return;
    }

    if (leaveModel->rowCount() == 0) {
        QMessageBox::information(this, "搜索结果", "未查询到数据");
        loadStaffLeaveData(); // 无结果时恢复原始数据
    }
}

void Property_Manager::on_pushButton_7_clicked()
{
    QModelIndexList selectedRows = ui->tableView_3->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "提示", "请选择要操作的请假记录");
        return;
    }

    // 【关键】先保存选中行的关键信息，避免刷新后丢失
    QModelIndex index = selectedRows.first();
    QString staffId = leaveModel->data(leaveModel->index(index.row(), 0)).toString();

    QSqlQuery query(db);
    query.prepare("UPDATE LEAVE SET status = 'approved' WHERE staff_id = :staffId");
    query.bindValue(":staffId", staffId);

    if (query.exec()) {
        QMessageBox::information(this, "成功", "同意请假操作完成");
        // 刷新表格
        loadStaffLeaveData();
    } else {
        QMessageBox::critical(this, "数据库错误", "同意请假失败：" + query.lastError().text());
        loadStaffLeaveData();
    }
}

void Property_Manager::on_pushButton_8_clicked()
{
    QModelIndexList selectedRows = ui->tableView_3->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "提示", "请选择要操作的请假记录");
        return;
    }

    QModelIndex index = selectedRows.first();
    QString staffId = leaveModel->data(leaveModel->index(index.row(), 0)).toString();

    QSqlQuery query(db);
    query.prepare("UPDATE LEAVE SET status = 'rejected' WHERE staff_id = :staffId");
    query.bindValue(":staffId", staffId);

    if (query.exec()) {
        QMessageBox::information(this, "成功", "驳回请假操作完成");
        loadStaffLeaveData();

    } else {
        QMessageBox::critical(this, "数据库错误", "驳回请假失败：" + query.lastError().text());
        loadStaffLeaveData();
    }
}

void Property_Manager::on_pushButton_9_clicked()
{
    loadStaffLeaveData();
}

void Property_Manager::on_toolButton_clicked()
{
    this->close();
    if (!loginWindow) {
        loginWindow = new Login();  // 可以安全使用 Login 类
    }
    loginWindow->show();
    Login::currentUserId = -1;
    Login::currentUsername="";
}

void Property_Manager::on_pushButton_18_clicked()
{
    announceDialog = new announce1(this);
    announceDialog->show();
}

void Property_Manager::setupChartAndTable() {
    qDebug() << "开始初始化数据统计图表和表格";

    // ========== 1. 从数据库查询数据 ==========
    // 物业费总和
    double propertySum = 0.0;
    QSqlQuery propQuery(db);
    propQuery.prepare("SELECT SUM(total) FROM PAYMENT_NEW WHERE payment_type = 'property_fee'");
    if (propQuery.exec() && propQuery.next()) {
        propertySum = propQuery.value(0).toDouble();
        qDebug() << "物业费查询成功，总和:" << propertySum;
    } else {
        qCritical() << "物业费查询错误:" << propQuery.lastError().text();
        QMessageBox::critical(this, "数据查询失败", "无法获取物业费数据:\n" + propQuery.lastError().text());
        return;
    }

    // 车位租金总和
    double carSum = 0.0;
    QSqlQuery carQuery(db);
    carQuery.prepare("SELECT SUM(total) FROM PAYMENT_NEW WHERE payment_type = 'carspace_fee'");
    if (carQuery.exec() && carQuery.next()) {
        carSum = carQuery.value(0).toDouble();
        qDebug() << "车位租金查询成功，总和:" << carSum;
    } else {
        qCritical() << "车位租金查询错误:" << carQuery.lastError().text();
        QMessageBox::critical(this, "数据查询失败", "无法获取车位租金数据:\n" + carQuery.lastError().text());
        return;
    }

    // ========== 2. 设置费用表格 ==========
    if (!feeModel) {
        qCritical() << "feeModel未初始化!";
        feeModel = new QSqlQueryModel(this); // 确保feeModel已初始化
    }
    // 使用更安全的SQL查询方式
    QString sql = QString("SELECT '物业费' AS 费用类型, %1 AS 总收入 "
                          "UNION ALL "
                          "SELECT '车位租金' AS 费用类型, %2 AS 总收入")
                      .arg(propertySum).arg(carSum);

    feeModel->setQuery(sql);
    if (feeModel->lastError().isValid()) {
        qCritical() << "费用统计SQL执行失败:" << feeModel->lastError().text();
        QMessageBox::critical(this, "数据加载失败", "无法加载费用统计表格:\n" + feeModel->lastError().text());
        return;
    }

    // 设置表格样式
    QTableView *tableView = ui->tableView_2;
    tableView->setModel(feeModel);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    tableView->verticalHeader()->setDefaultSectionSize(50); // 减小行高，提高可用性
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 设置表格字体
    QFont tableFont = tableView->font();
    tableFont.setPointSize(12);
    tableView->setFont(tableFont);

    // 设置表格居中显示
    CenteredDelegate1 *delegate = new CenteredDelegate1(tableView);
    tableView->setItemDelegate(delegate);

    qDebug() << "费用统计表格设置完成";

    // ========== 3. 创建柱状图 ==========
    // 1) 准备数据系列
    QBarSet *propSet = new QBarSet("物业费");
    QBarSet *carSet = new QBarSet("车位租金");
    *propSet << propertySum;
    *carSet << carSum;

    QBarSeries *series = new QBarSeries();
    series->append(propSet);
    series->append(carSet);
    series->setBarWidth(0.1); // 调整为更合理的宽度

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("收入统计（物业费 vs 车位租金）");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // 设置图表字体
    QFont chartTitleFont = chart->titleFont();
    chartTitleFont.setPointSize(14);
    chart->setTitleFont(chartTitleFont);

    // ========== 4. 设置坐标轴 ==========
    // X 轴（分类轴）
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append("费用类型");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Y 轴（数值轴）
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("收入（元）");
    axisY->setLabelFormat("%.2f");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    qDebug() << "柱状图创建完成";

    // ========== 5. 添加到界面 ==========
    // 获取 StackedWidget 中索引为 2 的页面
    QWidget *chartPage = ui->stackedWidget->widget(2);
    if (!chartPage) {
        qCritical() << "StackedWidget 中索引 2 的页面不存在!";
        QMessageBox::critical(this, "界面错误", "无法加载数据统计页面");
        return;
    }

    // 安全清理旧布局
    QLayout *oldLayout = chartPage->layout();
    if (oldLayout) {
        // 保存表格控件，避免删除
        QWidget *tableWidget = ui->tableView_2;

        // 移除并删除其他所有控件
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0))) {
            if (item->widget() && item->widget() != tableWidget) {
                delete item->widget();
            }
            delete item;
        }
        delete oldLayout;
    }

    // 创建图表视图
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(300); // 设置最小高度，避免压缩

    // 添加新布局（图表 + 表格）
    QVBoxLayout *newLayout = new QVBoxLayout(chartPage);
    newLayout->addWidget(chartView);
    newLayout->addWidget(tableView);
    newLayout->setStretch(0, 3); // 图表占3份
    newLayout->setStretch(1, 1); // 表格占1份
    chartPage->setLayout(newLayout);

}
void Property_Manager::on_pushButton_19_clicked()
{
    // 获取当前选中的行
    QModelIndexList selectedRows = ui->tableView_5->selectionModel()->selectedRows();

    // 检查是否有选中的行
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "操作提示", "请先选择要删除的公告");
        return;
    }

    // 确认删除操作（防止误删）
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", "确定要删除选中的公告吗？此操作不可恢复！",QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        return;
    }
    // 开启事务处理（确保数据库操作的原子性）
    db.transaction();

    // 遍历所有选中的行（支持多行删除）
    for (const QModelIndex &index : selectedRows) {
        int row = index.row();

        // 获取公告ID（假设公告ID在第0列）
        QString announcementId = ui->tableView_5->model()->data(ui->tableView_5->model()->index(row, 0)).toString();

        if (announcementId.isEmpty()) {
            qWarning() << "无效的公告ID，跳过删除操作，行号:" << row;
            continue;
        }

        // 执行数据库删除
        QSqlQuery query(db);
        query.prepare("DELETE FROM ANNOUNCEMENT WHERE announcement_id = :id");
        query.bindValue(":id", announcementId);

        if (!query.exec()) {
            qCritical() << "删除公告失败，ID:" << announcementId << " 错误:" << query.lastError().text();

            // 回滚事务
            db.rollback();
            QMessageBox::critical(this, "删除失败", "无法从数据库删除公告:\n" + query.lastError().text());
            return;
        }
    }
    // 提交事务
    if (!db.commit()) {
        qCritical() << "提交删除事务失败:" << db.lastError().text();
        QMessageBox::critical(this, "操作失败", "数据库事务提交失败:\n" + db.lastError().text());
        return;
    }

    // 刷新表格数据（重新查询数据库）
    QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(ui->tableView_5->model());
    if (model) {
        model->setQuery("SELECT announcement_id, content, release_time FROM ANNOUNCEMENT");
        if (model->lastError().isValid()) {
            qWarning() << "刷新公告列表失败:" << model->lastError().text();
        }
    }
    QMessageBox::information(this, "操作成功", "所选公告已成功删除");
    setupChartAndTable();

}

