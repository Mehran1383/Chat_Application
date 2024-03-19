#ifndef ADDPHOTO_H
#define ADDPHOTO_H

#include <QWidget>
#include <QtDebug>
#include <QCameraInfo>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QMediaRecorder>
#include <QUrl>
#include <QtGui>
#include <QTextStream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFileDialog>
#include <QDialog>
#include "chat.h"
namespace Ui {
class AddPhoto;
}

class AddPhoto : public QDialog
{
    Q_OBJECT

public:
    explicit AddPhoto(QWidget *parent = nullptr);
    ~AddPhoto();
    void connectCamera();
    QJsonValue jsonValFromPixmap(const QPixmap* );
    QPixmap picmapForm(const QJsonValue &);
    bool writejson(const QJsonObject &);
    QString username;

private slots:
    void on_connect_clicked();

    void on_capture_clicked();

    void on_choose_clicked();

    void on_save_clicked();

private:
    Ui::AddPhoto *ui;
    QCamera *camera;
    QCameraViewfinder* viewfinder;
    QCameraImageCapture *imageCapture;
    bool conected;
    bool tryAgain;
};

#endif // ADDPHOTO_H
