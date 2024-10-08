#include "picshow.h"
#include "ui_picshow.h"
#include "picbutton.h"
#include <QGraphicsOpacityEffect> //图形透明度效果

PicShow::PicShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PicShow)
{
    ui->setupUi(this);

    //清空下标签按钮的文字，初始设置两个按钮图标及路径参数
    ui->lb_PicShow->setText("");
    ui->pbtn_PreviousPic->setText("");
    ui->pbtn_NextPic->setText("");
    ui->pbtn_PreviousPic->initIcon(":/res/icon/previous.png", ":/res/icon/previous_hover.png", ":/res/icon/previous_press.png");
    ui->pbtn_NextPic->initIcon(":/res/icon/next.png", ":/res/icon/next_hover.png", ":/res/icon/next_press.png");

    //创建两个图形透明效果，设置为完全透明，分别设置到两个按钮
    QGraphicsOpacityEffect* pre_graphicsOpacityEffect = new QGraphicsOpacityEffect(this);
    pre_graphicsOpacityEffect->setOpacity(0);
    ui->pbtn_PreviousPic->setGraphicsEffect(pre_graphicsOpacityEffect);
    QGraphicsOpacityEffect* next_graphicsOpacityEffect = new QGraphicsOpacityEffect(this);
    next_graphicsOpacityEffect->setOpacity(0);
    ui->pbtn_NextPic->setGraphicsEffect(next_graphicsOpacityEffect);

    //初始化创建两个动画效果
    _animation_show_pre = new QPropertyAnimation(pre_graphicsOpacityEffect, "opacity", this);
    _animation_show_pre->setEasingCurve(QEasingCurve::Linear); //设置渐变曲线
    _animation_show_pre->setDuration(500); //设置渐变时长，单位：毫秒
    _animation_show_next = new QPropertyAnimation(next_graphicsOpacityEffect, "opacity", this);
    _animation_show_next->setEasingCurve(QEasingCurve::Linear); //设置渐变曲线
    _animation_show_next->setDuration(500); //设置渐变时长，单位：毫秒

    //点击按钮，发送信号给树形图
    connect(ui->pbtn_PreviousPic, &QPushButton::clicked, this, &PicShow::sig_PreClicked);
    connect(ui->pbtn_NextPic, &QPushButton::clicked, this, &PicShow::sig_NextClicked);
}

PicShow::~PicShow()
{
    delete ui;
}

//设置按钮显示或隐藏
void PicShow::ShowOrHiddenButtons(bool is_show)
{
    //在按钮当前不可见并且需要展示时
    if (!_is_visible_btn && is_show) {
        //停止当前的动画效果，设置一个展示效果并开启，然后更新当前按钮为可见状态
        _animation_show_pre->stop();
        _animation_show_pre->setStartValue(0);
        _animation_show_pre->setEndValue(1);
        _animation_show_pre->start();

        _animation_show_next->stop();
        _animation_show_next->setStartValue(0);
        _animation_show_next->setEndValue(1);
        _animation_show_next->start();

        _is_visible_btn = true;

        return;
    }

    //在按钮当前可见并且不需要展示时
    if (_is_visible_btn && !is_show) {
        //停止当前的动画效果，设置一个隐藏效果并开启，然后更新当前按钮为不可见状态
        _animation_show_pre->stop();
        _animation_show_pre->setStartValue(1);
        _animation_show_pre->setEndValue(0);
        _animation_show_pre->start();

        _animation_show_next->stop();
        _animation_show_next->setStartValue(1);
        _animation_show_next->setEndValue(0);
        _animation_show_next->start();

        _is_visible_btn = false;

        return;
    }
}

//重写event方法，处理一些事件
bool PicShow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Enter:
        //鼠标进入图片展示界面，显示按钮
        ShowOrHiddenButtons(true);
        break;
    case QEvent::Leave:
        //鼠标进入图片展示界面，隐藏按钮
        ShowOrHiddenButtons(false);
        break;
    default:
        break;
    }

    return QDialog::event(event);
}

//显示图片
void PicShow::slot_ShowPic(const QString &pic_path)
{
    //缓存（记录）图片路径，加载图片，设置缩放比例，显示到标签
    _selected_path = pic_path;
    _pixmap.load(pic_path);
    int width = this->width() - 20; //展示区域与界面区域之间增加些距离，类似于内边距
    int height = this->height() - 20;
    _pixmap = _pixmap.scaled(width, height, Qt::KeepAspectRatio);
    ui->lb_PicShow->setPixmap(_pixmap);
}

//切换显示图片
void PicShow::slot_ReloadPic(const QString &pic_path)
{
    //更新记录当前选中项路径
    _selected_path = pic_path;
    if (_selected_path != "") {
        //加载图片，设置图片缩放，显示到标签
        _pixmap.load(_selected_path);
        const int& width = ui->gridLayout->geometry().width();
        const int& height = ui->gridLayout->geometry().height();
        _pixmap = _pixmap.scaled(width, height, Qt::KeepAspectRatio);
        ui->lb_PicShow->setPixmap(_pixmap);
    }
}

//当前选中项位于要关闭的项目中，所以清理当前选中项的图片展示
void PicShow::slot_ClearSelected()
{
    _selected_path = "";
    ui->lb_PicShow->clear();
}

//重载图片
void PicShow::ReloadPic()
{
    //在图片存在的情况下
    if (_selected_path != "") {
        const int& width = ui->gridLayout->geometry().width();
        const int& height = ui->gridLayout->geometry().height();
        _pixmap.load(_selected_path);
        _pixmap = _pixmap.scaled(width, height, Qt::KeepAspectRatio);
        ui->lb_PicShow->setPixmap(_pixmap);
    }
}

