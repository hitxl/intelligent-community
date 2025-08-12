#ifndef OWNER1_H
#define OWNER1_H

#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QWidget>
#include <QSqlQueryModel>
#include <QStyledItemDelegate>
#include <Qt>
#include <QFont>
#include <QHeaderView>
#include <QStyleOptionViewItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class Owner;
}
QT_END_NAMESPACE

// 自定义委托类，实现内容居中
class Login;
class Owner : public QWidget
{
    Q_OBJECT

public:
    explicit Owner(QWidget *parent = nullptr);
    ~Owner();

private slots:
    void on_toolButton_4_clicked();
    void on_toolButton_6_clicked();
    void on_toolButton_5_clicked();
    void on_checkbutton_2_clicked();
    void on_payButton_2_clicked();
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_6_clicked();
    void on_tabWidget_3_tabBarClicked(int index);
    void on_toolButton_clicked();
    void on_toolButton_7_clicked();
private:
    Ui::Owner *ui;
    QSqlDatabase db;
    QSqlQueryModel m_userAccountModel;
    QSqlQueryModel m_parkingApplyModel;
    QSqlQueryModel m_parkingSpaceModel;
    QSqlQueryModel m_myparkingSpaceModel;
    Login *loginWindow= nullptr;
    void initDatabase();
    void initTableModels();
    void setupTableViews();
    void calculateAttendance();
    void loadParkingApplicationData();
    void loadParkingSpaceData();
    void loadmyParkingSpaceData();
};

#endif // OWNER1_H
