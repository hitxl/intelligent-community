#include "function.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlQuery>


Function::Function() {}
Function::~Function() {}

bool Function::connectDatabase(QSqlDatabase &db) {
    // 检查是否已存在默认连接
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
        db.setDatabaseName("IntelligentCommunity.db");
    }

    // 仅在未打开时尝试打开数据库
    if (!db.isOpen()) {
        if (!db.open()) {
            qDebug() << "数据库连接失败:" << db.lastError().text();
            return false;
        }
        qDebug() << "数据库连接成功";
        return true;
    }

    return true;
}
void Function::closeDatabase(QSqlDatabase &db) {
    if (db.isOpen()) {
        db.close();
        qDebug() << "数据库连接已关闭";
    }
}

QSqlQuery Function::executeQuery(const QString &sql, const QVariantList &params) {
    QSqlDatabase db;
    if (!connectDatabase(db)) {
        return QSqlQuery();
    }

    QSqlQuery query(db);
    query.prepare(sql);

    for (int i = 0; i < params.size(); ++i) {
        query.bindValue(i, params[i]);
    }

    if (!query.exec()) {
        qDebug() << "查询执行失败: " << query.lastError().text();
        qDebug() << "SQL: " << sql;
    }
    return query;
}

bool Function::executeNonQuery(const QString &sql, const QVariantList &params) {
    QSqlDatabase db;
    if (!connectDatabase(db)) {
        return false;
    }

    QSqlQuery query(db);
    query.prepare(sql);

    for (int i = 0; i < params.size(); ++i) {
        query.bindValue(i, params[i]);
    }

    if (!query.exec()) {
        qDebug() << "非查询执行失败: " << query.lastError().text();
        qDebug() << "SQL: " << sql;
        return false;
    }
    return true;
}

QSqlRecord Function::querySingleRecord(const QString &sql, const QVariantList &params) {
    QSqlQuery query = executeQuery(sql, params);
    if (query.next()) {
        return query.record();
    }
    return QSqlRecord();
}

bool Function::userExists(int userId) {
    QString sql = "SELECT user_id FROM USER_ACCOUNT WHERE user_id = ?";
    QVariantList params = {userId};
    QSqlRecord record = querySingleRecord(sql, params);
    return record.count() > 0;
}

QString Function::getUserType(int userId) {
    QString sql = "SELECT user_type FROM USER_ACCOUNT WHERE user_id = ?";
    QVariantList params = {userId};
    QSqlRecord record = querySingleRecord(sql, params);
    if (record.count() > 0) {
        return record.value("user_type").toString();
    }
    return "";
}

