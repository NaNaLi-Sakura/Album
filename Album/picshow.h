/*
* 程序：picshow.h
*
* 功能：图片展示界面类
*
* 作者：雪与冰心丶
*
* 日期：2024/10/6
*/

#ifndef PICSHOW_H
#define PICSHOW_H

#include <QDialog>
#include <QPropertyAnimation> //动画属性效果

namespace Ui {
class PicShow;
}

class PicShow : public QDialog
{
    Q_OBJECT

signals:
    void sig_PreClicked(); //左边按钮被点击
    void sig_NextClicked(); //右边按钮被点击

public:
    explicit PicShow(QWidget *parent = nullptr);
    ~PicShow();

    //重载图片
    void ReloadPic();

protected:
    //重写event方法，处理一些事件
    bool event(QEvent *event) override;

public slots:
    //双击显示图片
    void slot_ShowPic(const QString& pic_path);
    //切换显示图片
    void slot_ReloadPic(const QString& pic_path);
    //当前选中项位于要关闭的项目中，所以清理当前选中项的图片展示
    void slot_ClearSelected();

private:
    //设置按钮显示或隐藏
    void ShowOrHiddenButtons(bool is_show);

    Ui::PicShow *ui;

    QPropertyAnimation* _animation_show_pre; //动画效果（上一个）
    QPropertyAnimation* _animation_show_next; //动画效果（下一个）

    bool _is_visible_btn; //按钮可见状态

    QString _selected_path; //选择项表示的路径
    QPixmap _pixmap; //图片
};

#endif // PICSHOW_H
