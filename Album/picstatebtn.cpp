#include "picstatebtn.h"
#include "const.h"
#include <QEvent>

PicStateBtn::PicStateBtn(QWidget *parent) : QPushButton(parent)
{}

//初始化图标路径，并且设置正常状态下的图标
void PicStateBtn::initIcons(const QString &normal_path_play, const QString &hover_path_play, const QString &pressed_path_play, const QString &normal_path_pause, const QString &hover_path_pause, const QString &pressed_path_pause)
{
    //存储各图标路径
    _normal_path_play = normal_path_play;
    _hover_path_play = hover_path_play;
    _pressed_path_play = pressed_path_play;
    _normal_path_pause = normal_path_pause;
    _hover_path_pause = hover_path_pause;
    _pressed_path_pause = pressed_path_pause;

    //初始设置好按钮大小、加载正常状态下的图标、设置图标大小
    QPixmap pixmap; pixmap.load(_normal_path_play);
    this->resize(pixmap.size());
    this->setIcon(pixmap);
    this->setIconSize(pixmap.size());
    //初始设置状态为“播放-正常状态”
    _curr_state = PlayNormalState;
}

//重写event方法，处理一些事件
bool PicStateBtn::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Enter:
        //鼠标进入按钮范围，如果是播放状态，显示“播放-悬停”，否则显示“暂停-悬停”
        if (_curr_state < PauseNormalState) setPlayHoverIcon();
        else setPauseHoverIcon();
        break;
    case QEvent::Leave:
        //鼠标离开按钮范围，如果是播放状态，显示“播放-正常”，否则显示“暂停-正常”
        if (_curr_state < PauseNormalState) setPlayNormalIcon();
        else setPauseNormalIcon();
        break;
    case QEvent::MouseButtonPress:
        //鼠标点击按钮，如果是播放状态，显示“播放-点击”，否则显示“暂停-点击”
        if (_curr_state < PauseNormalState) setPlayPressedIcon();
        else setPausePressedIcon();
        break;
    case QEvent::MouseButtonRelease:
        //鼠标释放按钮，如果是播放状态，显示“暂停-悬停”，否则显示“播放-悬停”
        if (_curr_state < PauseNormalState) setPauseHoverIcon();
        else setPlayHoverIcon();
        break;
    default:
        break;
    }

    return QPushButton::event(event);
}

void PicStateBtn::setPlayNormalIcon()
{
    //加载图标，设置状态
    QPixmap pixmap; pixmap.load(_normal_path_play);
    this->setIcon(pixmap);
    _curr_state = PlayNormalState;
}

void PicStateBtn::setPlayHoverIcon()
{
    //加载图标，设置状态
    QPixmap pixmap; pixmap.load(_hover_path_play);
    this->setIcon(pixmap);
    _curr_state = PlayHoverState;
}

void PicStateBtn::setPlayPressedIcon()
{
    //加载图标，设置状态
    QPixmap pixmap; pixmap.load(_pressed_path_play);
    this->setIcon(pixmap);
    _curr_state = PlayPressedState;
}

void PicStateBtn::setPauseNormalIcon()
{
    //加载图标，设置状态
    QPixmap pixmap; pixmap.load(_normal_path_pause);
    this->setIcon(pixmap);
    _curr_state = PauseNormalState;
}

void PicStateBtn::setPauseHoverIcon()
{
    //加载图标，设置状态
    QPixmap pixmap; pixmap.load(_hover_path_pause);
    this->setIcon(pixmap);
    _curr_state = PauseHoverState;
}

void PicStateBtn::setPausePressedIcon()
{
    //加载图标，设置状态
    QPixmap pixmap; pixmap.load(_pressed_path_pause);
    this->setIcon(pixmap);
    _curr_state = PausePressedState;
}

//动画区域的启停信号的槽函数，控制按钮的状态图标
void PicStateBtn::slot_Start()
{
    //设置“暂停-正常”图标，因为动画播放，按钮自然要显示“暂停”图标，提示可暂停
    setPauseNormalIcon();
}

//动画区域的启停信号的槽函数，控制按钮的状态图标
void PicStateBtn::slot_Stop()
{
    //设置“播放-正常”图标，因为动画暂停，按钮自然要显示“播放”图标，提示可播放
    setPlayNormalIcon();
}
