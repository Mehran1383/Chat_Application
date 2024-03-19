#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>
#include<QtNetwork/QNetworkAccessManager>
#include<QtNetwork/QNetworkReply>
#include<QtNetwork/QNetworkRequest>
#include<QtNetwork/QAuthenticator>
#include<QtNetwork/QNetworkProxy>
#include <QUrlQuery>
#include <QLineEdit>
#include<QJsonDocument>
#include<QJsonObject>
#include<chat.h>
#include<QFile>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
namespace Ui {
class SignUp;
}

class SignUp : public QDialog
{
    Q_OBJECT
public:
    explicit SignUp(QWidget *parent = nullptr);
    ~SignUp();
//public slots:
//    void get(QString location);
//    void send(QString location,QByteArray data);
private slots:
    //void on_pushButton_7_clicked();

    void on_cancel_clicked();
    void handleReply(QNetworkReply *reply);
    void on_Signupbutton_clicked();
    void on_Seepasswordbutton_pressed();
    void on_Seepasswordbutton_clicked();
    void on_Passwordsinput_textChanged(const QString &text);

private:
    Ui::SignUp *ui;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int mouseClickMove_X;
    int mouseClickMove_Y;
    QNetworkAccessManager manager;
    QRegularExpression  password_regex;
    QRegularExpressionValidator password_regex_val;
    bool is_password_val = false;
};

#endif // SIGNUP_H
