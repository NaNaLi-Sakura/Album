/*
* 程序：slideshow.h
*
* 功能：幻灯片播放界面类
*
* 作者：雪与冰心丶
*
* 日期：2024/10/7
*/

#ifndef SLIDESHOW_H
#define SLIDESHOW_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class SlideShow;
}

class SlideShow : public QDialog
{
    Q_OBJECT

public:
    explicit SlideShow(QWidget *parent = nullptr, QTreeWidgetItem* first_item = nullptr, QTreeWidgetItem* last_item = nullptr);
    ~SlideShow();

private:
    Ui::SlideShow *ui;

    QTreeWidgetItem* _first_item;
    QTreeWidgetItem* _last_item;

private slots:
    //前置按钮点击的槽函数
    void slot_SlidePrevious();
    //后置按钮点击的槽函数
    void slot_SlideNext();

};

#endif // SLIDESHOW_H
