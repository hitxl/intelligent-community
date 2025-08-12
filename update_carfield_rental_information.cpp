#include "update_carfield_rental_information.h"
#include "ui_update_carfield_rental_information.h"
#include "function.h"

update_carfield_rental_information::update_carfield_rental_information(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::update_carfield_rental_information)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QSQLITE");
           db.setDatabaseName("IntelligentCommunity.db"); // 数据库文件名（或完整路径）

           if (!db.open()) {
               qDebug() << "Error: Failed to open database:" << db.lastError().text();
               return;
           }
}

update_carfield_rental_information::~update_carfield_rental_information()
{
    delete ui;
}

void update_carfield_rental_information::on_pushButton_clicked()
{
    //跳出信息确认页面，但是不要新建文件，在原来的文件内部实现，显示被修改的出租合同信息，包括ownername,rental_id
    //要求表达出：请您确认是否修改XXX的X订单
    //点击确定执行相应的数据库操作，更改payment_status和end_date,fee等信息，点击取消则返回原界面
//contract_enddateEdit//contract_fee_lineEdit//comboBox//lineEdit
    QString input_fee=ui->contract_fee_lineEdit->text().trimmed();
    QString input_id=ui->lineEdit->text().trimmed();
    QString payment_status = ui->comboBox->currentText();
    QDate enddata=ui->contract_enddateEdit->date();
    QString end_data=enddata.toString("yyyy-MM-dd");
    if (input_fee.isEmpty() || input_id.isEmpty() || end_data.isEmpty()) {
            QMessageBox::warning(this, "输入错误", "请确保所有信息都已输入！");
            return;
        }
    QString sql ="UPDATE PARKING_RENTAL_NEW SET fee=?,payment_status=?,end_date=? WHERE rental_id=?";
    QVariantList params ={input_fee,payment_status,end_data,input_id};
    if (Function::executeNonQuery(sql, params)) {
                QMessageBox::information(this, "成功", "用户信息更新成功");
                // 重置表单
                ui->contract_fee_lineEdit->clear();
                ui->lineEdit->clear();
                ui->comboBox->setEnabled(false);
            }
}
