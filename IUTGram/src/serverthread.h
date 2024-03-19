#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QHash>
#include <QMap>
#include <QObject>
#include <QThread>
#include "listviewdelegate.h"
#include "ui_chat.h"
#include "chat.h"
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTimer>
#include <QVector>
#include <QDir>
#include <QGraphicsTextItem>
#include <map>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDataStream>
#include <QListView>
class Chat;
class ServerThread;

class ServerThread : public QThread
{
    Q_OBJECT
public:
    void getuserlist();
    void getgrouplist();
    void getchannellist();
    void getuserchats();
    void getgroupchats();
    void getchannelchats();
    void send_message_user();
    void send_message_gp();
    void send_message_channel();

    void send();

    void make_gp();
    void make_channel();
    void join_gp();
    void join_channel();

    void start_message_with_user();





    Ui_Chat *ui;
    Chat* mainchat_pointer;

    explicit ServerThread(QObject *parent = nullptr);

    void set_dst_pv(QListWidgetItem* item);
    void set_dst_gp(QListWidgetItem* item);
    void set_dst_channel(QListWidgetItem* item);

    bool ok;
signals:
    void userlist_done();
    void grouplist_done();
    void channellist_done();
    void userchats_done();
    void groupchats_done();
    void channelchats_done();





public slots:
    void replyFinished_user(QNetworkReply *reply);//list
    void replyFinished_group(QNetworkReply *reply);//list
    void replyFinished_channel(QNetworkReply *reply);//list
    //------------------------------------------------
    void replyFinished_user_chats(QNetworkReply *reply);
    void replyFinished_group_chats(QNetworkReply *reply);
    void replyFinished_channel_chats(QNetworkReply *reply);
    //------------------------------------------------------
private:
    void run();
    int stepU;
    int stepG;
    int stepC;
    QMap<QString,QVector<QString>> allmessages;
    QNetworkAccessManager* Thread_netManager;

    QNetworkAccessManager* Send_User;
    QNetworkAccessManager* Send_Group;
    QNetworkAccessManager* Send_Channel;

    QNetworkAccessManager* JoinChannel;
    QNetworkAccessManager* JoinGroup;
    QNetworkAccessManager* MakeChannel;
    QNetworkAccessManager* MakeGroup;
    QNetworkAccessManager* startchat;


    QVector <QListWidgetItem> UserList;
    int user_num;
    QMap <QString , int> User_Message_Number;

    QVector <QListWidgetItem> GroupList;
    int group_num;
    QMap <QString , int> Group_Message_Number;

    QVector <QListWidgetItem> ChannelList;
    int channel_num;
    QMap <QString , int> Channel_Message_Number;

    QString dst;
    QString dst_pv;
    QString dst_gp;
    QString dst_channel;

    QGraphicsScene* scene;
    QString USERNAME;

    QStandardItemModel mymodel;







};

#endif // SERVERTHREAD_H
