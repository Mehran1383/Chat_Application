#include "oflinechat.h"
#include "ui_oflinechat.h"
#include <chat.h>
#include <QShortcut>
#include <QAbstractSlider>
OflineChat::OflineChat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OflineChat)
{
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(exit(1)));
    ui->setupUi(this);
//    token=offlineToken;
    ui->stackedWidget->setCurrentIndex(0);
    ui->flash->hide();
    QTimer *timer=new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(checkifonline()));
    timer->start(1000);
    process();
    connect(ui->PV_ListWidget,&QListWidget::itemClicked,this,&OflineChat::process2);
    connect(ui->Group_ListWidget,&QListWidget::itemClicked,this,&OflineChat::process3);
    connect(ui->Channel_ListWidget,&QListWidget::itemClicked,this,&OflineChat::process4);

    scroll = ui->listView->verticalScrollBar();
    connect(scroll,&QAbstractSlider::sliderReleased,this,&OflineChat::show);
    ui->listView->setItemDelegate(new listviewdelegate());


}

OflineChat::~OflineChat()
{
    delete ui;
}
void OflineChat::checkifonline()
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
            isonline=true;
            Chat *is=new Chat;
            is->setWindowFlags(Qt::FramelessWindowHint);
            is->setStyleSheet("background-color: transparent;");
            is->show();
            this->close();
            is->exec();
        }

    }

void OflineChat::process()
{

    QFile file("username.txt");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream ustream(&file);
    ustream >> User;

    qDebug() << "helloooooo";
    qDebug() << User;
    Root_Address = "C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + User;
    PV_Address = "C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + User + "\\PV";
    GP_Address = "C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + User + "\\GP";
    Channel_Address = "C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + User + "\\Channel";

    PVList1_Address = PV_Address + "\\list1.txt";
    PVList2_Address = PV_Address + "\\list2.txt";

    GPList1_Address = GP_Address + "\\list1.txt";
    GPList2_Address = GP_Address + "\\list2.txt";

    ChList1_Address = Channel_Address + "\\list1.txt";
    ChList2_Address = Channel_Address + "\\list2.txt";




    //------------------------------------------------------------------------------------------------------------------------------------------------------------
    QFile file1(PVList1_Address);
    file1.open(QIODevice::ReadOnly);
    QDataStream ds1(&file1);
    QVariant variant1;
    QVector <QString> vec;

    for(;!ds1.atEnd();)
    {
        ds1 >> variant1;
        QJsonObject obj = variant1.toJsonObject();
        bool check = 0;
        for(auto it = vec.begin(); it!=vec.end();it++){
            if(*it == obj["src"].toString()){
                check = 1;
                break;
            }
        }
        if(check == 0)
        ui->PV_ListWidget->insertItem(0,obj["src"].toString());
        vec.push_back(obj["src"].toString());
    }
    file1.close();
    //-----------------------------------------------
    QFile file2(PVList2_Address);
    file2.open(QIODevice::ReadOnly);
    QDataStream ds2(&file2);
    QVariant variant2;

    for(;!ds2.atEnd();)
    {
        ds2 >> variant2;
        QJsonObject obj = variant2.toJsonObject();
        bool check = 0;
        for(auto it = vec.begin(); it!=vec.end();it++){
            if(*it == obj["src"].toString()){
                check = 1;
                break;
            }
        }
        if(check == 0)
        ui->PV_ListWidget->insertItem(0,obj["src"].toString());
        vec.push_back(obj["src"].toString());
    }
    file2.close();
    //-----------------------------------------------
    //-----------------------------------------------
    QFile file3(GPList1_Address);
    file3.open(QIODevice::ReadOnly);
    QDataStream ds3(&file3);
    QVariant variant3;

    for(;!ds3.atEnd();)
    {
        ds3 >> variant3;
        QJsonObject obj = variant3.toJsonObject();
        bool check = 0;
        for(auto it = vec.begin(); it!=vec.end();it++){
            if(*it == obj["group_name"].toString()){
                check = 1;
                break;
            }
        }
        if(check == 0)
        ui->Group_ListWidget->insertItem(0,obj["group_name"].toString());
        vec.push_back(obj["group_name"].toString());
    }
    file3.close();
    //-----------------------------------------------
    QFile file4(GPList2_Address);
    file4.open(QIODevice::ReadOnly);
    QDataStream ds4(&file4);
    QVariant variant4;

    for(;!ds4.atEnd();)
    {
        ds4 >> variant4;
        QJsonObject obj = variant4.toJsonObject();
        bool check = 0;
        for(auto it = vec.begin(); it!=vec.end();it++){
            if(*it == obj["group_name"].toString()){
                check = 1;
                break;
            }
        }
        if(check == 0)
        ui->Group_ListWidget->insertItem(0,obj["group_name"].toString());
        vec.push_back(obj["group_name"].toString());
    }
    file4.close();
    //-----------------------------------------------
    //-----------------------------------------------
    QFile file5(ChList1_Address);
    file5.open(QIODevice::ReadOnly);
    QDataStream ds5(&file5);
    QVariant variant5;

    for(;!ds5.atEnd();)
    {
        ds5 >> variant5;
        QJsonObject obj = variant5.toJsonObject();
        bool check = 0;
        for(auto it = vec.begin(); it!=vec.end();it++){
            if(*it == obj["channel_name"].toString()){
                check = 1;
                break;
            }
        }
        if(check == 0)
        ui->Channel_ListWidget->insertItem(0,obj["channel_name"].toString());
        vec.push_back(obj["channel_name"].toString());
    }
    file5.close();
    //-----------------------------------------------
    QFile file6(ChList2_Address);
    file6.open(QIODevice::ReadOnly);
    QDataStream ds6(&file6);
    QVariant variant6;



    for(;!ds6.atEnd();)
    {
        ds6 >> variant6;
        QJsonObject obj = variant6.toJsonObject();
        bool check = 0;
        for(auto it = vec.begin(); it!=vec.end();it++){
            if(*it == obj["channel_name"].toString()){
                check = 1;
                break;
            }
        }
        if(check == 0)
        ui->Channel_ListWidget->insertItem(0,obj["channel_name"].toString());
        vec.push_back(obj["channel_name"].toString());
    }
    file6.close();
    //-----------------------------------------------
    //-----------------------------------------------

}

void OflineChat::process2(QListWidgetItem* item)
{
    QAbstractItemModel* model = ui->listView->model();
    if(model){
        int row = model->rowCount();
        for(int i= row -1 ; i>=0 ; i-- ) model->removeRow(i);}

    QString src;
    QString dst;
    QString pm;
    QString date;

    QString name = item->text();
    QString adr = PV_Address + "\\" + name + "\\chat.txt";
    QString num_adr = PV_Address + "\\" + name + "\\msg_num.txt";
    QFile file(adr);
    file.open(QIODevice::ReadOnly);
    QDataStream ds(&file);
    QVariant variant;

    QFile f(num_adr);
    f.open(QIODevice::ReadOnly);
    QTextStream ts(&f);
    int msg_num;
    QString num;
    ts >> num;
    msg_num = num.toInt();


    for(int i = 0;i < msg_num;i++)
    {
        ds >> variant;
        QJsonObject obj = variant.toJsonObject();
        src = obj["src"].toString();
        dst = obj["dst"].toString();
        pm = obj["body"].toString();
        date = obj["date"].toString();

        if(src == User){
            QStandardItem *item1 = new QStandardItem(pm + "\r\n" + date);
            item1->setData("incoming" , Qt::UserRole + 1);
            mymodel.appendRow(item1);
            ui->listView->setModel(&mymodel);
            ui->listView->setItemDelegate(new listviewdelegate());
        }
        else{
            QStandardItem *item1 = new QStandardItem(pm + "\r\n" + date);
            item1->setData("outgoing" , Qt::UserRole + 1);
            mymodel.appendRow(item1);
            ui->listView->setModel(&mymodel);
            ui->listView->setItemDelegate(new listviewdelegate());

        }

    }
    ui->listView->scrollToBottom();
}

void OflineChat::process3(QListWidgetItem *item)
{
    QAbstractItemModel* model = ui->listView->model();
    if(model){
        int row = model->rowCount();
        for(int i= row -1 ; i>=0 ; i-- ) model->removeRow(i);}

    QString src;
    QString dst;
    QString pm;
    QString date;


    QString name = item->text();
    QString adr = GP_Address + "\\" + name + "\\chat.txt";
    QString num_adr = GP_Address + "\\" + name + "\\msg_num.txt";
    QFile file(adr);
    file.open(QIODevice::ReadOnly);
    QDataStream ds(&file);
    QVariant variant;



    QFile f(num_adr);
    f.open(QIODevice::ReadOnly);
    QTextStream ts(&f);
    int msg_num;
    QString num;
    ts >> num;
    msg_num = num.toInt();


    for(int i = 0;i < msg_num;i++)
    {
        ds >> variant;
        QJsonObject obj = variant.toJsonObject();
        src = obj["src"].toString();
        dst = obj["dst"].toString();
        pm = obj["body"].toString();
        date = obj["date"].toString();

        if(src == User){
            QStandardItem *item1 = new QStandardItem(pm + "/n" + date);
            item1->setData("incoming" , Qt::UserRole + 1);
            mymodel.appendRow(item1);
            ui->listView->setModel(&mymodel);
            ui->listView->setItemDelegate(new listviewdelegate());
        }
        else{
                QStandardItem *item1 = new QStandardItem(src +" : " + "\n" + pm + "\n" + date);
                item1->setData("outgoing" , Qt::UserRole + 1);
                mymodel.appendRow(item1);
                ui->listView->setModel(&mymodel);
                ui->listView->setItemDelegate(new listviewdelegate());

            }

    }
    ui->listView->scrollToBottom();
}

void OflineChat::process4(QListWidgetItem *item)
{
    QAbstractItemModel* model = ui->listView->model();
    if(model){
        int row = model->rowCount();
        for(int i= row -1 ; i>=0 ; i-- ) model->removeRow(i);}

    QString src;
    QString dst;
    QString pm;
    QString date;


    QString name = item->text();
    QString adr = Channel_Address + "\\" + name + "\\chat.txt";
    QString num_adr = Channel_Address + "\\" + name + "\\msg_num.txt";
    QFile file(adr);
    file.open(QIODevice::ReadOnly);
    QDataStream ds(&file);
    QVariant variant;

    QFile f(num_adr);
    f.open(QIODevice::ReadOnly);
    QTextStream ts(&f);
    int msg_num;
    QString num;
    ts >> num;
    msg_num = num.toInt();


    for(int i = 0;i < msg_num;i++)
    {
        ds >> variant;
        QJsonObject obj = variant.toJsonObject();
        src = obj["src"].toString();
        dst = obj["dst"].toString();
        pm = obj["body"].toString();
        date = obj["date"].toString();

        if(src == User){
            QStandardItem *item1 = new QStandardItem(pm + "/n" + date);
            item1->setData("incoming" , Qt::UserRole + 1);
            mymodel.appendRow(item1);
            ui->listView->setModel(&mymodel);
            ui->listView->setItemDelegate(new listviewdelegate());
        }
        else{
                QStandardItem *item1 = new QStandardItem(src +" : " + "\n" + pm + "\n" + date);
                item1->setData("outgoing" , Qt::UserRole + 1);
                mymodel.appendRow(item1);
                ui->listView->setModel(&mymodel);
                ui->listView->setItemDelegate(new listviewdelegate());

            }
    }
    ui->listView->scrollToBottom();
}

void OflineChat::on_PV_clicked()
{
    QGraphicsOpacityEffect effect;
    ui->stackedWidget->setGraphicsEffect(&effect);
    QPropertyAnimation animation(&effect,"opacity");
    animation.setDuration(5000);
    animation.setStartValue(1.0);
    animation.setEndValue(0.0);
    animation.start();
    ui->stackedWidget->setCurrentIndex(0);
}

void OflineChat::on_Group_clicked()
{
    QGraphicsOpacityEffect effect;
    ui->stackedWidget->setGraphicsEffect(&effect);
    QPropertyAnimation animation(&effect,"opacity");
    animation.setDuration(5000);
    animation.setStartValue(1.0);
    animation.setEndValue(0.0);
    animation.start();
    ui->stackedWidget->setCurrentIndex(1);
}

void OflineChat::on_Channel_clicked()
{
    QGraphicsOpacityEffect effect;
    ui->stackedWidget->setGraphicsEffect(&effect);
    QPropertyAnimation animation(&effect,"opacity");
    animation.setDuration(5000);
    animation.setStartValue(1.0);
    animation.setEndValue(0.0);
    animation.start();
    ui->stackedWidget->setCurrentIndex(2);
}

void OflineChat::on_flash_clicked()
{
    ui->listView->scrollToBottom();
    ui->flash->hide();

}

void OflineChat::show(){
    if(ui->listView->verticalScrollBar()->value() == ui->listView->verticalScrollBar()->maximum()) ui->flash->hide();
    else ui->flash->show();
}






