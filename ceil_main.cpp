#include "ceil_main.h"
#include "ui_ceil_main.h"

Ceil_Main::Ceil_Main(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Ceil_Main)
{
    ui->setupUi(this);
}

Ceil_Main::~Ceil_Main()
{
    delete ui;
}
