#ifndef OFLINECHAT_H
#define OFLINECHAT_H

#include <QDialog>
#include <QFile>
#include <QDir>
#include <QDataStream>
#include <QVariant>
#include <QListWidget>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardItemModel>
#include <QScrollBar>
namespace Ui {
class OflineChat;
}

class OflineChat : public QDialog
{
    Q_OBJECT

public:
    bool isonline=false;
    explicit OflineChat(QWidget *parent = nullptr);
    ~OflineChat();
    QString token;
    QString User;
    QAbstractSlider *scroll;
    void show();
public slots:
    void process();
    void process2(QListWidgetItem *item);
    void process3(QListWidgetItem *item);
    void process4(QListWidgetItem *item);

public:
    QString Root_Address;
    QString PV_Address;
    QString GP_Address;
    QString Channel_Address;

    QString PVList1_Address;
    QString PVList2_Address;

    QString GPList1_Address;
    QString GPList2_Address;

    QString ChList1_Address;
    QString ChList2_Address;


private slots:
    void checkifonline();

    void on_PV_clicked();

    void on_Group_clicked();

    void on_Channel_clicked();

    void on_flash_clicked();

private:
    Ui::OflineChat *ui;
    QStandardItemModel mymodel;

signals:
    void done();



};

#endif // OFLINECHAT_H
