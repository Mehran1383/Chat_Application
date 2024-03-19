#include "logout.h"
#include "ui_logout.h"
#include<QTextStream>
#include<QUrl>
#include<QUrlQuery>
#include<QDebug>
#include<QNetworkRequest>
#include<QNetworkReply>
Logout::Logout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Logout)
{
    ui->setupUi(this);
    QFile username("username.txt");
    QTextStream userstream(&username);
    if(username.open(QIODevice::ReadOnly)){
            user=userstream.readLine();
}
    else{
        qDebug()<<"cant oppen the file";

    }
    username.close();


    QFile password("password.txt");
    QTextStream passstream(&password);
    if(password.open(QIODevice::ReadOnly)){
            passw=userstream.readLine();
}
    else{
        qDebug()<<"cant oppen the file";

    }
    password.close();
}

Logout::~Logout()
{
    delete ui;
}

void Logout::handleReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QString reply_message = reply->readAll();
        qDebug()<<reply_message;


        QFile deltoken("token.txt");
        deltoken.open(QIODevice::WriteOnly);
        deltoken.close();

        QFile delpass("password.txt");
        delpass.open(QIODevice::WriteOnly);
        delpass.close();

        QFile deluser("username.txt");
        deluser.open(QIODevice::WriteOnly);
        deluser.close();

        exit(0);

    }
    else{
        qDebug()<<"Reply error";
    }
}

void Logout::on_Logoutbutton_pressed()
{
    QUrl url("http://api.barafardayebehtar.ml:8080/login");
        QUrlQuery query;
        query.addQueryItem("username",user);
        query.addQueryItem("password",passw);
        url.setQuery(query);
        QNetworkRequest request(url);
        QNetworkReply *reply = manager.get(request);
}


void Logout::on_Cancel_clicked()
{
    this->hide();
}

