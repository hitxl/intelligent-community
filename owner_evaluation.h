#ifndef OWNER_EVALUATION_H
#define OWNER_EVALUATION_H

#include <QWidget>
#include <QSqlQuery>
#include <QMessageBox>
namespace Ui {
class owner_evaluation;
}

class Owner_Evaluation : public QWidget
{
    Q_OBJECT

public:
    // 构造函数：接收维修ID，parent 为父窗口
    explicit Owner_Evaluation(int repairId, QWidget *parent = nullptr);
    ~Owner_Evaluation();  // 析构函数

signals:
    // 评价提交完成后发出的信号，用于通知主界面刷新
    void evaluationSubmitted();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::owner_evaluation *ui;       // UI 对象指针
    int m_repairId;                 // 待评价的维修记录 ID
    void centerWindow();
};

#endif // OWNER_EVALUATION_H
