#include "owner_evaluation.h"
#include "ui_owner_evaluation.h"
#include <QMessageBox>
#include <QSqlError>
#include <QScreen>  // 替代 QDesktopWidget 的头文件
#include <QApplication>

Owner_Evaluation::Owner_Evaluation(int repairId, QWidget *parent)
    : QWidget(parent, Qt::Window | Qt::WindowCloseButtonHint)  // 带关闭按钮的窗口
    , ui(new Ui::owner_evaluation)
    , m_repairId(repairId)
{
    ui->setupUi(this);
    QHBoxLayout *horizontalLayout = qobject_cast<QHBoxLayout*>(ui->widget->layout());
    if (horizontalLayout) {
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
    }
    resize(800, 800);
    setWindowTitle("维修评价");

    // 窗口关闭时自动释放资源
    setAttribute(Qt::WA_DeleteOnClose);

    // 调用居中函数
    centerWindow();
}

Owner_Evaluation::~Owner_Evaluation()
{
    delete ui;
}

// 窗口居中（基于 QScreen 实现，兼容 Qt5 新版本和 Qt6）
void Owner_Evaluation::centerWindow()
{
    // 获取窗口的几何信息
    QRect windowRect = this->geometry();

    // 获取屏幕中心坐标（支持多屏幕，取主屏幕）
    QPoint screenCenter = QApplication::primaryScreen()->geometry().center();

    // 计算窗口左上角坐标（使窗口中心与屏幕中心重合）
    windowRect.moveCenter(screenCenter);
    this->setGeometry(windowRect);
}

// 提交评价
void Owner_Evaluation::on_pushButton_clicked()
{
    QString evalContent = ui->textEdit->toPlainText().trimmed();

    if (evalContent.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "评价内容不能为空", QMessageBox::Ok);
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE REPAIR SET evaluation = :eval WHERE repair_id = :rid");
    query.bindValue(":eval", evalContent);
    query.bindValue(":rid", m_repairId);

    if (query.exec()) {
        QMessageBox::information(this, "评价成功", "评价内容已提交", QMessageBox::Ok);
        emit evaluationSubmitted();  // 通知主窗口刷新
        close();
    } else {
        QMessageBox::critical(this, "提交失败",
                              "评价提交失败：" + query.lastError().text(),
                              QMessageBox::Ok);
    }
}

// 关闭窗口（如果UI中有关闭按钮，绑定此函数）


void Owner_Evaluation::on_pushButton_2_clicked()
{
    close();

}

