#ifndef CHAT_H
#define CHAT_H

#include <QDialog>
#include<serverthread.h>
#include <QMovie>
#include <QScrollBar>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include "listviewdelegate.h"
#include "addphoto.h"
#include "logout.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QCheckBox>
namespace Ui {
class Chat;
}
class ServerThread;
class Chat : public QDialog
{
    Q_OBJECT

public:
    ServerThread* SThread;
    explicit Chat(QWidget *parent = nullptr);
    ~Chat();
    void show();
    QAbstractSlider *scroll;
    void on_emojiButton_clicked();
    QString font;
    int size;
    void keyPressEvent(QKeyEvent *event);
    QCheckBox* check;
    QPixmap *myIcon;

private slots:
    void on_PV_clicked();

    void on_Group_clicked();

    void on_Channel_clicked();

    void on_flash_clicked();

    void emojiProc(QListWidgetItem *item);

    void on_icon_clicked();

    void _itemClickedProc(QListWidgetItem *item);

    void on_photo_clicked();

    void on_quit_clicked();

    void on_them_clicked();

    void on_theme1_clicked();

    void on_theme2_clicked();

    void on_theme3_clicked();

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_save_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_out_clicked();

private:
    QString Usertoken;
    void checkifonline();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int mouseClickMove_X;
    int mouseClickMove_Y;
    bool is_emoTable_open = false;
    Ui::Chat *ui;

signals:
    void _itemClicked(QListWidgetItem *item);


};

#endif // CHAT_H
