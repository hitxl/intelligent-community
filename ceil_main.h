#ifndef CEIL_MAIN_H
#define CEIL_MAIN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Ceil_Main;
}
QT_END_NAMESPACE

class Ceil_Main : public QMainWindow
{
    Q_OBJECT

public:
    Ceil_Main(QWidget *parent = nullptr);
    ~Ceil_Main();

private:
    Ui::Ceil_Main *ui;
};
#endif // CEIL_MAIN_H
