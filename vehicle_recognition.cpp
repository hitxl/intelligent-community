#include "vehicle_recognition.h"
#include "ui_vehicle_recognition.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QImage>
#include <QBuffer>
#include <QDebug>
#include <QSqlError>
#include <QJsonObject>
#include <QUrlQuery>
#include <QJsonArray>
#include <QJsonValue>
#include <QSslSocket>

// 定义密钥（仅在 cpp 中定义一次，避免重复）
const QString API_KEY_VALUE = "5CjeFltkXnf2aR0mn1XZBaTV";
const QString SECRET_KEY_VALUE = "Lr8tJ8UtjVUXsladpCFTDq14WvegA6oI";

Vehicle_Recognition::Vehicle_Recognition(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Vehicle_Recognition)
    , networkManager(new QNetworkAccessManager(this))
    , API_KEY(API_KEY_VALUE)  // 初始化密钥
    , SECRET_KEY(SECRET_KEY_VALUE)
{
    ui->setupUi(this);
    resize(800, 800);
    ui->selectImageBtn->setEnabled(false);

    // 检查 SSL 支持
    qDebug() << "OpenSSL 支持：" << QSslSocket::supportsSsl();
    qDebug() << "OpenSSL 版本：" << QSslSocket::sslLibraryVersionString();
    if (!QSslSocket::supportsSsl()) {
        QMessageBox::warning(this, "SSL 警告", "当前环境不支持 SSL，无法使用 HTTPS 请求");
    }

    // 数据库连接
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("IntelligentCommunity.db");
    }
    if (!db.open()) {
        QMessageBox::critical(this, "数据库错误", "无法打开数据库：" + db.lastError().text());
    }

    getBaiduAccessToken();
}

Vehicle_Recognition::~Vehicle_Recognition()
{
    if (db.isOpen()) db.close();
    delete ui;
}

void Vehicle_Recognition::on_selectImageBtn_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(
        this, "选择车辆图片", "", "图片文件 (*.jpg *.jpeg *.png *.bmp)"
        );
    if (imagePath.isEmpty()) return;

    QImage image(imagePath);
    if (image.isNull()) {
        QMessageBox::warning(this, "错误", "无法加载图片（请检查格式）");
        return;
    }
    if (image.width() < 15 || image.height() < 15 ||
        image.width() > 4096 || image.height() > 4096) {
        QMessageBox::warning(this, "尺寸错误", "图片尺寸不符合要求（最短边≥15px，最长边≤4096px）");
        return;
    }

    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    if (!image.save(&buffer, "JPG")) {
        QMessageBox::warning(this, "编码错误", "图片转换为 JPG 失败");
        return;
    }
    QByteArray imageData = buffer.data();
    if (imageData.size() > 8 * 1024 * 1024) {
        QMessageBox::warning(this, "大小超限", "图片编码后超过8M（百度官方限制）");
        return;
    }
    QString imageBase64 = imageData.toBase64();

    callBaiduLicensePlateAPI(imageBase64);
}

void Vehicle_Recognition::getBaiduAccessToken()
{
    QString tokenUrl = "https://aip.baidubce.com/oauth/2.0/token";
    QUrlQuery query;
    query.addQueryItem("grant_type", "client_credentials");
    query.addQueryItem("client_id", API_KEY);
    query.addQueryItem("client_secret", SECRET_KEY);

    QNetworkRequest request;
    request.setUrl(QUrl(tokenUrl + "?" + query.toString()));

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            qDebug() << "Token 原始返回：" << data;

            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
            if (parseError.error != QJsonParseError::NoError) {
                QMessageBox::critical(this, "解析错误", "Token 返回格式异常：" + parseError.errorString());
                reply->deleteLater();
                return;
            }

            QJsonObject obj = doc.object();
            if (obj.contains("error_code")) {
                QString errorMsg = QString("Token 获取失败：%1（错误码：%2）")  .arg(obj["error_msg"].toString()).arg(obj["error_code"].toInt());
                QMessageBox::critical(this, "官方接口错误", errorMsg);
                reply->deleteLater();
                return;
            }
            if (obj.contains("access_token")) {
                accessToken = obj["access_token"].toString();
                qDebug() << "已保存 Token：" << accessToken.left(20) << "...";
                ui->selectImageBtn->setEnabled(true);
            } else {
                QMessageBox::critical(this, "解析错误", "未找到 access_token");
            }
        } else {
            QString errorMsg = QString("Token 请求失败：%1（错误码：%2）").arg(reply->errorString()) .arg(reply->error());
            QMessageBox::critical(this, "网络错误", errorMsg);
        }
        reply->deleteLater();
    });
}

void Vehicle_Recognition::callBaiduLicensePlateAPI(const QString &imageBase64)
{
    if (accessToken.isEmpty()) {
        qDebug() << "Token 为空，重新获取";
        getBaiduAccessToken();
        return;
    }

    QString apiUrl = "https://aip.baidubce.com/rest/2.0/ocr/v1/license_plate";
    apiUrl += "?access_token=" + accessToken;

    QNetworkRequest request;
    request.setUrl(QUrl(apiUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray postData;
    postData.append("image=" + QUrl::toPercentEncoding(imageBase64));
    postData.append("&multi_detect=true");

    QNetworkReply *reply = networkManager->post(request, postData);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            QString errorMsg = QString("识别请求失败：%1（错误码：%2）") .arg(reply->errorString()) .arg(reply->error());
            QMessageBox::critical(this, "网络错误", errorMsg);
            reply->deleteLater();
            return;
        }

        QByteArray data = reply->readAll();
        qDebug() << "识别结果原始数据：" << data;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            QMessageBox::critical(this, "解析错误", "识别结果格式异常：" + parseError.errorString());
            reply->deleteLater();
            return;
        }

        QJsonObject resultObj = doc.object();
        reply->deleteLater();

        if (resultObj.contains("error_code")) {
            QString errorDetail = QString("官方错误码：%1\n错误信息：%2") .arg(resultObj["error_code"].toInt()) .arg(resultObj["error_msg"].toString());
            QMessageBox::critical(this, "API 调用失败", errorDetail);
            return;
        }

        QJsonArray wordsResult = resultObj["words_result"].toArray();
        if (wordsResult.isEmpty()) {
            QMessageBox::information(this, "识别结果", "未检测到车牌");
            return;
        }

        QJsonObject plateInfo = wordsResult[0].toObject();
        QString carNumber = plateInfo["number"].toString();
        QString color = plateInfo["color"].toString();

        if (carNumber.isEmpty() || color.isEmpty()) {
            QMessageBox::warning(this, "解析错误", "未获取到有效车牌号或颜色");
            return;
        }

        ui->resultLabel->setText(QString("车牌号：%1\n颜色：%2").arg(carNumber).arg(color));
        checkDatabaseAndShowResult(carNumber);
    });
}

void Vehicle_Recognition::checkDatabaseAndShowResult(const QString &carNumber)
{
    if (!db.isOpen()) {
        QMessageBox::critical(this, "数据库错误", "数据库连接已断开");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT status FROM PARKING_APPLICATION_NEW WHERE car_number = :carNumber");
    query.bindValue(":carNumber", carNumber);

    if (!query.exec()) {
        QMessageBox::critical(this, "查询失败", "数据库错误：" + query.lastError().text());
        return;
    }

    if (query.next()) {
        QString status = query.value(0).toString();
        if (status == "allocated") {
            QMessageBox::information(this, "放行许可", "车牌号：" + carNumber + "\n状态：已分配车位，允许出入");
        } else {
            QMessageBox::warning(this, "禁止进入", "车牌号：" + carNumber + "\n状态：" + status + "，无有效车位");
        }
    } else {
        QMessageBox::warning(this, "无记录", "未查询到车牌号：" + carNumber + " 的车位申请记录");
    }
}

