#ifndef PROPERTY_STAFF_H
#define PROPERTY_STAFF_H
#include <QStyleOptionViewItem>
#include "qdatetimeedit.h"
#include "qpushbutton.h"
#include "qsqldatabase.h"
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTableView>
#include<QAbstractItemModel>
#include <QSqlQueryModel>
#include <QStyledItemDelegate>
#include <QModelIndex>
#include"CenteredDelegate1.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class Property_Staff;
}
QT_END_NAMESPACE
class Login;


class Property_Staff : public QMainWindow
{
    Q_OBJECT

public:
    Property_Staff(QWidget *parent = nullptr);
    ~Property_Staff();

private slots:

    void on_Checkin_Button_clicked();

    void on_Leave_Button_2_clicked();

    void on_Attendence_Button_3_clicked();

    void on_Repair_Button_4_clicked();

    void on_Parking_Button_clicked();

    void on_Feemanagement_Button_clicked();

    void on_Ownermanagement_Button_2_clicked();

    void on_reflash_clicked();

    void on_reflash2_clicked();

    void on_reflash3_clicked();

    void on_pushButton_18_clicked();

    void on_datechoose_pushButton_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_3_clicked();

    void on_find_space_pushButton_5_clicked();

    void on_spaceprice_findid_pushButton_clicked();

    void on_spaceprice_change_pushButton_7_clicked();

    void on_contract_findpushButton_19_clicked();

    void on_contractid_findpushButton_11_clicked();

    void on_contract_changepushButton_10_clicked();

    void on_filter_pushButton_5_clicked();

    bool deleteOwnerInformation(const QString &ownerId);

    void on_contract_addpushButton_9_clicked();

    void on_approveButton_clicked();

    void on_pushButton_15_clicked();

    void on_chose_pushButton_5_clicked();

    void on_find_my_repairpushButton_5_clicked();

    void on_confirm_progress_changepushButton_5_clicked();

    void on_repair_id_pushButton_6_clicked();

    void on_staff_id_pushButton_5_clicked();

    void loadFeedbackData(const QString &repairId, const QString &staffId);

    void on_go_to_work_2_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_14_clicked();

    void on_go_off_work_2_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_23_clicked();

    void deleteowner(const QString &ownerId);

    void on_tabWidget_2_tabBarClicked(int index1);

    void on_caridentify_clicked();

    void on_Vehicle_Recognition_clicked();

    void on_toolButton_clicked();

private:
    Ui::Property_Staff *ui;
    QSqlDatabase db;
    Login *loginWindow= nullptr;

};
#endif // PROPERTY_STAFF_H
