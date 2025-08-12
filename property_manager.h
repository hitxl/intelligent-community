#ifndef PROPERTY_MANAGER_H
#define PROPERTY_MANAGER_H

#include <QSqlQuery>
#include <QDebug>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QStyledItemDelegate>
#include <Qt>
#include "proper_manageradd.h"
#include <QFont>
#include "announce1.h"
#include <QHeaderView>
#include <QStyleOptionViewItem>
#include "property_staffadd.h"
#include "property_managerdel.h"
#include "property_staffdel.h"
#include <QSqlError>
#include <QLabel>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QtCharts>
QT_BEGIN_NAMESPACE
namespace Ui { class Property_Manager; }
QT_END_NAMESPACE

// 自定义委托类，实现内容居中
class Login;


// 自定义可编辑的 QueryModel（用于员工表）
class EditableSqlModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit EditableSqlModel(QObject *parent = nullptr, const QString &connectionName = "qt_sql_default_connection")
        : QSqlQueryModel(parent), m_connectionName(connectionName) {}

    Qt::ItemFlags flags(const QModelIndex &index) const override {
        Qt::ItemFlags flags = QSqlQueryModel::flags(index);
        if (index.column() != 0 && index.column() != 1) {
            flags |= Qt::ItemIsEditable;
        }
        return flags;
    }

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override {
        if (role != Qt::EditRole) return false;

        static const QMap<int, QString> fieldMap = {
            {0, "staff_id"},
            {1, "user_id"},
            {2, "position"},
            {3, "department"},
            {4, "admin_id"},
            {5, "attendance_count"}
        };

        if (!fieldMap.contains(index.column())) {
            qCritical() << "未知列索引，无法更新：" << index.column();
            return false;
        }

        QString fieldName = fieldMap[index.column()];
        QString staffId = data(index.siblingAtColumn(0)).toString();

        if (!QSqlDatabase::contains(m_connectionName)) {
            qCritical() << "数据库连接不存在：" << m_connectionName;
            return false;
        }

        QSqlDatabase db = QSqlDatabase::database(m_connectionName);
        if (!db.isOpen()) {
            qCritical() << "数据库连接已关闭：" << m_connectionName;
            return false;
        }

        db.transaction();

        QSqlQuery query(db);
        QString sql = QString("UPDATE staff SET %1 = :value WHERE staff_id = :staffId")
                          .arg(fieldName);
        query.prepare(sql);
        query.bindValue(":value", value);
        query.bindValue(":staffId", staffId);

        if (!query.exec()) {
            qCritical() << "数据库更新失败：" << query.lastError().text();
            qCritical() << "执行的SQL：" << sql;
            qCritical() << "绑定的值：" << value << ", staff_id:" << staffId;

            db.rollback();

            QMessageBox::critical(nullptr, "更新失败",
                                  QString("无法更新数据到数据库：\n%1\n\nSQL: %2")
                                      .arg(query.lastError().text())
                                      .arg(sql));
            return false;
        }

        if (!db.commit()) {
            qCritical() << "事务提交失败：" << db.lastError().text();
            QMessageBox::critical(nullptr, "提交失败",
                                  QString("无法提交更改到数据库：\n%1")
                                      .arg(db.lastError().text()));
            return false;
        }

        setQuery(this->query().lastQuery(), db);
        setHeaderData(0, Qt::Horizontal, "员工ID");
        setHeaderData(1, Qt::Horizontal, "用户ID");
        setHeaderData(2, Qt::Horizontal, "职位");
        setHeaderData(3, Qt::Horizontal, "部门");
        setHeaderData(4, Qt::Horizontal, "管理员ID");
        setHeaderData(5, Qt::Horizontal, "出勤次数");

        return true;
    }

private:
    QString m_connectionName;
};

// 自定义可编辑的 QueryModel（用于用户表）
class EditableUserSqlModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit EditableUserSqlModel(QObject *parent = nullptr, const QString &connectionName = "qt_sql_default_connection")
        : QSqlQueryModel(parent), m_connectionName(connectionName) {}

    Qt::ItemFlags flags(const QModelIndex &index) const override {
        Qt::ItemFlags flags = QSqlQueryModel::flags(index);
        flags |= Qt::ItemIsEditable;
        return flags;
    }

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override {
        if (role != Qt::EditRole) return false;

        static const QMap<int, QString> fieldMap = {
            {0, "user_id"},
            {1, "username"},
            {2, "password"},
            {3, "phone"},
            {4, "email"},
            {5, "register_time"},
            {6, "last_login"},
            {7, "user_type"}
        };

        if (!fieldMap.contains(index.column())) {
            qCritical() << "未知列索引，无法更新：" << index.column();
            return false;
        }

        QString fieldName = fieldMap[index.column()];
        QString userId = data(index.siblingAtColumn(0)).toString();

        if (!QSqlDatabase::contains(m_connectionName)) {
            qCritical() << "数据库连接不存在：" << m_connectionName;
            return false;
        }

        QSqlDatabase db = QSqlDatabase::database(m_connectionName);
        if (!db.isOpen()) {
            qCritical() << "数据库连接已关闭：" << m_connectionName;
            return false;
        }

        db.transaction();

        QSqlQuery query(db);
        QString sql = QString("UPDATE USER_ACCOUNT SET %1 = :value WHERE user_id = :userId")
                          .arg(fieldName);
        query.prepare(sql);
        query.bindValue(":value", value);
        query.bindValue(":userId", userId);

        if (!query.exec()) {
            qCritical() << "数据库更新失败：" << query.lastError().text();
            qCritical() << "执行的SQL：" << sql;
            qCritical() << "绑定的值：" << value << ", user_id:" << userId;

            db.rollback();

            QMessageBox::critical(nullptr, "更新失败",
                                  QString("无法更新数据到数据库：\n%1\n\nSQL: %2")
                                      .arg(query.lastError().text())
                                      .arg(sql));
            return false;
        }

        if (!db.commit()) {
            qCritical() << "事务提交失败：" << db.lastError().text();
            QMessageBox::critical(nullptr, "提交失败",
                                  QString("无法提交更改到数据库：\n%1")
                                      .arg(db.lastError().text()));
            return false;
        }

        setQuery(this->query().lastQuery(), db);
        setHeaderData(0, Qt::Horizontal, "用户ID");
        setHeaderData(1, Qt::Horizontal, "用户名");
        setHeaderData(2, Qt::Horizontal, "密码");
        setHeaderData(3, Qt::Horizontal, "电话");
        setHeaderData(4, Qt::Horizontal, "邮箱");
        setHeaderData(5, Qt::Horizontal, "注册时间");
        setHeaderData(6, Qt::Horizontal, "最后登录时间");
        setHeaderData(7, Qt::Horizontal, "身份权限");

        return true;
    }

private:
    QString m_connectionName;
};
//自定义不可编辑的 QueryModel（用于请假表）
class LeaveSqlModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit LeaveSqlModel(QObject *parent = nullptr, const QString &connectionName = "qt_sql_default_connection")
        : QSqlQueryModel(parent), m_connectionName(connectionName) {}

private:
    QString m_connectionName;
};

class Property_Manager : public QMainWindow
{
    Q_OBJECT

public:
    Property_Manager(QWidget *parent = nullptr);
    ~Property_Manager();
    void updateStatus(const QString &status);

private slots:
    // 切换界面设置
    void on_toolButton_5_clicked();
    void on_toolButton_10_clicked();
    void on_toolButton_11_clicked();

    // 刷新界面设置
    void handleEmployeeAdded();
    void handleUserDeleted();
    void handleStaffAdded();
    void handleStaffDeleted();

    void on_proper_manageradd_closed();
    void on_property_managerdel_closed();
    void on_property_staffadd_closed();
    void on_property_staffdel_closed();

    // 主界面按钮设置
    void on_pushButton_6_clicked();
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_16_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_13_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_EditFinish_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_17_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_15_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

private:
    Ui::Property_Manager *ui;
    QLabel *statusLabel;
    QSqlDatabase db;
    Login *loginWindow= nullptr;
    bool isEmployeeEditing = false;
    bool isUserEditing = false;
    Proper_Manageradd *proper_manageradd = nullptr;
    Property_Staffadd *property_staffadd = nullptr;
    Property_Staffdel *property_staffdel = nullptr;
    Property_Managerdel *property_managerdel = nullptr;
    announce1 *announceDialog;

    // 模型成员
    EditableSqlModel m_model1;
    EditableUserSqlModel m_userModel;
    QSqlQueryModel m_userAccountModel;
    LeaveSqlModel *leaveModel;
    QSqlQueryModel *feeModel; // 用于 tableView_2 的数据模型
    void setupChartAndTable(); // 同时初始化图表和表格
    void initDatabase();
    void initTableModels();
    void setupTableViews();
    void calculateAttendance();
    void loadUserAccountData();
    void loadStaffLeaveData();
};

#endif // PROPERTY_MANAGER_H
