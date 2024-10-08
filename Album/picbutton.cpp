#include "picbutton.h"
#include <QEvent>

PicButton::PicButton(QWidget *parent) : QPushButton(parent)
{}

//设置图标路径，并且设置正常状态下的图标
void PicButton::initIcon(const QString &normal_path, const QString &hover_path, const QString &pressed_path)
{
    _normal_path = normal_path;
    _hover_path = hover_path;
    _pressed_path = pressed_path;

    //初始设置好按钮大小、正常状态下的图标、图标大小
    QPixmap pixmap;
    pixmap.load(_normal_path);
    this->resize(pixmap.size());
    this->setIcon(pixmap);
    this->setIconSize(pixmap.size());
}

//重写event方法，处理一些事件
bool PicButton::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Enter:
        //鼠标进入按钮范围，显示悬停图标
        setHoverIcon();
        break;
    case QEvent::MouseButtonPress:
        //鼠标点击按钮，显示点击图标
        setPressedIcon();
        break;
    case QEvent::MouseButtonRelease:
        //鼠标释放按钮，显示悬停图标
        setHoverIcon();
        break;
    case QEvent::Leave:
        //鼠标离开按钮，显示正常图标
        setNormalIcon();
        break;
    default:
        break;
    }

    return QPushButton::event(event);
}

void PicButton::setNormalIcon()
{
    QPixmap pixmap; pixmap.load(_normal_path);
    this->setIcon(pixmap);
}
void PicButton::setHoverIcon()
{
    QPixmap pixmap; pixmap.load(_hover_path);
    this->setIcon(pixmap);
}
void PicButton::setPressedIcon()
{
    QPixmap pixmap; pixmap.load(_pressed_path);
    this->setIcon(pixmap);
}

