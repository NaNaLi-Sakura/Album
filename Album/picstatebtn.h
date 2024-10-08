/*
* 程序：picstatebtn.h
*
* 功能：幻灯片界面的动画播放按钮操作类
*
* 作者：雪与冰心丶
*
* 日期：2024/10/7
*/

#ifndef PICSTATEBTN_H
#define PICSTATEBTN_H

#include <QPushButton>

class PicStateBtn : public QPushButton
{
public:
    explicit PicStateBtn(QWidget *parent = nullptr);

    //初始化图标路径，并且设置正常状态下的图标
    void initIcons(const QString& normal_path_play, const QString& hover_path_play, const QString& pressed_path_play,
                   const QString& normal_path_pause, const QString& hover_path_pause, const QString& pressed_path_pause);

protected:
    //重写event方法，处理一些事件
    bool event(QEvent *event) override;

private:
    void setPlayNormalIcon();
    void setPlayHoverIcon();
    void setPlayPressedIcon();
    void setPauseNormalIcon();
    void setPauseHoverIcon();
    void setPausePressedIcon();

private:
    QString _normal_path_play;
    QString _hover_path_play;
    QString _pressed_path_play;
    QString _normal_path_pause;
    QString _hover_path_pause;
    QString _pressed_path_pause;
    int _curr_state; //当前状态（3播放、3暂停，共6中状态）

public slots:
    //动画区域的启停信号的槽函数，控制按钮的状态图标
    void slot_Start();
    //动画区域的启停信号的槽函数，控制按钮的状态图标
    void slot_Stop();
};

#endif // PICSTATEBTN_H
