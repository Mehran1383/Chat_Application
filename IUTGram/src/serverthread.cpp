#include "serverthread.h"

ServerThread::ServerThread(QObject *parent) : QThread(parent)
{
    QFile file_username;
    QString username;
    file_username.setFileName("username.txt");
    file_username.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream_username(&file_username);
    stream_username >> username;
    file_username.close();

    USERNAME = username;

    ok = true;
    stepU = 1;
    stepG = 1;
    stepC = 1;
    scene = NULL;

}

void ServerThread::set_dst_pv(QListWidgetItem *item)
{
    dst_pv = item->text();
    dst_gp = "";
    dst_channel = "";
    dst = item->text();
    ui->name->setText("Chating with " + dst);
}
void ServerThread::getuserlist()
{
    QNetworkRequest request;
    QUrl url;
    QEventLoop loop;
    QFile file;
    QString token;
    file.setFileName("token.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream >> token;
    file.close();
    url = "http://api.barafardayebehtar.ml:8080/getuserlist?token=" + token;
    request.setUrl(url);
    Thread_netManager->get(request);
    connect(this,&ServerThread::userlist_done,&loop,&QEventLoop::quit);
    loop.exec();
}
void ServerThread::getuserchats()
{
    QNetworkRequest request;
    QUrl url;
    QEventLoop loop;
    QFile file;
    QString token;
    file.setFileName("token.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream >> token;
    file.close();
    url = "http://api.barafardayebehtar.ml:8080/getuserchats?token=" + token + "&dst=" + dst_pv;
    request.setUrl(url);
    Thread_netManager->get(request);
    connect(this,&ServerThread::userchats_done,&loop,&QEventLoop::quit);
    loop.exec();
}
void ServerThread::send_message_user()
{

    QNetworkRequest request;
    QUrl url;
    QEventLoop loop;
    QFile file;
    QString token;
    file.setFileName("token.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream >> token;
    file.close();
    url = "http://api.barafardayebehtar.ml:8080/sendmessageuser?token=" + token + "&dst=" + dst + "&body=" + ui->chatbar->text();
    ui->chatbar->setText("");
    request.setUrl(url);
    Send_User->get(request);
}
void ServerThread::start_message_with_user()
{
    QNetworkRequest request;
    QUrl url;
    QEventLoop loop;
    QFile file;
    QString token;
    file.setFileName("token.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream >> token;
    file.close();
    url = "http://api.barafardayebehtar.ml:8080/sendmessageuser?token=" + token + "&dst=" + ui->PV_usernamelabel_startchat->text() + "&body=" + ui->chatbar->text();
    request.setUrl(url);
    startchat->get(request);
}

void ServerThread::replyFinished_user(QNetworkReply *reply)
{

    QString message;
    QJsonDocument jsdoc;
    jsdoc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsobj;
    jsobj = jsdoc.object();
    QString pm = jsobj["message"].toString();
    QString messageNumber;
    int j = 0;
    for(int i = 20;pm[i] != '-';i++ , j++)
    {
        messageNumber[j] = pm[i];
    }
    int msgCount = messageNumber.toInt();

        for(int i = 0;i < msgCount;i++)
        {
            if(msgCount != user_num && stepU == 1)
            {
                message = (jsobj[QString("block %1").arg(i)].toObject()["src"]).toString();



                QDir direction;
                QString fullAddress = "C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + USERNAME + "\\PV\\";
                fullAddress += message;
                direction.mkpath(fullAddress);

                QFile file("C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + USERNAME + "\\PV\\list1.txt");
                file.open(QIODevice::Append);
                QDataStream dstream(&file);
                dstream << QJsonDocument(jsobj[QString("block %1").arg(i)].toObject()).toVariant();
                file.close();

                QListWidgetItem* list = new QListWidgetItem;
                list->setText(message);
                UserList.push_back(*list);
                ui->PV_ListWidget->insertItem(0,list);
            }
            else if(msgCount != user_num && stepU != 1)
            {
                message = (jsobj[QString("block %1").arg(msgCount - 1)].toObject()["src"]).toString();


                QDir direction;
                QString fullAddress = "C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + USERNAME + "\\PV\\";
                fullAddress += message;
                direction.mkpath(fullAddress);

                QFile file("C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + USERNAME + "\\PV\\list2.txt");
                file.open(QIODevice::Append);
                QDataStream dstream(&file);
                dstream << QJsonDocument(jsobj[QString("block %1").arg(i)].toObject()).toVariant();
                file.close();

                QListWidgetItem* list = new QListWidgetItem;
                list->setText(message);
                UserList.push_back(*list);
                ui->PV_ListWidget->insertItem(0,list);
                break;
            }
        }
        user_num = msgCount;
        stepU++;
        emit userlist_done();
}
void ServerThread::replyFinished_user_chats(QNetworkReply *reply)
{
    QString message;
    QString source;
    QString dst;
    QString date;
    QJsonDocument jsdoc;
    jsdoc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsobj;
    jsobj = jsdoc.object();
    QString pm = jsobj["message"].toString();
    QString messageNumber;
    int j = 0;
    for(int i = 11;pm[i] != '-';i++ , j++)
    {
        messageNumber[j] = pm[i];
    }
    int msg_num = messageNumber.toInt();
    User_Message_Number[dst_pv] = msg_num;
    if(msg_num == 0)
    {
        QStandardItem *item1 = new QStandardItem("There is no message . . .");
        item1->setData("incoming" , Qt::UserRole + 1);
        mymodel.appendRow(item1);
        ui->listview->setModel(&mymodel);
        ui->listview->setItemDelegate(new listviewdelegate());
    }

    {
        int i = 0;
        QString x = (jsobj[QString("block %1").arg(i)].toObject()["dst"]).toString();

        QFile file("C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + USERNAME + "\\PV\\" + x + "\\msg_num.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream tstream(&file);
        tstream << msg_num;
        file.close();
    }

    QAbstractItemModel* model = ui->listview->model();
    if(model){
        int row = model->rowCount();
        for(int i= row -1 ; i>=0 ; i-- ) model->removeRow(i);}


        for(int i = 0;i < msg_num;i++)
        {
            message = (jsobj[QString("block %1").arg(i)].toObject()["body"]).toString();
            source = (jsobj[QString("block %1").arg(i)].toObject()["src"]).toString();
            dst = (jsobj[QString("block %1").arg(i)].toObject()["dst"]).toString();
            date = (jsobj[QString("block %1").arg(i)].toObject()["date"]).toString();

            if(source == USERNAME){
                QStandardItem *item1 = new QStandardItem(message+ "\r\n" + date);
                item1->setData("incoming" , Qt::UserRole + 1);
                mymodel.appendRow(item1);
                ui->listview->setModel(&mymodel);
                listviewdelegate * list = new listviewdelegate();
                list->SetFont(mainchat_pointer->font);
                list->setSize(mainchat_pointer->size);
                ui->listview->setItemDelegate(list);
            }
            else{
                QStandardItem *item1 = new QStandardItem(message + "\r\n" + date);
                item1->setData("outgoing" , Qt::UserRole + 1);
                mymodel.appendRow(item1);
                ui->listview->setModel(&mymodel);
                listviewdelegate * list = new listviewdelegate();
                list->SetFont(mainchat_pointer->font);
                list->setSize(mainchat_pointer->size);
                ui->listview->setItemDelegate(list);

            }


            QString fullAddress = "C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + USERNAME + "\\PV\\" + dst + "\\chat.txt";
            QFile file(fullAddress);
            file.open(QIODevice::Append);
            QDataStream dstream(&file);
            dstream << QJsonDocument(jsobj[QString("block %1").arg(i)].toObject()).toVariant();
            file.close();



        }


        emit userchats_done();
}



void ServerThread::set_dst_gp(QListWidgetItem *item)
{
    dst_gp = item->text();
    dst_channel = "";
    dst_pv = "";
    dst = item->text();
    ui->name->setText("Chat in " +dst);
}

void ServerThread::set_dst_channel(QListWidgetItem *item)
{
    dst_channel = item->text();
    dst_gp = "";
    dst_pv = "";
    dst = item->text();
    ui->name->setText("Chat in " +dst);
}



void ServerThread::getgrouplist()
{
    QNetworkRequest request;
    QUrl url;
    QEventLoop loop;
    QFile file;
    QString token;
    file.setFileName("token.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream >> token;
    file.close();

    url = "http://api.barafardayebehtar.ml:8080/getgrouplist?token=" + token;
    request.setUrl(url);

    Thread_netManager->get(request);
    connect(this,&ServerThread::grouplist_done,&loop,&QEventLoop::quit);
    loop.exec();
}

void ServerThread::getchannellist()
{
    QNetworkRequest request;
    QUrl url;
    QEventLoop loop;
    QFile file;
    QString token;
    file.setFileName("token.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream >> token;
    file.close();

    url = "http://api.barafardayebehtar.ml:8080/getchannellist?token=" + token;
    request.setUrl(url);

    Thread_netManager->get(request);
    connect(this,&ServerThread::channellist_done,&loop,&QEventLoop::quit);
    loop.exec();
}



void ServerThread::getgroupchats()
{
    QNetworkRequest request;
    QUrl url;
    QEventLoop loop;
    QFile file;
    QString token;
    file.setFileName("token.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream >> token;
    file.close();

    url = "http://api.barafardayebehtar.ml:8080/getgroupchats?token=" + token + "&dst=" + dst_gp;
    request.setUrl(url);

    Thread_netManager->get(request);
    connect(this,&ServerThread::groupchats_done,&loop,&QEventLoop::quit);
    loop.exec();
}

void ServerThread::getchannelchats()
{
    QNetworkRequest request;
    QUrl url;
    QEventLoop loop;
    QFile file;
    QString token;
    file.setFileName("token.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream >> token;
    file.close();
    url = "http://api.barafardayebehtar.ml:8080/getchannelchats?token=" + token + "&dst=" + dst_channel;
    request.setUrl(url);

    Thread_netManager->get(request);
    connect(this,&ServerThread::channelchats_done,&loop,&QEventLoop::quit);
    loop.exec();
}



void ServerThread::send_message_gp()
{
    QNetworkRequest request;
    QUrl url;
    QEventLoop loop;
    QFile file;
    QString token;
    file.setFileName("token.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream >> token;
    file.close();

    url = "http://api.barafardayebehtar.ml:8080/sendmessagegroup?token=" + token + "&dst=" + dst + "&body=" + ui->chatbar->text();
    ui->chatbar->setText("");
    request.setUrl(url);

    Send_Group->get(request);
}

void ServerThread::send_message_channel()
{
    QNetworkRequest request;
    QUrl url;
    QEventLoop loop;
    QFile file;
    QString token;
    file.setFileName("token.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream >> token;
    file.close();

    url = "http://api.barafardayebehtar.ml:8080/sendmessagechannel?token=" + token + "&dst=" + dst + "&body=" + ui->chatbar->text();
    ui->chatbar->setText("");
    request.setUrl(url);

    Send_Channel->get(request);
}

void ServerThread::send()
{
    if(dst_pv != "")
     send_message_user();
    if(dst_gp != "")
     send_message_gp();
    if(dst_channel != "")
     send_message_channel();
}

void ServerThread::make_gp()
{
    QNetworkRequest request;
    QUrl url;
    QEventLoop loop;
    QFile file;
    QString token;
    file.setFileName("token.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream >> token;
    file.close();
    url = "http://api.barafardayebehtar.ml:8080/creategroup?token=" + token + "&group_name=" + ui->make_id_2->text() + "&group_title=" ;
    request.setUrl(url);
    MakeGroup->get(request);
}

void ServerThread::make_channel()
{
    QNetworkRequest request;
    QUrl url;
    QEventLoop loop;
    QFile file;
    QString token;
    file.setFileName("token.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream >> token;
    file.close();

    url = "http://api.barafardayebehtar.ml:8080/createchannel?token=" + token + "&channel_name=" + ui->make_id->text() + "&channel_title=";
    request.setUrl(url);

    MakeChannel->get(request);
}

void ServerThread::join_gp()
{
    QNetworkRequest request;
    QUrl url;
    QEventLoop loop;
    QFile file;
    QString token;
    file.setFileName("token.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream >> token;
    file.close();

    url = "http://api.barafardayebehtar.ml:8080/joingroup?token=" + token + "&group_name=" + ui->make_id_4->text();
    request.setUrl(url);

    JoinGroup->get(request);
}

void ServerThread::join_channel()
{
    QNetworkRequest request;
    QUrl url;
    QEventLoop loop;
    QFile file;
    QString token;
    file.setFileName("token.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream >> token;
    file.close();
    url = "http://api.barafardayebehtar.ml:8080/joinchannel?token=" + token + "&channel_name=" + ui->make_id_3->text();
    request.setUrl(url);

    JoinChannel->get(request);
}



void ServerThread::replyFinished_group(QNetworkReply *reply)
{
    QString message;
    QJsonDocument jsdoc;
    jsdoc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsobj;
    jsobj = jsdoc.object();
    QString msg = jsobj["message"].toString();
    QString messageNumber;
    int j = 0;
    for(int i = 12;msg[i] != '-';i++ , j++)
    {
        messageNumber[j] = msg[i];
    }
    int msg_num = messageNumber.toInt();



        for(int i = 0;i < msg_num;i++)
        {
           if(msg_num != group_num && stepG == 1)
           {
                message = (jsobj[QString("block %1").arg(i)].toObject()["group_name"]).toString();


                QDir direction;
                QString fullAddress = "C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + USERNAME + "\\GP\\";
                fullAddress += message;
                direction.mkpath(fullAddress);

                QFile file("C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + USERNAME + "\\GP\\list1.txt");
                file.open(QIODevice::Append);
                QDataStream dstream(&file);
                dstream << QJsonDocument(jsobj[QString("block %1").arg(i)].toObject()).toVariant();
                file.close();

                    QListWidgetItem* list = new QListWidgetItem;
                    list->setToolTip(message);
                    list->setText(message);
                    GroupList.push_back(*list);
                    ui->Group_ListWidget->insertItem(0,list);
           }
           else if(msg_num != group_num && stepG != 1)
           {
               message = (jsobj[QString("block %1").arg(msg_num - 1)].toObject()["group_name"]).toString();


               QDir direction;
               QString fullAddress = "C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + USERNAME + "\\GP\\";
               fullAddress += message;
               direction.mkpath(fullAddress);

               QFile file("C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + USERNAME + "\\GP\\list2.txt");
               file.open(QIODevice::Append);
               QDataStream dstream(&file);
               dstream << QJsonDocument(jsobj[QString("block %1").arg(i)].toObject()).toVariant();
               file.close();


               QListWidgetItem* list = new QListWidgetItem;
               list->setText(message);
               GroupList.push_back(*list);
               ui->Group_ListWidget->insertItem(0,list);
               break;
           }

        }
    group_num = msg_num;
    stepG++;
    emit grouplist_done();
}

void ServerThread::replyFinished_channel(QNetworkReply *reply)
{
    QString message;
    QJsonDocument jsdoc;
    jsdoc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsobj;
    jsobj = jsdoc.object();
    QString msg = jsobj["message"].toString();
    QString messageNum;
    int j = 0;
    for(int i = 12;msg[i] != '-';i++ , j++)
    {
        messageNum[j] = msg[i];
    }
    int msg_num = messageNum.toInt();
        for(int i = 0;i < msg_num;i++)
        {
            if(msg_num != channel_num && stepC == 1)
            {
                message = (jsobj[QString("block %1").arg(i)].toObject()["channel_name"]).toString();


                QDir direction;
                QString fullAddress = "C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + USERNAME + "\\Channel\\";
                fullAddress += message;
                direction.mkpath(fullAddress);

                QFile file("C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + USERNAME + "\\Channel\\list1.txt");
                file.open(QIODevice::Append);
                QDataStream dstream(&file);
                dstream << QJsonDocument(jsobj[QString("block %1").arg(i)].toObject()).toVariant();
                file.close();

                QListWidgetItem* list = new QListWidgetItem;
                list->setToolTip("Channel name is " + message);
                list->setText(message);
                ChannelList.push_back(*list);
                ui->Channel_ListWidget->insertItem(0,list);
            }
            else if(msg_num != channel_num && stepC != 1)
            {
                message = (jsobj[QString("block %1").arg(msg_num - 1)].toObject()["channel_name"]).toString();


                QDir direction;
                QString fullAddress = "C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + USERNAME + "\\Channel\\";
                fullAddress += message;
                direction.mkpath(fullAddress);

                QFile file("C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + USERNAME + "\\Channel\\list2.txt");
                file.open(QIODevice::Append);
                QDataStream dstream(&file);
                dstream << QJsonDocument(jsobj[QString("block %1").arg(i)].toObject()).toVariant();
                file.close();



                QListWidgetItem* list = new QListWidgetItem;
                list->setText(message);
                ChannelList.push_back(*list);
                ui->Channel_ListWidget->insertItem(0,list);
                break;
            }


        }
    channel_num = msg_num;
    stepC++;
    emit channellist_done();
}


void ServerThread::replyFinished_group_chats(QNetworkReply *reply)
{
    QString message;
    QString src;
    QString dst;
    QString date;
    QJsonDocument jsdoc;
    jsdoc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsobj;
    jsobj = jsdoc.object();
    QString msg = jsobj["message"].toString();
    QString messageNum;
    int j = 0;
    for(int i = 11;msg[i] != '-';i++ , j++)
    {
        messageNum[j] = msg[i];
    }
    int msg_num = messageNum.toInt();
    Group_Message_Number[dst_gp] = msg_num;
        if(msg_num == 0)
        {

            QStandardItem *item1 = new QStandardItem("There is no message . . .");
            item1->setData("incoming" , Qt::UserRole + 1);
            mymodel.appendRow(item1);
            ui->listview->setModel(&mymodel);
            ui->listview->setItemDelegate(new listviewdelegate());
        }

        QFile file("C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + USERNAME + "\\GP\\" + dst_gp + "\\msg_num.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream tstream(&file);
        tstream << msg_num;
        file.close();

        QAbstractItemModel* model = ui->listview->model();
        if(model){
            int row = model->rowCount();
            for(int i= row -1 ; i>=0 ; i-- ) model->removeRow(i);}



        for(int i = 0;i < msg_num;i++)
        {
            message = (jsobj[QString("block %1").arg(i)].toObject()["body"]).toString();
            src = (jsobj[QString("block %1").arg(i)].toObject()["src"]).toString();
            dst = (jsobj[QString("block %1").arg(i)].toObject()["dst"]).toString();
            date = (jsobj[QString("block %1").arg(i)].toObject()["date"]).toString();

            if(src == USERNAME){
                QStandardItem *item1 = new QStandardItem(message + "/n" + date);
                item1->setData("incoming" , Qt::UserRole + 1);
                mymodel.appendRow(item1);
                ui->listview->setModel(&mymodel);
                listviewdelegate * list = new listviewdelegate();
                list->SetFont(mainchat_pointer->font);
                list->setSize(mainchat_pointer->size);
                ui->listview->setItemDelegate(list);
            }
            else{
                    QStandardItem *item1 = new QStandardItem(src +" : " + "\n" + message + "\n" + date);
                    item1->setData("outgoing" , Qt::UserRole + 1);
                    mymodel.appendRow(item1);
                    ui->listview->setModel(&mymodel);
                    listviewdelegate * list = new listviewdelegate();
                    list->SetFont(mainchat_pointer->font);
                    list->setSize(mainchat_pointer->size);
                    ui->listview->setItemDelegate(list);

                }


            QString fullAddress = "C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + USERNAME + "\\GP\\" + dst + "\\chat.txt";
            QFile file(fullAddress);
            file.open(QIODevice::Append);
            QDataStream dstream(&file);
            dstream << QJsonDocument(jsobj[QString("block %1").arg(i)].toObject()).toVariant();
            file.close();




        }
        emit groupchats_done();


}

void ServerThread::replyFinished_channel_chats(QNetworkReply *reply)
{
    QString message;
    QString src;
    QString dst;
    QString date;
    QJsonDocument jsdoc;
    jsdoc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsobj;
    jsobj = jsdoc.object();
    QString msg = jsobj["message"].toString();
    QString messageNum;
    int j = 0;

    for(int i = 11;msg[i] != '-';i++ , j++)
    {
        messageNum[j] = msg[i];
    }
    int msg_num = messageNum.toInt();
    Channel_Message_Number[dst_channel] = msg_num;
        if(msg_num == 0)
        {
            QStandardItem *item1 = new QStandardItem("There is no message . . .");
            item1->setData("incoming" , Qt::UserRole + 1);
            mymodel.appendRow(item1);
            ui->listview->setModel(&mymodel);
            ui->listview->setItemDelegate(new listviewdelegate());
        }


        QFile file("C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + USERNAME + "\\Channel\\" + dst_channel + "\\msg_num.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream tstream(&file);
        tstream << msg_num;
        file.close();

        QAbstractItemModel* model = ui->listview->model();
        if(model){
            int row = model->rowCount();
            for(int i= row -1 ; i>=0 ; i-- ) model->removeRow(i);}



        for(int i = 0;i < msg_num;i++)
        {
            message = (jsobj[QString("block %1").arg(i)].toObject()["body"]).toString();
            src = (jsobj[QString("block %1").arg(i)].toObject()["src"]).toString();
            dst = (jsobj[QString("block %1").arg(i)].toObject()["dst"]).toString();
            date = (jsobj[QString("block %1").arg(i)].toObject()["date"]).toString();

            if(src == USERNAME){

                QStandardItem *item1 = new QStandardItem(message+ "\n" + date);
                item1->setData("incoming" , Qt::UserRole + 1);
                mymodel.appendRow(item1);
                ui->listview->setModel(&mymodel);
                listviewdelegate * list = new listviewdelegate();
                list->SetFont(mainchat_pointer->font);
                list->setSize(mainchat_pointer->size);
                ui->listview->setItemDelegate(list);
            }

            else{
                    QStandardItem *item1 = new QStandardItem(src + " : " + "\n" + message + "\n" + date);
                    item1->setData("outgoing" , Qt::UserRole + 1);
                    mymodel.appendRow(item1);
                    ui->listview->setModel(&mymodel);
                    listviewdelegate * list = new listviewdelegate();
                    list->SetFont(mainchat_pointer->font);
                    list->setSize(mainchat_pointer->size);
                    ui->listview->setItemDelegate(list);
                }
            QString fullAddress = "C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\users\\" + USERNAME + "\\Channel\\" + dst + "\\chat.txt";
            QFile file(fullAddress);
            file.open(QIODevice::Append);
            QDataStream dstream(&file);
            dstream << QJsonDocument(jsobj[QString("block %1").arg(i)].toObject()).toVariant();
            file.close();

        }
        emit channelchats_done();
}

void ServerThread::run()
{

    QEventLoop my_loop;
    Thread_netManager = new QNetworkAccessManager(this);
    Send_User = new QNetworkAccessManager(this);
    Send_Group = new QNetworkAccessManager(this);
    Send_Channel = new QNetworkAccessManager(this);

    JoinChannel = new QNetworkAccessManager(this);
    JoinGroup = new QNetworkAccessManager(this);
    MakeChannel = new QNetworkAccessManager(this);
    MakeGroup = new QNetworkAccessManager(this);

    startchat = new QNetworkAccessManager(this);

    while(ok)
    {
    connect(Thread_netManager, &QNetworkAccessManager::finished,this, &ServerThread::replyFinished_user);
        getuserlist();
        QTimer::singleShot( 200,&my_loop,SLOT(quit()) );
        my_loop.exec();
    disconnect(Thread_netManager, &QNetworkAccessManager::finished,this, &ServerThread::replyFinished_user);
    //------------------------------------------------------------------------------------------------------
    qDebug() << "Disconnected\n";
    connect(Thread_netManager, &QNetworkAccessManager::finished,this, &ServerThread::replyFinished_group);
    qDebug() << "Connected\n";
        getgrouplist();
        QTimer::singleShot( 200,&my_loop,SLOT(quit()) );
        my_loop.exec();
    disconnect(Thread_netManager, &QNetworkAccessManager::finished,this, &ServerThread::replyFinished_group);
    //------------------------------------------------------------------------------------------------------
    qDebug() << "Disconnected\n";
    connect(Thread_netManager, &QNetworkAccessManager::finished,this, &ServerThread::replyFinished_channel);
    qDebug() << "Connected\n";
        getchannellist();
        QTimer::singleShot( 200,&my_loop,SLOT(quit()) );
        my_loop.exec();
    disconnect(Thread_netManager, &QNetworkAccessManager::finished,this, &ServerThread::replyFinished_channel);
    //------------------------------------------------------------------------------------------------------
    connect(Thread_netManager,&QNetworkAccessManager::finished,this,&ServerThread::replyFinished_user_chats);
    if(dst_pv.isEmpty())
    {

    }
    else
    {
        getuserchats();
        QTimer::singleShot( 200,&my_loop,SLOT(quit()) );
        my_loop.exec();
    }
    disconnect(Thread_netManager,&QNetworkAccessManager::finished,this,&ServerThread::replyFinished_user_chats);
    //---------------------------------------------------------------------------------------------------------
    connect(Thread_netManager,&QNetworkAccessManager::finished,this,&ServerThread::replyFinished_group_chats);
    if(dst_gp.isEmpty())
    {

    }
    else
    {
        getgroupchats();
    }
    disconnect(Thread_netManager,&QNetworkAccessManager::finished,this,&ServerThread::replyFinished_group_chats);
    //---------------------------------------------------------------------------------------------------------
    connect(Thread_netManager,&QNetworkAccessManager::finished,this,&ServerThread::replyFinished_channel_chats);
    if(dst_channel.isEmpty())
    {

    }
    else
    {
        getchannelchats();
    }
    disconnect(Thread_netManager,&QNetworkAccessManager::finished,this,&ServerThread::replyFinished_channel_chats);
    //---------------------------------------------------------------------------------------------------------




    }

}

