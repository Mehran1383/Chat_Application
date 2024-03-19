#include "login.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;


    w.setWindowFlags(Qt::FramelessWindowHint);
    //w.setStyleSheet("background-color: transparent;");
    w.show();
    return a.exec();
}
