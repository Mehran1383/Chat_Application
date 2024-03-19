#include "addphoto.h"
#include "ui_addphoto.h"

AddPhoto::AddPhoto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPhoto)
{
    ui->setupUi(this);
    ui->save->setDisabled(1);
    ui->sampel->hide();
    conected=false;
    tryAgain=false;
    camera = new QCamera();
    qDebug() << "Number of cameras found :" << QCameraInfo::availableCameras().count();
   QList <QCameraInfo> cameras = QCameraInfo::availableCameras();
   foreach (const QCameraInfo & cameraInfo,cameras) {
       qDebug() << "Camera info:" << cameraInfo.deviceName() << cameraInfo.description() << cameraInfo.position();
       ui->deviceSelection->addItem(cameraInfo.description());
}
   QFile user("username.txt");
   user.open(QFile::ReadOnly | QFile::Text);
   QTextStream nstream(&user);
   nstream >> username;
}

AddPhoto::~AddPhoto()
{
    delete ui;
}

void AddPhoto::on_connect_clicked()
{
    if(!conected){
        connectCamera();

    }
    else{
        camera->stop();
        viewfinder->deleteLater();
        ui->connect->setText("Connect");
        conected = false;
    }
}

void AddPhoto::connectCamera(){
    QList <QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach(const QCameraInfo & cameraInfo,cameras){
        qDebug() << cameraInfo.description() << ui->deviceSelection->currentText();
        if(cameraInfo.description() == ui->deviceSelection->currentText()){
            camera = new QCamera(cameraInfo);
            viewfinder = new QCameraViewfinder(this);
            camera->setViewfinder(viewfinder);
            ui->horizontalLayout_2->addWidget(viewfinder);
            conected = true;
            ui->connect->setText("Disconnect");
            camera->start();
            return;

        }
    }
}

void AddPhoto::on_capture_clicked()
{
    if(conected && !tryAgain){

        imageCapture = new QCameraImageCapture(camera);
        camera->setCaptureMode(QCamera::CaptureStillImage);
        camera->searchAndLock();
        imageCapture->capture("C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\photos\\" + username+ ".jpg");
        ui->capture->setText("Try again");
        camera->unlock();
        ui->save->setDisabled(0);
        tryAgain = true;
        camera->stop();
        camera->start();

    }
    else if(conected){
        ui->capture->setText("Capture");
        ui->sampel->hide();
        tryAgain = false;
        QDir dir;
        QString str = "C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\photos\\" + username + ".jpg";
        dir.remove(str);
    }
}

void AddPhoto::on_choose_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open file"),"C://","Picture files (*.jpg)");
    if(filename!=NULL){
    QPixmap img(filename);
    QPixmap img2 = img.scaled(200,200,Qt::KeepAspectRatio);
    ui->sampel->show();
    ui->sampel->setPixmap(img2);
    ui->save->setDisabled(0);
    QString newName = "C:\\Users\\Topsystem\\Documents\\build-nmassenger-Desktop_Qt_5_13_1_MSVC2017_64bit-Debug\\photos\\" + username + ".jpg";
    QDir dir;
    dir.rename(filename,newName);
    }
}

void AddPhoto::on_save_clicked()
{
    if(camera->isAvailable()){
    camera->stop();
    viewfinder->deleteLater();}
    Chat* chat=new Chat;
    chat->show();
    this->hide();
    chat->setWindowFlag(Qt::FramelessWindowHint);
    chat->setStyleSheet("background-color: transparent;");
    chat->exec();

}
