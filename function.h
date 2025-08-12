#ifndef FUNCTION_H
#define FUNCTION_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariantList>

class Function {
public:
    Function();
    ~Function();
    static bool connectDatabase(QSqlDatabase &db);
    static void closeDatabase(QSqlDatabase &db);
    static bool userExists(int userId);
    static QString getUserType(int userId);
    static QSqlQuery executeQuery(const QString &sql, const QVariantList &params = QVariantList());
    static bool executeNonQuery(const QString &sql, const QVariantList &params = QVariantList());
    static QSqlRecord querySingleRecord(const QString &sql, const QVariantList &params = QVariantList());

};

#endif // FUNCTION_H
