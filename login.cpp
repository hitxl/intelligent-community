#include "login.h"
#include "ui_login.h"

#include "property_manager.h"
#include "owner1(2).h"
#include"property_staff.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
int Login::currentUserId = -1;
const QString DB_PATH = "IntelligentCommunity.db";
QString Login::currentUsername = "";

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
    , propertyManager(nullptr)

    ,propertyStaff(nullptr)
    , ownerMainWindow(nullptr)
{
    ui->setupUi(this);
    initDatabase();
}

Login::~Login()
{
    if (db.isOpen()) db.close();
    delete ui;
}

// 数据库初始化（打开数据库连接）
void Login::initDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DB_PATH);
    if (!db.open()) {
        QMessageBox::critical(this, "错误", "数据库打开失败：" + db.lastError().text());
    } else {
        qDebug() << "数据库连接成功！";
    }
}

// 校验用户名+密码是否匹配
bool Login::checkLogin(const QString &username, const QString &password, int *userId, QString *userType)
{
    QSqlQuery query(db);
    // 明确从 USER_ACCOUNT 表查询 username 对应的 user_id 和 user_type
    query.prepare("SELECT user_id, user_type FROM USER_ACCOUNT "
                  "WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        *userId = query.value("user_id").toInt(); // 从 USER_ACCOUNT 表获取 user_id
        *userType = query.value("user_type").toString();
        return true;
    }

    return false;
}
void Login::on_Confirm_Button_clicked()
{
    QString username = ui->lineEdit_username->text().trimmed();
    QString password = ui->lineEdit_password->text().trimmed();

    // 1. 空值校验
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "提示", "用户名和密码不能为空！");
        return;
    }

    // 2. 校验用户名+密码，从 USER_ACCOUNT 表获取 user_id
    int userId = -1;
    QString userType;
    if (!checkLogin(username, password, &userId, &userType)) {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误，请重新输入！");
        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
        return;
    }

    // 3. 保存用户信息到全局变量（currentUserId 为 USER_ACCOUNT.user_id）
    currentUserId = userId; // 此处 userId 来自 USER_ACCOUNT 表的 user_id
    currentUsername = username;
    qDebug() << "当前登录用户的 user_id（来自 USER_ACCOUNT）：" << currentUserId;

    // 4. 校验用户信息有效性
    if (userType.isEmpty() || userId == -1) {
        QMessageBox::warning(this, "提示", "获取用户信息异常，请重新登录！");
        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
        return;
    }

    // 5. 按用户类型跳转主界面（保持原有逻辑）
    qDebug() << "准备跳转，用户类型：" << userType;
    if (userType == "admin") {
        if (!propertyManager) {
            propertyManager = new Property_Manager(this);
        }
        this->close();
        propertyManager->show();
    } else if (userType == "owner") {
        if (!ownerMainWindow) {
            ownerMainWindow = new Owner(nullptr);
        }
        this->close();
        ownerMainWindow->show();
        qDebug() << "Owner 界面显示状态：" << ownerMainWindow->isVisible();
    } else if (userType == "staff") {
        if (!propertyStaff) {
            propertyStaff = new Property_Staff(nullptr);
        }
        this->close();
        propertyStaff->show();
        qDebug() << "Staff 界面显示状态：" << propertyStaff->isVisible();
    }
}
// 退出按钮点击事件
void Login::on_Exit_Button_clicked()
{
    this->close();
    QApplication::quit();
}


