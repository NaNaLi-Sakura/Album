#include "picanimationwgt.h"
#include "protreeitem.h"
#include <QPainter>

PicAnimationWgt::PicAnimationWgt(QWidget *parent)
    : QWidget(parent), _factor(0.0), _curr_item(nullptr), _is_start(false)
{
    //创建定时器，连接定时器的信号
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, &PicAnimationWgt::slot_Timeout);
}

PicAnimationWgt::~PicAnimationWgt()
{}

//设置展示的图片
void PicAnimationWgt::SetPixmap(QTreeWidgetItem *treeItem)
{
    //检测项是否存在，初始记录为当前项
    if (!treeItem) return;
    _curr_item = treeItem;
    //记录存储该项，设置图片1
    ProTreeItem* pro_treeItem = dynamic_cast<ProTreeItem*>(treeItem);
    QString item_path = pro_treeItem->GetPath();
    _pixmap1.load(item_path);
    if (_item_map.find(item_path) == _item_map.end()) {
        //若没有存储，则存储
        _item_map[item_path] = pro_treeItem;
        //发送更新预览列表逻辑
        emit sig_UpdatePreviewList(pro_treeItem);
    }

    //在设置过图片1后，发送信号，同步缩略图的选中项
    emit sig_SelectedPreviewListItem(pro_treeItem);

    //若已存储过该项，则无需再次存储。记录存储下一项，设置图片2
    ProTreeItem* pro_nextItem = pro_treeItem->GetNextItem();
    if (!pro_nextItem) return;
    QString next_item_path = pro_nextItem->GetPath();
    _pixmap2.load(next_item_path);
    if (_item_map.find(next_item_path) == _item_map.end()) {
        //若没有存储下一项，则存储
        _item_map[next_item_path] = pro_nextItem;
        //发送更新预览列表逻辑
        emit sig_UpdatePreviewList(pro_nextItem);
    }
}

//开启轮播
void PicAnimationWgt::Start()
{
    //给播放按钮、音频播放发送开启的信号
    emit sig_Start();
    emit sig_StartMusic();
    //动画因子初始置0，开启定时器，设置开始播放的标志置为true
    _factor = 0;
    _timer->start(25);
    _is_start = true;
}

//停止轮播
void PicAnimationWgt::Stop()
{
    //给播放按钮、音频播放发送停止的信号
    emit sig_Stop();
    emit sig_StopMusic();
    //停止计时器，将动画因子置0，将开始播放的标志置为false
    _timer->stop();
    _factor = 0;
    _is_start = false;
}

//前置切换
void PicAnimationWgt::SlidePrevious()
{
    //停止动画，根据当前项获取前项，设置展示前项图片，触发重绘动画
    Stop();
    if (!_curr_item) return;
    ProTreeItem* pro_curr_item = dynamic_cast<ProTreeItem*>(_curr_item);
    ProTreeItem* pro_pre_item = pro_curr_item->GetPreItem();
    if (!pro_pre_item) return;

    SetPixmap(pro_pre_item);
    this->update();
}

//后置切换
void PicAnimationWgt::SlideNext()
{
    //停止动画，根据当前项获取后项，设置展示后项图片，触发重绘动画
    Stop();
    if (!_curr_item) return;
    ProTreeItem* pro_curr_item = dynamic_cast<ProTreeItem*>(_curr_item);
    ProTreeItem* pro_next_item = pro_curr_item->GetNextItem();
    if (!pro_next_item) return;

    SetPixmap(pro_next_item);
    this->update();
}

//重绘事件
void PicAnimationWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    //图1
    //-------------------------------------------------------------------
    /* 这段代码的主要功能是在一个Widget上居中绘制一个带有透明度效果的QPixmap。
     * 它首先检查 _pixmap1 是否有效，然后创建一个 QPainter 对象并设置抗锯齿效果以提高绘图质量。
     * 接着获取Widget的几何尺寸，并根据这些尺寸对 _pixmap1 进行缩放以适应Widget，同时保持图片的宽高比不变。
     * 之后计算了一个透明度值 alpha，用于调整图像的不透明度。
     * 然后创建一个新的 QPixmap alphaPixmap 并将其填充为透明。
     * 接下来使用这个新的 QPixmap 作为绘图设备，先以源复制模式绘制原始图像，
     * 再改变合成模式并在整个区域上绘制一个带有透明度的黑色矩形，从而实现图像的渐变效果。
     * 最后，计算出 _pixmap1 在Widget中的居中位置，
     * 并使用初始创建的 QPainter 在该位置绘制经过透明度处理的 QPixmap。
     * 这样就能在Widget上居中显示一个带有透明度效果的图像。
    */
    //检测图1是否有效
    if (_pixmap1.isNull()) return;

    //创建一个QPainter对象用于绘制当前widget
    QPainter painter(this);
    //设置抗锯齿效果以提高绘制质量
    painter.setRenderHint(QPainter::Antialiasing, true);

    //获取当前widget的几何尺寸（位置+大小）
    QRect rect = this->geometry();
    //提取矩形的宽度和高度
    int width = rect.width();
    int height = rect.height();
    //将_pixmap1缩放以适应当前widget的大小，并保持原图的宽高比
    _pixmap1 = _pixmap1.scaled(width, height, Qt::KeepAspectRatio);

    //计算透明度值，_factor是一个介于0到1之间的浮点数，表示不透明度的比例
    //alpha则是对应的透明度值（0完全透明，255完全不透明）
    int alpha = (1.0f - _factor) * 255; //_factor越大，alpha越小，越透明

    //创建一个与_pixmap1同样大小的新QPixmap对象，并填充为透明
    QPixmap alphaPixmap(_pixmap1.size());
    alphaPixmap.fill(Qt::transparent);
    //使用新创建的QPixmap作为设备来绘制
    QPainter painter1(&alphaPixmap);
    //设置合成模式为源复制模式，这意味着新的像素将完全替换旧的像素
    painter1.setCompositionMode(QPainter::CompositionMode_Source);
    //在新QPixmap的位置(0, 0)处绘制原始的_pixmap1
    painter1.drawPixmap(0, 0, _pixmap1);
    //改变合成模式为目标在源内部模式，这意味着只有与目标重叠的部分才会被绘制
    painter1.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    //使用带有透明度alpha的黑色填充整个新QPixmap的矩形区域
    painter1.fillRect(alphaPixmap.rect(), QColor(0, 0, 0, alpha));
    //结束对alphaPixmap的绘制操作
    painter1.end();

    //计算_pixmap1在当前widget中的居中位置
    int x = (width - _pixmap1.width()) / 2;
    int y = (height - _pixmap1.height()) / 2;
    //在当前widget的居中位置绘制带有透明度的QPixmap
    painter.drawPixmap(x, y, alphaPixmap);
    //-------------------------------------------------------------------

    //图2
    //-------------------------------------------------------------------
    //检测图2是否有效
    if (_pixmap2.isNull()) return;

    //将_pixmap2缩放以适应当前widget的大小，并保持原图的宽高比
    _pixmap2 = _pixmap2.scaled(width, height, Qt::KeepAspectRatio);
    //计算不透明值
    alpha = (_factor) * 255;
    //创建一个与_pixmap2同样大小的新QPixmap对象，并填充为透明
    QPixmap alphaPixmap2(_pixmap2.size());
    alphaPixmap2.fill(Qt::transparent);
    //使用新创建的QPixmap作为设备来绘制
    QPainter painter2(&alphaPixmap2);
    //设置合成模式为源复制模式，这意味着新的像素将完全替换旧的像素
    painter2.setCompositionMode(QPainter::CompositionMode_Source);
    //在新QPixmap的位置(0, 0)处绘制原始的_pixmap1
    painter2.drawPixmap(0, 0, _pixmap2);
    //改变合成模式为目标在源内部模式，这意味着只有与目标重叠的部分才会被绘制
    painter2.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    //使用带有透明度alpha的黑色填充整个新QPixmap的矩形区域
    painter2.fillRect(alphaPixmap2.rect(), QColor(0, 0, 0, alpha));
    //结束对alphaPixmap的绘制操作
    painter2.end();

    //计算_pixmap2在当前widget中的居中位置
    x = (width - _pixmap2.width()) / 2;
    y = (height - _pixmap2.height()) / 2;
    //在当前widget的居中位置绘制带有透明度的QPixmap
    painter.drawPixmap(x, y, alphaPixmap2);
    //-------------------------------------------------------------------
}

//列表项被点击，给动画区域发送带有点击项路径信息的信号，动画区域对应的槽函数
void PicAnimationWgt::slot_UpdateSelectedShow(QString item_path)
{
    //根据路径找到对应记录的项，加载动画，触发重绘显示动画
    auto iter = _item_map.find(item_path);
    if (iter == _item_map.end()) return;
    UpdateSelectedShow(iter.value()); //加载动画
    update();
}

//动画播放按钮被点击，控制动画播放的启停
void PicAnimationWgt::slot_StartOrStop()
{
    //若当前未开始，则开始播放
    if (!_is_start) {
        //将不透明度因子置0，开启定时器触发动画重绘播放，最后将启动状态置为true
        _factor = 0;
        _timer->start(25);
        _is_start = true;
        //给音频播放发送启动信号
        emit sig_StartMusic();
    }
    else {
        //若当前正在播放，则停止播放
        //将不透明度因子置0，关闭定时器，手动触发重绘，最后将启动状态置为false
        _factor = 0;
        _timer->stop();
        this->update();
        _is_start = false;
        //给音频播放发送启动信号
        emit sig_StopMusic();
    }
}

//定时器超时信号的槽函数
void PicAnimationWgt::slot_Timeout()
{
    //检测当前项，不存在则停止播放动画，触发重绘，返回
    if (!_curr_item) {
        Stop();
        this->update();
        return;
    }
    //更新动画因子
    _factor += 0.01;
    if (_factor >= 1) {
        //若动画因子超过了1，置0
        _factor = 0;
        //设置展示下一张图片，触发重绘
        ProTreeItem* pro_curr_item = dynamic_cast<ProTreeItem*>(_curr_item);
        ProTreeItem* pro_next_item = pro_curr_item->GetNextItem();
        if (!pro_next_item) {
            //下一项不存在则停止播放动画，触发重绘，返回
            Stop();
            this->update();
            return;
        }
        SetPixmap(pro_next_item);
        this->update();
        return;
    }
    //动画因子没有超过1，仅更新动画因子即可，然后触发重绘
    this->update();
}

//加载动画
void PicAnimationWgt::UpdateSelectedShow(QTreeWidgetItem *treeItem)
{
    //检测项的有效性
    if (!treeItem) return;
    //获取选中项的路径，若当前项未记录在集合，则记录到集合，存储当前项，加载到图片1
    ProTreeItem* pro_treeItem = dynamic_cast<ProTreeItem*>(treeItem);
    QString item_path = pro_treeItem->GetPath();
    if (_item_map.find(item_path) == _item_map.end()) {
        _item_map[item_path] = pro_treeItem;
    }
    _curr_item = pro_treeItem;
    _pixmap1.load(item_path);
    //获取下一项，获取下一项的路径，记录到集合，加载到图2
    ProTreeItem* pro_nextItem = pro_treeItem->GetNextItem();
    if (!pro_nextItem) return;
    QString nextItem_path = pro_nextItem->GetPath();
    if (_item_map.find(nextItem_path) == _item_map.end()) {
        _item_map[nextItem_path] = pro_nextItem;
    }
    _pixmap2.load(nextItem_path);
}
