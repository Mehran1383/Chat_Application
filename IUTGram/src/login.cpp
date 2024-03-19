#include "login.h"
#include<oflinechat.h>
#include "ui_login.h"
#include<fstream>
//#include<about.h>
#include<iostream>
#include<QString>
#include<QMessageBox>
#include<QShortcut>
#include<QDesktopServices>
#include <QMovie>
Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{

    //shortcuts
    new QShortcut(QKeySequence(Qt::Key_Enter), this, SLOT(on_LoginButton_pressed()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y), this, SLOT(makecheckerok()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(exit(1)));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N), this, SLOT(on_SignUpButton_clicked()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_H), this, SLOT(help()));
     connect(&manager, &QNetworkAccessManager::finished, this, &Login::handleReply);
    QString tokenn="";
    QFile checktoken("token.txt");
    checktoken.open(QFile::ReadOnly | QFile::Text);
    QTextStream chstream(&checktoken);
    chstream >> tokenn;
    checktoken.close();
    ui->setupUi(this);
    checkifonline();
    if(tokenn!=""){
        if(isonline){
        chat=new Chat;
        chat->setWindowFlags(Qt::FramelessWindowHint);
        chat->setStyleSheet("background-color: transparent;");
        chat->show();
        this->close();
        chat->exec();
    }
        else{

            OflineChat *chat=new OflineChat;
            chat->setWindowFlags(Qt::FramelessWindowHint);
            chat->setStyleSheet("background-color: transparent;");
            chat->show();
            this->close();
            chat->exec();
        }
        exit(1);
    }

    QTimer *timer=new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(checkifonline()));
    timer->start(1000);
    ui->alreadySigned->hide();
    ui->Captcha->hide();
    ui->Recaptcha->hide();
    ui->RECAPTCHAOK->hide();
   // ui->About->hide();
   // ui->telegramlink->hide();
   // ui->aboutokbutton->hide();

    QMovie *movie = new QMovie(":/Loginimage/couple-chatting-on-mobile-3773159-3162252.gif");
    ui->label_3->setMovie(movie);
    movie->start();

        int i=0;
         QFile logedinacc("logedinaccounts.txt");
         QTextStream stream(&logedinacc);
         if(!logedinacc.open(QIODevice::ReadOnly)){
             qDebug()<<"file is not oppening";
             qDebug()<<logedinacc.errorString();
         }
         else{

             logedinacc.seek(0);

             while(!stream.atEnd()){
                i++;
                QString mmuser=stream.readLine();
                if(mmuser!=""){
                users.push_back(mmuser);
                passwords.push_back(stream.readLine());
                token.push_back(stream.readLine());
}
//                else{
//                    QString mmuser=stream.readLine();
//                    mmuser=stream.readLine();
//                }
             }

         for(auto it=users.begin();it!=users.end();it++){

         ui->SuggestionList->addItem(*it);

         }
    }
         if(i==0){
             qDebug()<<"added to list";
             ui->SuggestionList->hide();
         }
         logedinacc.close();
}

Login::~Login()
{

    delete ui;
}
void Login::mousePressEvent(QMouseEvent *event)
{
    mouseClickMove_X = event->x();
    mouseClickMove_Y = event->y();
}

void Login::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalX()-mouseClickMove_X,event->globalY()-mouseClickMove_Y);
}

void Login::on_SignUpButton_clicked()
{

    SignUp w;
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.setStyleSheet("background-color: transparent;");
    w.show();
    this->hide();
    w.exec();
}

void Login::handleReply(QNetworkReply *reply)
{
qDebug()<<"halat"<<isonline;

    if (reply->error() == QNetworkReply::NoError) {
        QString reply_message = reply->readAll();
        qDebug()<<reply_message;
        QJsonDocument jDoc = QJsonDocument::fromJson(reply_message.toUtf8());
        QJsonObject jObj = jDoc.object();
        //if(jObj["message"]!="You are already logged in!"){
        if(jObj["code"].toString().compare("200")== 0){
           qDebug()<<jObj["token"].toString();

            QFile logedinacc("logedinaccounts.txt");
            QTextStream stream(&logedinacc);
            if(!logedinacc.open(QIODevice::Append)){
                qDebug()<<"file is not oppening";
                qDebug()<<logedinacc.errorString();
            }
            else{
                int ifavailable=0;
                int count =0;
                for(auto it=users.begin();it!=users.end();it++){
                    if(*it==ui->usernameinput->text()){
                        ifavailable=1;
                        break;
                    }
                    count++;}
                QFile file("token.txt");

                file.open(QFile::WriteOnly | QFile::Text);
                QTextStream pstream(&file);
                QFile username("username.txt");
                QTextStream userstream(&username);
                username.open(QIODevice::WriteOnly);

                if(ifavailable==0){
                    qDebug()<<"added to file";
                stream<<ui->usernameinput->text()<<endl;
                stream<<ui->passwordinput->text()<<endl;
                stream<<jObj["token"].toString()<<endl;
                pstream << jObj["token"].toString();
                file.close();
                userstream<<ui->usernameinput->text()<<endl;
                username.close();
                QFile password("password.txt");
                QTextStream passstream(&password);
                password.open(QIODevice::WriteOnly);
                passstream<<ui->passwordinput->text();
                password.close();

            }
                else{
                     pstream << jObj["token"].toString();
                    //pstream << token[count];
                    file.close();
                    userstream<<ui->usernameinput->text()<<endl;
                    username.close();
                    QFile password("password.txt");
                    QTextStream passstream(&password);
                    password.open(QIODevice::WriteOnly);
                    passstream<<ui->passwordinput->text();
                    password.close();
                }
            }
            logedinacc.close();
            counts_of_login=0;
            ui->errorlable->setText("");
            ui->errorlable_2->setText("");
            ui->passwordinput->setStyleSheet("background-color:rgba(0,0,0,0);"
                                             "border:none;"
                                             "border-bottom: 2px solid rgba(46,82,101,200);"
                                             "color:rgba(0,0,0,240);"
                                             "padding-bottom:7px;");

            ui->usernameinput->setStyleSheet("background-color:rgba(0,0,0,0);"
                                             "border:none;"
                                             "border-bottom: 2px solid rgba(46,82,101,200);"
                                             "color:rgba(0,0,0,240);"
                                             "padding-bottom:7px;");


            ui->passwordinput->clear();
            ui->usernameinput->clear();
            chat=new Chat;
            chat->setWindowFlags(Qt::FramelessWindowHint);
            chat->setStyleSheet("background-color: transparent;");
            chat->show();
            this->hide();
            chat->exec();//}
//        else{

//                ui->alreadySigned->show();

//                for(int i=0;i<40;i++){
//                    ui->alreadySigned->move(270+i,120);

//                }
//                for(int i=0;i<10;i++){
//                    ui->alreadySigned->move(270+40-i,120);
//                }
//            }
        }
        else if(jObj["code"].toString().compare("401")== 0||jObj["code"].toString().compare("404")==0)
        {
            counts_of_login++;
           ui->errorlable->setText("x");
           ui->errorlable_2->setText("x");
           ui->passwordinput->setStyleSheet("background-color:rgba(0,0,0,0);"
                                            "border:none;"
                                            "border-bottom: 2px solid rgba(255,0,0,200);"
                                            "color:rgba(0,0,0,240);"
                                            "padding-bottom:7px;");

           ui->usernameinput->setStyleSheet("background-color:rgba(0,0,0,0);"
                                            "border:none;"
                                            "border-bottom: 2px solid rgba(255,0,0,200);"
                                            "color:rgba(0,0,0,240);"
                                            "padding-bottom:7px;");
        }
        else
        {
            qDebug()<<"connection error";
        }
    } else {
            qDebug() << "Request failed:" << reply->errorString();
        }
        reply->deleteLater();


}



bool Login::check_Captcha(QString &captcha, QString &user_input){
    return captcha.compare(user_input) == 0;
}
// function to generate CAPTCHA of length n
void Login::generateCaptcha(){
    time_t t;
    int n=7;
    srand((unsigned)time(&t));
    QString required_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    captcha = "";
    while(n--)
        captcha.push_back(required_chars[rand()%62]);

}
void Login::on_RECAPTCHAOK_clicked()
{
    if(ui->Recaptcha->text()==captcha){
        counts_of_login=0;
        ui->Captcha->hide();
        ui->Recaptcha->hide();
        ui->RECAPTCHAOK->hide();
    }
    else{
        generateCaptcha();
        ui->Captcha->setText(captcha);
    }
}

//void Login::help()
//{
//    about *newabout=new about;
//    newabout->setWindowFlags(Qt::FramelessWindowHint);
//    newabout->setStyleSheet("background-color: transparent;");
//    newabout->show();
//    this->hide();
//    newabout->exec();
//}


void Login::on_usernameinput_textChanged(const QString &arg1)
{
    counter_in_suggestionlist=0;
//    QRegExp RegExp(arg1,Qt::CaseInsensitive, QRegExp::Wildcard);
    ui->SuggestionList->clear();
    for(auto it=users.begin();it!=users.end();it++){
        if(it->contains(arg1)){
            ui->SuggestionList->addItem(*it);
            counter_in_suggestionlist++;
        }
        if(counter_in_suggestionlist==0){
            ui->SuggestionList->hide();
        }
        else{
            ui->SuggestionList->show();
        }
    }




}


void Login::on_SuggestionList_itemDoubleClicked(QListWidgetItem *item)
{
    QString user=item->text();
    int j=0;
     for(auto it=users.begin();it!=users.end();it++){
         if(*it==user){
            break;
         }
         j++;
     }
     counter_in_suggestionlist=-1;
     ui->usernameinput->setText(user);
     ui->passwordinput->setText(passwords[j]);
     ui->SuggestionList->hide();

}


void Login::on_Seepasswordbutton_pressed()
{
    ui->passwordinput->setEchoMode(QLineEdit::Normal);
}


void Login::on_Seepasswordbutton_clicked()
{
    ui->passwordinput->setEchoMode(QLineEdit::Password);
}




void Login::on_LoginButton_pressed()
{
if(isonline){
    if(counts_of_login<=3){
    QUrl url("http://api.barafardayebehtar.ml:8080/login");
        QUrlQuery query;
        query.addQueryItem("username", ui->usernameinput->text());
        query.addQueryItem("password", ui->passwordinput->text());
        url.setQuery(query);
        QNetworkRequest request(url);
        QNetworkReply *reply = manager.get(request);
}
    else{
        generateCaptcha();
        ui->Captcha->show();
        ui->Recaptcha->show();
        ui->RECAPTCHAOK->show();
        if(counts_of_login>=4){
            ui->Recaptcha->setStyleSheet("background-color:rgba(0,0,0,0);"
                                             "border:none;"
                                             "border-bottom: 2px solid rgba(255,0,0,200);"
                                             "color:rgba(0,0,0,240);"
                                             "padding-bottom:7px;");
        }
        ui->Captcha->setText(captcha);

    }}else{
    int i=0;

    for(auto it=users.begin();it!=users.end();it++){
        if(*it==ui->usernameinput->text()&&passwords[i]==ui->passwordinput->text()){
            offlineuser=*it;
            offlineToken=token[i];
            break;
        }
        i++;
    }
    if(offlineuser!=""){
       QFile offlinefile("token.txt");
       offlinefile.open(QIODevice::WriteOnly);

       QTextStream nstream(&offlinefile);
       nstream << offlineToken;
       offlinefile.close();
       QFile newf("user.txt");
       newf.open(QIODevice::WriteOnly);
       QTextStream lstream(&newf);
       lstream<<offlineuser;
       newf.close();
    OflineChat * offlinechat=new OflineChat;
    offlinechat->setWindowFlags(Qt::FramelessWindowHint);
    offlinechat->setStyleSheet("background-color: transparent;");
    offlinechat->show();
    this->hide();
    offlinechat->exec();

}
}

}
void Login::checkifonline()
{

        QNetworkAccessManager *nam=new QNetworkAccessManager;
        QNetworkRequest req(QUrl("http://www.google.com"));
        //sending and recieving requests from server
        QNetworkReply *reply = nam->get(req);
        QEventLoop loop;
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        qDebug()<<reply->bytesAvailable();
        if(reply->bytesAvailable())
        {
            //see if it has bytes(connected)
            ui->status_text_label->setStyleSheet("border-image: url(:/eyeimage/9694699.png);");

            ui->SignUpButton->show();
            isonline=true;
            ui->LoginButton->show();
        }
        else
        {
            qDebug()<<"offline";
            isonline=false;
            ui->SignUpButton->hide();
            ui->LoginButton->hide();
            ui->status_text_label->setStyleSheet("border-image: url(:/eyeimage/9694707.png);");

        }
    }



