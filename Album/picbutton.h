/*
* 程序：picbutton.h
*
* 功能：图片展示界面的按钮操作类
*
* 作者：雪与冰心丶
*
* 日期：2024/10/6
*/

#ifndef PICBUTTON_H
#define PICBUTTON_H

#include <QPushButton>

class PicButton : public QPushButton
{
public:
    explicit PicButton(QWidget *parent = nullptr);

    //设置图标路径，并且设置正常状态下的图标
    void initIcon(const QString& normal_path, const QString& hover_path, const QString& pressed_path);

protected:
    //重写event方法，处理一些事件
    bool event(QEvent *event) override;

private:
    void setNormalIcon();
    void setHoverIcon();
    void setPressedIcon();

    QString _normal_path;
    QString _hover_path;
    QString _pressed_path;
};

#endif // PICBUTTON_H
