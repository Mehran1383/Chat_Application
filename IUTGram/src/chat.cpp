#include "chat.h"
#include "ui_chat.h"
#include<login.h>
#include<QThread>
#include<serverthread.h>
#include<QShortcut>
Chat::Chat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Chat)
{
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(close()));
    ui->setupUi(this);
    ui->listWidget->hide();
    ui->flash->hide();
    ui->stackedWidget->setCurrentIndex(0);
    ui->font->setCurrentIndex(0);
    SThread = new ServerThread;
    SThread->ui = ui;
    SThread->mainchat_pointer = this;
    SThread->start();
    connect(ui->PV_ListWidget,&QListWidget::itemClicked,SThread,&ServerThread::set_dst_pv);
    connect(ui->Group_ListWidget,&QListWidget::itemClicked,SThread,&ServerThread::set_dst_gp);
    connect(ui->Channel_ListWidget,&QListWidget::itemClicked,SThread,&ServerThread::set_dst_channel);
    connect(ui->sendmessage,&QPushButton::clicked,SThread,&ServerThread::send);
    connect(ui->PV_Options,&QPushButton::clicked,SThread,&ServerThread::start_message_with_user);
    connect(ui->GP_Join,&QPushButton::clicked,SThread,&ServerThread::join_gp);
    connect(ui->GP_Make,&QPushButton::clicked,SThread,&ServerThread::make_gp);
    connect(ui->Channel_Join,&QPushButton::clicked,SThread,&ServerThread::join_channel);
    connect(ui->Channel_Create,&QPushButton::clicked,SThread,&ServerThread::make_channel);

    scroll = ui->listview->verticalScrollBar();
    connect(scroll,&QAbstractSlider::sliderReleased,this,&Chat::show);
    ui->listview->setItemDelegate(new listviewdelegate());
    connect(ui->listWidget, &QListWidget::itemClicked, this, &Chat::_itemClickedProc);

    QListWidgetItem *item;
    QFont tempFont;
    QFile emojiFile(":/emoji_dataset.txt");
    emojiFile.open(QIODevice::ReadOnly | QIODevice::Text);

    QString emoji;
    while (!emojiFile.atEnd())
    {
        emoji = emojiFile.readLine().trimmed();

        item = new QListWidgetItem(emoji);
        item->setTextAlignment(Qt::AlignHCenter);
        tempFont = item->font();
        tempFont.setPixelSize(25);
        item->setFont(tempFont);
        ui->listWidget->addItem(item);
    }

    emojiFile.close();
    QFile user("username.txt");
    user.open(QFile::ReadOnly | QFile::Text);
    QString name;
    QTextStream nstream(&user);
    nstream >> name;
    ui->user->setText("Hi " + name);

    QPixmap img("C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\photos\\"+ name + ".jpg");
    QPixmap img2 = img.scaled(95,85,Qt::KeepAspectRatio);
    if(!img2.isNull()){
        ui->profile->setPixmap(img2);
    }

    check = new QCheckBox();
    check->setText("Dont show this message again");
    myIcon = new QPixmap (":/eyeimage/leave_icon.png");



}



Chat::~Chat()
{
    delete ui;
}
void Chat::mousePressEvent(QMouseEvent *event)
{
    mouseClickMove_X = event->x();
    mouseClickMove_Y = event->y();
}

void Chat::mouseMoveEvent(QMouseEvent *event)
{
    //moving the screen when pressing 2
    move(event->globalX()-mouseClickMove_X,event->globalY()-mouseClickMove_Y);
}



void Chat::on_PV_clicked()
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

void Chat::on_Group_clicked()
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

void Chat::on_Channel_clicked()
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

void Chat::on_flash_clicked()
{
    ui->listview->scrollToBottom();
    ui->flash->hide();

}

void Chat::show(){
    if(ui->listview->verticalScrollBar()->value() == ui->listview->verticalScrollBar()->maximum()) ui->flash->hide();
    else ui->flash->show();
}

void Chat::on_icon_clicked()
{
    if (is_emoTable_open)
    {
        ui->listWidget->hide();

        is_emoTable_open = false;
    }
    else{

    connect(this , &Chat::_itemClicked , this , &Chat::emojiProc);
    ui->listWidget->show();
    is_emoTable_open = true;}
}

void Chat::emojiProc(QListWidgetItem *item)
{
    QString temp = ui->chatbar->text();
    temp += item->text();
    ui->chatbar->setText(temp);
}

void Chat::_itemClickedProc(QListWidgetItem *item)
{
    emit _itemClicked(item);
}



void Chat::on_photo_clicked()
{
    AddPhoto* photo = new AddPhoto;
    photo->show();
    this->hide();
    photo->setWindowFlags(Qt::FramelessWindowHint);
    photo->exec();
}

void Chat::on_quit_clicked()
{
    exit(0);
}

void Chat::on_them_clicked()
{
ui->stackedWidget->setCurrentIndex(3);

}
void Chat::on_theme1_clicked()
{
    ui->listview->setStyleSheet("border-image: url(:/eyeimage/wp6028915-whatsapp-chat-iphone-wallpapers.jpg);\
                                border-radius : 20px;\
                                border:20px solid;");


}

void Chat::on_theme2_clicked()
{
    ui->listview->setStyleSheet("border-image: url(:/eyeimage/jt4AoG.jpg);\
                                border-radius : 20px;\
                                border:20px solid;");

}

void Chat::on_theme3_clicked()
{
    ui->listview->setStyleSheet("border-image: url(:/eyeimage/wp10254494-whatsapp-chat-wallpapers.jpg);\
                                border-radius : 20px;\
                                border:20px solid;");

}

void Chat::on_fontComboBox_currentFontChanged(const QFont &f)
{
    ui->text->setFont(f);
}

void Chat::on_save_clicked()
{
    font = ui->fontComboBox->currentFont().toString();
    size = ui->spinBox->value();

}

void Chat::on_spinBox_valueChanged(int arg1)
{
    QFont f(ui->fontComboBox->currentFont().toString(),arg1);
    ui->text->setFont(f);
}


void Chat::on_out_clicked()
{
    Logout* out = new Logout();
    out->show();
    out->exec();
}

void Chat::keyPressEvent(QKeyEvent *event){
    QFile file("check.txt");
    QString st;
    if(file.open(QFile::ReadOnly | QFile::Text)){
       QTextStream str(&file);
       str >> st;
       file.close();
    }

        if(event->key() == Qt::Key_Escape && st!="1"){
        QMessageBox *message = new QMessageBox;

        message->setCheckBox(check);
        message->setWindowTitle("Close the IUTGram");
        message->setIcon(QMessageBox::Question);
        message->setInformativeText(tr("Are you sure you want to close <b>IUTGram<b>?"));
        message->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        message->setDefaultButton(QMessageBox::No);
        message->setIconPixmap(*myIcon);
        int ret = message->exec();
        int get = check->checkState();
        if(get){
            if(file.open(QFile::WriteOnly | QFile::Text)){
                QTextStream str(&file);
                str << check->checkState();
                file.close();
            }
        }
        if(ret == QMessageBox::Yes) exit(0);
    }
    else if(event->key() == Qt::Key_Escape) exit(0);

}
