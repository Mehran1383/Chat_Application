#ifndef LOGOUT_H
#define LOGOUT_H
#include<QNetworkAccessManager>
#include <QDialog>
#include<QFile>
namespace Ui {
class Logout;
}

class Logout : public QDialog
{
    Q_OBJECT

public:
    QString user;
    QString passw;
    explicit Logout(QWidget *parent = nullptr);
    ~Logout();
    QNetworkAccessManager manager;

private slots:
     void handleReply(QNetworkReply *reply);
    void on_Logoutbutton_pressed();

    void on_Cancel_clicked();

private:
    Ui::Logout *ui;
};

#endif // LOGOUT_H
