#include "signup.h"
#include "ui_signup.h"
#include<login.h>
SignUp::SignUp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignUp)
{
    connect(&manager, &QNetworkAccessManager::finished, this, &SignUp::handleReply);
    ui->setupUi(this);

    ui->Passwordsinput->setToolTip("Please enter a strong password!\n Your password should include these items:\n Upercase,Lowercase,Numbers\n '@#$%^&*'");
    password_regex.setPattern("^(?=.*\\d)(?=.*[a-zA-Z])[a-zA-Z0-9 @#$%^&*-]{7,}$");
    password_regex_val.setRegularExpression(password_regex);

}

SignUp::~SignUp()
{
    delete ui;
}

void SignUp::on_Signupbutton_clicked()
{
    ui->Usernameexists->setText("");
    if(ui->Usernamesinput->text()==""&&ui->Passwordsinput->text()==""){
        ui->Usernamesinput->setStyleSheet("background-color:rgba(0,0,0,0);"
                                         "border:none;"
                                         "border-bottom: 2px solid rgba(255,0,0,200);"
                                         "color:rgba(0,0,0,240);"
                                         "padding-bottom:7px;");
        ui->Passwordsinput->setStyleSheet("background-color:rgba(0,0,0,0);"
                                         "border:none;"
                                         "border-bottom: 2px solid rgba(255,0,0,200);"
                                         "color:rgba(0,0,0,240);"
                                         "padding-bottom:7px;");
    }
    else if( ui->Usernamesinput->text()==""){
        ui->Usernamesinput->setStyleSheet("background-color:rgba(0,0,0,0);"
                                         "border:none;"
                                         "border-bottom: 2px solid rgba(255,0,0,200);"
                                         "color:rgba(0,0,0,240);"
                                         "padding-bottom:7px;");
        ui->Passwordsinput->setStyleSheet("background-color:rgba(0,0,0,0);"
                                         "border:none;"
                                         "border-bottom: 2px solid rgba(46,82,101,200);"
                                         "color:rgba(0,0,0,240);"
                                         "padding-bottom:7px;");
    }
    else if(ui->Passwordsinput->text()==""){
        ui->Passwordsinput->setStyleSheet("background-color:rgba(0,0,0,0);"
                                         "border:none;"
                                         "border-bottom: 2px solid rgba(255,0,0,200);"
                                         "color:rgba(0,0,0,240);"
                                         "padding-bottom:7px;");
        ui->Usernamesinput->setStyleSheet("background-color:rgba(0,0,0,0);"
                                         "border:none;"
                                         "border-bottom: 2px solid rgba(46,82,101,200);"
                                         "color:rgba(0,0,0,240);"
                                         "padding-bottom:7px;");
    }
    else{
    // Create a network request
    QUrl url("http://api.barafardayebehtar.ml:8080/signup");
    QUrlQuery query;
    query.addQueryItem("username", ui->Usernamesinput->text());
    query.addQueryItem("password", ui->Passwordsinput->text());
    query.addQueryItem("firstname", ui->Namesinput->text());
    query.addQueryItem("lastname", ui->FamilyNamesinput->text());

    url.setQuery(query);

    QNetworkRequest request(url);

    // Send the request using the GET method
    QNetworkReply *reply = manager.get(request);
}
}
void SignUp::handleReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QString reply_message = reply->readAll();
        qDebug()<<reply_message;
        QJsonDocument jDoc = QJsonDocument::fromJson(reply_message.toUtf8());
        QJsonObject jObj = jDoc.object();
        if(jObj["code"].toString().compare("204")== 0)
        {
            //existing username
            ui->Usernameexists->setText("username exists");

        }
        else if (jObj["code"].toString().compare("200")== 0)
        {
           //signed in successfully
            ui->FamilyNamesinput->clear();
            ui->Namesinput->clear();
            ui->Usernamesinput->clear();
            ui->Passwordsinput->clear();
            QFile logedinacc("logedinaccounts.txt");
            QTextStream stream(&logedinacc);
            if(!logedinacc.open(QIODevice::Append)){
                qDebug()<<"file is not oppening";
                qDebug()<<logedinacc.errorString();
            }
            else{
                stream<<ui->Usernamesinput->text()<<endl;
                stream<<ui->Passwordsinput->text()<<endl;
            }
            logedinacc.close();
            //logedinuser=jObj["token"].toString();
           Login *a=new Login();
           a->setWindowFlags(Qt::FramelessWindowHint);
           a->setStyleSheet("background-color: transparent;");
           a->show();

           this->hide();

        }
        else
        {
            //connection error
            ui->Usernameexists->setText("connection error");
        }





    } else {
        // Request failed
        qDebug() << "Request failed:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();
}


void SignUp::on_cancel_clicked()
{
    this->hide();
    Login *login = new Login();
    login->setWindowFlags(Qt::FramelessWindowHint);
    login->setStyleSheet("background-color: transparent;");
    login->show();
}

void SignUp::mousePressEvent(QMouseEvent *event)
{
    mouseClickMove_X = event->x();
    mouseClickMove_Y = event->y();
}

void SignUp::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalX()-mouseClickMove_X,event->globalY()-mouseClickMove_Y);
}






void SignUp::on_Seepasswordbutton_pressed()
{
    ui->Passwordsinput->setEchoMode(QLineEdit::Normal);
}


void SignUp::on_Seepasswordbutton_clicked()
{
    ui->Passwordsinput->setEchoMode(QLineEdit::Password);
}


void SignUp::on_Passwordsinput_textChanged(const QString &text)
{
    int pos = 0;
    QString text_tmp = text;

    if ((password_regex_val.validate(text_tmp, pos) == QValidator::Acceptable) &&
            ((5 <= text_tmp.length()) && (text_tmp.length() <= 10))){
        is_password_val = true;
        ui->Passwordsinput->setStyleSheet("border:none;\
                                          border-bottom: 2px solid rgb(0, 255, 0);\
                                          color:rgba(0,0,0,240);\
                                          padding-bottom:7px;");
    }
    else if (text_tmp.isEmpty()) {
        is_password_val = false;
        ui->Passwordsinput->setStyleSheet("border:none;\
                                          border-bottom: 2px solid rgb(255, 0, 0) ;\
                                          color:rgba(0,0,0,240);\
                                          padding-bottom:7px;");
    }
    else {
        is_password_val = false;
        ui->Passwordsinput->setStyleSheet("border:none;\
                                          border-bottom: 2px solid rgb(255, 0, 0) ;\
                                          color:rgba(0,0,0,240);\
                                          padding-bottom:7px;");
    }

}
