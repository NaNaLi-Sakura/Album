/*
* 程序：previewlistwgt.h
*
* 功能：幻灯片界面的缩略图预览界面的操作类
*
* 作者：雪与冰心丶
*
* 日期：2024/10/7
*/

#ifndef PREVIEWLISTWGT_H
#define PREVIEWLISTWGT_H

#include <QListWidget>
#include <QTreeWidgetItem>

class PreviewListWgt : public QListWidget
{
    Q_OBJECT
public:
    explicit PreviewListWgt(QWidget *parent = nullptr);
    ~PreviewListWgt();

private:
    QMap<QString, QListWidgetItem*> _item_set; //path <=> pre_item
    int _index; //项的索引
    QPoint _origin_pos; //起点的位置
    int _last_index; //最后一项的索引

    //记录项，加载缩略图
    void AddListItem(const QString& item_path);

public slots:
    //更新预览列表
    void slot_UpdatePreviewList(QTreeWidgetItem* treeItem);
    //同步预览列表的选中项
    void slot_SelectedPreviewListItem(QTreeWidgetItem* treeItem);

private slots:
    //列表图中的项被点击的槽函数
    void slot_ItemPressed(QListWidgetItem *listItem);

signals:
    //列表项被点击，给动画区域发送带有点击项路径信息的信号
    void sig_UpdateSelectedShow(QString item_path);
};

#endif // PREVIEWLISTWGT_H
