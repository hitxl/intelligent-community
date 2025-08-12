#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include "property_manager.h"
#include "owner1(2).h"
#include"property_staff.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    static int currentUserId;
    static QString currentUsername;
private slots:
    void on_Confirm_Button_clicked();
    void on_Exit_Button_clicked();
    void initDatabase();

private:
    Ui::Login *ui;
    QSqlDatabase db;
    Property_Manager *propertyManager;
    Property_Staff *propertyStaff;
    Owner *ownerMainWindow;
    bool checkLogin(const QString &username, const QString &password, int *userId, QString *userType);

};

#endif // LOGIN_H
