#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <signup.h>
#include<QRegExp>
#include<QMouseEvent>
#include<QList>
#include<QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();
    QThread* thread;
    QString offlineuser="";
    QString offlineToken="";
    Chat *chat;

public slots:
    void on_SignUpButton_clicked();
    void handleReply(QNetworkReply *reply);
    void on_RECAPTCHAOK_clicked();
//    void help();
    void on_usernameinput_textChanged(const QString &arg1);

    void on_SuggestionList_itemDoubleClicked(QListWidgetItem *item);

    void on_Seepasswordbutton_pressed();

    void on_Seepasswordbutton_clicked();

    void on_LoginButton_pressed();
    void checkifonline();
private:
    bool isonline;
    int counts_of_login=0;
    bool check_Captcha(QString &captcha, QString &user_input);
    // function to generate CAPTCHA of length n
    void generateCaptcha();
    QNetworkAccessManager manager;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int mouseClickMove_X;
    int mouseClickMove_Y;
    QString captcha;
    QVector<QString> users;
    QVector<QString> passwords;
    QVector<QString> token;
    int counter_in_suggestionlist=0;
    Ui::Login *ui;

};
#endif // LOGIN_H
