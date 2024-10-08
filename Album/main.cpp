#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //设置qss样式
    QFile qss(":/res/style/style.qss");
    if (qss.open(QIODevice::ReadOnly)) {
//调试
//        qDebug("Open qss success!");
        QString qssStyleSheet = QLatin1String(qss.readAll());
        a.setStyleSheet(qssStyleSheet);
        qss.close();
    }
    else {
        qDebug("Open qss failed.");
        return 0;
    }

    //创建窗口，设置窗口标题，设置最大化显示
    MainWindow w;
    w.setWindowTitle("相册");
    w.showMaximized();

    return a.exec();
}
