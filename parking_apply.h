#ifndef PARKING_APPLY_H
#define PARKING_APPLY_H

#include <QMainWindow>

namespace Ui {
class Parking_Apply;
}

class Parking_Apply : public QMainWindow
{
    Q_OBJECT
public:
    explicit Parking_Apply(QWidget *parent = nullptr);
    ~Parking_Apply();
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
private:
    Ui::Parking_Apply *ui;
};

#endif // PARKING_APPLY_H
