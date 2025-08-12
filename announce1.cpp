#include "announce1.h"
#include "ui_announce1.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

announce1::announce1(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::announce1)
{
    ui->setupUi(this);

    // 初始化数据库连接（类成员 db）
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    db.setDatabaseName("IntelligentCommunity.db");
    if (!db.open()) {
        QMessageBox::critical(this, "错误", "数据库连接失败: " + db.lastError().text());
    }
    // 连接按钮信号（确认发布按钮，假设你的按钮 objectName 是 pushButton ）
    connect(ui->pushButton, &QPushButton::clicked, this, &announce1::on_pushButton_clicked);
}

announce1::~announce1()
{
    db.close(); // 关闭数据库连接
    delete ui;
}

void announce1::on_pushButton_clicked()
{
    // 1. 获取内容并判空
    QString content = ui->textEdit->toPlainText().trimmed();
    // 2. 严格判空（注意括号范围）
    if (content.isEmpty()) {
        return; // 内容为空时直接返回，不执行后续操作
    }
    // 3. 内容非空时执行数据库插入
    QSqlQuery query(db);
    query.prepare("INSERT INTO ANNOUNCEMENT (content) VALUES (:content)");
    query.bindValue(":content", content);
    if (query.exec()) {
        QMessageBox::information(this, "成功", "公告添加成功！");
        ui->textEdit->clear(); // 清空文本框
        this->close(); // 关闭窗口
    } else {
        qDebug() << "公告插入失败：" << query.lastError().text();
        QMessageBox::warning(this, "提示", "发布失败，请重试");
    }
}
