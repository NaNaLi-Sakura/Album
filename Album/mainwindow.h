/*
* 程序：mainwindow.h
*
* 功能：主窗口
*
* 作者：雪与冰心丶
*
* 日期：2024/10/3
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "protree.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void sig_OpenPro(const QString& src_path); //参数为用户选择好的路径

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    //捕获窗口尺寸变化事件
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;

    QWidget* _proTree; //目录树界面（降低耦合，采用通用基类）
    QWidget* _picShow; //图片展示界面（降低耦合，采用通用基类）

    //初始菜单栏
    void initMenu();

private slots:
    //新建项目
    void slot_createPro(bool checked = false);
    //导入项目
    void slot_openPro(bool checked = false);
};
#endif // MAINWINDOW_H
