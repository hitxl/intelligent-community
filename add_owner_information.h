#ifndef ADD_OWNER_INFORMATION_H
#define ADD_OWNER_INFORMATION_H

#include <QWidget>

namespace Ui {
class Add_owner_information;
}

class Add_owner_information : public QWidget
{
    Q_OBJECT

public:
    explicit Add_owner_information(QWidget *parent = nullptr);
    ~Add_owner_information();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::Add_owner_information *ui;
};

#endif // ADD_OWNER_INFORMATION_H
