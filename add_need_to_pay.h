#ifndef ADD_NEED_TO_PAY_H
#define ADD_NEED_TO_PAY_H

#include <QWidget>

namespace Ui {
class add_need_to_pay;
}

class add_need_to_pay : public QWidget
{
    Q_OBJECT

public:
    explicit add_need_to_pay(QWidget *parent = nullptr);
    ~add_need_to_pay();

private:
    Ui::add_need_to_pay *ui;
};

#endif // ADD_NEED_TO_PAY_H
