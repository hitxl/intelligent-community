#include "update_carfield_information.h"
#include "ui_update_carfield_information.h"
#include "function.h"

update_carfield_information::update_carfield_information(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::update_carfield_information)
{
    ui->setupUi(this);

    //连接数据库
       db = QSqlDatabase::addDatabase("QSQLITE");
       db.setDatabaseName("IntelligentCommunity.db"); // 数据库文件名（或完整路径）

       if (!db.open()) {
           qDebug() << "Error: Failed to open database:" << db.lastError().text();
           return;
       }
}

update_carfield_information::~update_carfield_information()
{
    delete ui;
}

void update_carfield_information::on_pushButton_clicked()
{
    QString input_spaceid=ui->carfile_id_lineEdit->text().trimmed();
    QString input_ownername=ui->owner_namelineEdit->text().trimmed();
    QString space_status = ui->carfield_status_comboBox->currentText();
    if(input_spaceid.isEmpty()){
         QMessageBox::information(this, "提示", "车位ID不能为空");
         return;
    }
    if(space_status=="vacant"){
        input_ownername="";
    }
    else if (space_status=="rented"&&input_ownername.isEmpty()) {
         QMessageBox::information(this, "提示", "租借人姓名不能为空");
         return;
    }
    QString sql = "UPDATE PARKING_SPACE_NEW SET ownername = ?, space_status = ? WHERE space_id = ?";
        QVariantList params = {input_ownername,space_status, input_spaceid};

        if (Function::executeNonQuery(sql, params)) {
            QMessageBox::information(this, "成功", "用户信息更新成功");
            // 重置表单
            ui->owner_namelineEdit->clear();
            ui->carfile_id_lineEdit->clear();
             ui->carfield_status_comboBox->setCurrentIndex(0);
            ui->carfield_status_comboBox->setEnabled(false);
        }else{
            QMessageBox::critical(this, "错误", "更新失败");
        }

}
