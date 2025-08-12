#ifndef OWNER_PAY_H
#define OWNER_PAY_H

#include <QMainWindow>

namespace Ui {
class Owner_Pay;
}

class Owner_Pay : public QMainWindow
{
    Q_OBJECT

public:
    explicit Owner_Pay(QWidget *parent = nullptr);
    ~Owner_Pay();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    //  void on_pushButton_clicked();

private:
    Ui::Owner_Pay *ui;
};

#endif // OWNER_PAY_H
