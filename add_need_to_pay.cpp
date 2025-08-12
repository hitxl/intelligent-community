#include "add_need_to_pay.h"
#include "ui_add_need_to_pay.h"

add_need_to_pay::add_need_to_pay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::add_need_to_pay)
{
    ui->setupUi(this);
}

add_need_to_pay::~add_need_to_pay()
{
    delete ui;
}
