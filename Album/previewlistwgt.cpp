#include "previewlistwgt.h"
#include "protreeitem.h"
#include "const.h"
#include <QPainter>
#include "previewlistitem.h"
#include <QGuiApplication>

PreviewListWgt::PreviewListWgt(QWidget *parent) : QListWidget(parent), _index(0), _last_index(17)
{
    //设置视图模式为图标模式
    this->setViewMode(QListWidget::IconMode);
    //设置图标大小
    this->setIconSize(QSize(PREVIEW_ICON_SIZE, PREVIEW_ICON_SIZE));
    //设置列表项之间的间距为5像素
    this->setSpacing(5);

    //关联项的点击信号
    connect(this, &PreviewListWgt::itemPressed, this, &PreviewListWgt::slot_ItemPressed);
}

PreviewListWgt::~PreviewListWgt()
{}

//记录项，加载缩略图
void PreviewListWgt::AddListItem(const QString &item_path)
{
    //加载并按比例缩放原图片
    QPixmap src_pixmap(item_path);
    src_pixmap = src_pixmap.scaled(PREVIEW_ICON_SIZE, PREVIEW_ICON_SIZE, Qt::KeepAspectRatio);
    //创建目标图片，设置其大小、颜色及透明度
    QPixmap dst_pixmap(QSize(PREVIEW_ICON_SIZE, PREVIEW_ICON_SIZE));
    dst_pixmap.fill(QColor(220, 220, 220, 50)); //浅灰色，较透明（0是完全透明）
    //在目标图片的预留上下左右对称间隔处，画上经缩放后的原图
    int src_width = src_pixmap.width();
    int src_height = src_pixmap.height();
    int dst_width = dst_pixmap.width();
    int dst_height = dst_pixmap.height();
    int x = (dst_width - src_width) / 2;
    int y = (dst_height - src_height) / 2;
    QPainter painter(&dst_pixmap);
    painter.drawPixmap(x, y, src_pixmap);
    //图片项索引更新
    _index++;

    //据此路径创建项(自定义预览列表项)，设置项的大小，加入列表视图，加入集合，记录起点的位置
    PreviewListItem* preListItem = new PreviewListItem(QIcon(dst_pixmap), item_path, _index, this);
    preListItem->setSizeHint(QSize(PREVIEW_ITEM_SIZE, PREVIEW_ITEM_SIZE));
    this->addItem(preListItem);
    _item_set[item_path] = preListItem;
    if (_index == 1) _origin_pos = this->pos();
}

//更新预览列表
void PreviewListWgt::slot_UpdatePreviewList(QTreeWidgetItem *treeItem)
{
    //检测参数的有效性
    if (!treeItem) return;

    //获取项的路径，去集合中查找
    ProTreeItem* pro_treeItem = dynamic_cast<ProTreeItem*>(treeItem);
    QString item_path = pro_treeItem->GetPath();
    auto iter = _item_set.find(item_path);
    if (iter != _item_set.end()) return; //若已有记录，则直接返回

    //若没有该项的记录，则记录该项，加载缩略图
    AddListItem(item_path);
}

//同步预览列表的选中项
void PreviewListWgt::slot_SelectedPreviewListItem(QTreeWidgetItem *treeItem)
{
    //检测参数的有效性
    if (!treeItem) return;

    //获取项的路径，去集合中查找是否有记录
    ProTreeItem* pro_treeItem = dynamic_cast<ProTreeItem*>(treeItem);
    QString item_path = pro_treeItem->GetPath();
    auto iter = _item_set.find(item_path);
    if (iter == _item_set.end()) return; //无记录则直接返回

    //找到记录项，获取索引
    PreviewListItem* previewItem = dynamic_cast<PreviewListItem*>(iter.value());
    int index = previewItem->GetIndex();
    if (index > 17) {
        //如果索引大于17，即图片项已经记录了17个了，表示已经超过了最后一项
        //超过多少项，就将列表向左移动超过项数的宽度
        QPoint curr_pos = this->pos();
        this->move(curr_pos.x() - (index - _last_index) * PREVIEW_ITEM_SIZE, curr_pos.y());
    }
    else {
        //若没有超过最后一项（界面规定的最后一项，即索引为17），
        //则不移动（移动到起始位置，相当于做下变化），更新下最后一项的索引（没必要）
        this->move(_origin_pos);
        _last_index = 17;
    }
    //更新当前选中项
    this->setCurrentItem(iter.value());
}

//列表图中的项被点击的槽函数
void PreviewListWgt::slot_ItemPressed(QListWidgetItem *listItem)
{
    //检测是否是左键
    if (QGuiApplication::mouseButtons() != Qt::LeftButton) return;

    //设置点击项为当前项，获取项代表的路径，将路径作为消息给动画区域发送信号
    this->setCurrentItem(listItem);
    PreviewListItem* pre_listItem = dynamic_cast<PreviewListItem*>(listItem);
    QString item_path = pre_listItem->GetPath();
    emit sig_UpdateSelectedShow(item_path);
}
