#include "slideshow.h"
#include "ui_slideshow.h"
#include "previewlistwgt.h"
#include "protreewidget.h"

SlideShow::SlideShow(QWidget *parent, QTreeWidgetItem* first_item, QTreeWidgetItem* last_item) :
    QDialog(parent),
    ui(new Ui::SlideShow), _first_item(first_item), _last_item(last_item)
{
    ui->setupUi(this);

    //设置无边框样式
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    //清空下4个按钮的文字，初始化4个按钮（设置图标路径，设置初始图标）
    ui->pbtn_Pre->setText("");
    ui->pbtn_Next->setText("");
    ui->pbtn_Play->setText("");
    ui->pbtn_Close->setText("");
    ui->pbtn_Pre->initIcon(":/res/icon/previous.png", ":/res/icon/previous_hover.png", ":/res/icon/previous_press.png");
    ui->pbtn_Next->initIcon(":/res/icon/next.png", ":/res/icon/next_hover.png", ":/res/icon/next_press.png");
    ui->pbtn_Close->initIcon(":/res/icon/closeshow.png", ":/res/icon/closeshow_hover.png", ":/res/icon/closeshow_press.png");
    ui->pbtn_Play->initIcons(":/res/icon/play.png", ":/res/icon/play_hover.png", ":/res/icon/play_press.png",
                             ":/res/icon/pause.png", ":/res/icon/pause_hover.png", ":/res/icon/pause_press.png");

    //关联幻灯片界面的关闭按钮信号
    connect(ui->pbtn_Close, &QPushButton::clicked, this, &SlideShow::close);
    //关联幻灯片界面的切换按钮信号
    connect(ui->pbtn_Pre, &QPushButton::clicked, this, &SlideShow::slot_SlidePrevious);
    connect(ui->pbtn_Next, &QPushButton::clicked, this, &SlideShow::slot_SlideNext);
    //关联预览列表的点击项信号到动画区域
    connect(ui->lwgt_Preview, &PreviewListWgt::sig_UpdateSelectedShow, ui->wgt_PicAnimation, &PicAnimationWgt::slot_UpdateSelectedShow);
    //关联动画播放按钮的信号到动画区域，控制动画的启停
    connect(ui->pbtn_Play, &QPushButton::clicked, ui->wgt_PicAnimation, &PicAnimationWgt::slot_StartOrStop);
    //关联动画区域的启停信号到播放按钮，控制按钮的状态图标
    connect(ui->wgt_PicAnimation, &PicAnimationWgt::sig_Start, ui->pbtn_Play, &PicStateBtn::slot_Start);
    connect(ui->wgt_PicAnimation, &PicAnimationWgt::sig_Stop, ui->pbtn_Play, &PicStateBtn::slot_Stop);
    //关联动画区域的音乐启停信号到树形图，树形图控制音乐播放
    ProTreeWidget* pro_treeWidget = dynamic_cast<ProTreeWidget*>(parent);
    connect(ui->wgt_PicAnimation, &PicAnimationWgt::sig_StartMusic, pro_treeWidget, &ProTreeWidget::slot_StartMusic);
    connect(ui->wgt_PicAnimation, &PicAnimationWgt::sig_StopMusic, pro_treeWidget, &ProTreeWidget::slot_StopMusic);

    //关联动画区域的更新信号到缩略图预览区域
    connect(ui->wgt_PicAnimation, &PicAnimationWgt::sig_UpdatePreviewList, ui->lwgt_Preview, &PreviewListWgt::slot_UpdatePreviewList);
    //关联动画区域的同步选中信号到缩略图预览区域
    connect(ui->wgt_PicAnimation, &PicAnimationWgt::sig_SelectedPreviewListItem, ui->lwgt_Preview, &PreviewListWgt::slot_SelectedPreviewListItem);
    //设置第一项，启动轮播渐变效果
    ui->wgt_PicAnimation->SetPixmap(_first_item);
    ui->wgt_PicAnimation->Start(); //启动轮播渐变效果
}

SlideShow::~SlideShow()
{
    delete ui;
}

//前置按钮点击的槽函数
void SlideShow::slot_SlidePrevious()
{
    //调用动画区域的前置切换方法
    ui->wgt_PicAnimation->SlidePrevious();
}

//后置按钮点击的槽函数
void SlideShow::slot_SlideNext()
{
    //调用动画区域的后置切换方法
    ui->wgt_PicAnimation->SlideNext();
}
