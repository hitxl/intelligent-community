#ifndef VEHICLE_RECOGNITION_H
#define VEHICLE_RECOGNITION_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class Vehicle_Recognition;
}

class Vehicle_Recognition : public QWidget
{
    Q_OBJECT

public:
    explicit Vehicle_Recognition(QWidget *parent = nullptr);
    ~Vehicle_Recognition();

private slots:
    void on_selectImageBtn_clicked();

private:
    Ui::Vehicle_Recognition *ui;
    QNetworkAccessManager *networkManager;
    QSqlDatabase db;

    // 仅声明密钥（不定义，避免重复）
    const QString API_KEY;
    const QString SECRET_KEY;
    QString accessToken;

    void getBaiduAccessToken();
    void callBaiduLicensePlateAPI(const QString &imageBase64);
    void checkDatabaseAndShowResult(const QString &carNumber);
};

#endif // VEHICLE_RECOGNITION_H
