#include "protreeitem.h"
#include "const.h"

ProTreeItem::ProTreeItem(QTreeWidget *treeview, const QString &name, const QString &path, int type)
    : QTreeWidgetItem(treeview, type), _path(path), _name(name), _rootItem(nullptr), _preItem(nullptr), _nextItem(nullptr)
{

}

ProTreeItem::ProTreeItem(QTreeWidgetItem *parentItem, const QString &name, const QString &path, QTreeWidgetItem *rootItem, int type)
    : QTreeWidgetItem(parentItem, type), _path(path), _name(name), _rootItem(rootItem), _preItem(nullptr), _nextItem(nullptr)
{

}

const QString &ProTreeItem::GetPath() const
{
    return _path;
}

QTreeWidgetItem *ProTreeItem::GetRootItem()
{
    return _rootItem;
}

ProTreeItem *ProTreeItem::GetPreItem() const
{
    return dynamic_cast<ProTreeItem*>(_preItem);
}

ProTreeItem *ProTreeItem::GetNextItem() const
{
    return dynamic_cast<ProTreeItem*>(_nextItem);
}

//获取最后一个子项
ProTreeItem *ProTreeItem::GetLastPicChild()
{
    //如果当前项是图片，返回为空（图片项没有子项）
    if (this->type() == TreeItemPic) return nullptr;
    //遍历当前项的所有子项，从后往前找，找到最后一个子项（图片项）
    int childItemCount = this->childCount();
    if (childItemCount == 0) return nullptr; //无子项，返回为空
    for (int i = childItemCount - 1; i >= 0; --i) {
        //从后往前逐个处理每个子项，找到图片项则可以直接返回
        QTreeWidgetItem* lastChildItem = this->child(i);
        ProTreeItem* proLastChildItem = dynamic_cast<ProTreeItem*>(lastChildItem);
        int lastItemType = proLastChildItem->type();
        if (lastItemType == TreeItemPic) return proLastChildItem;
        //非图片项（项目级、目录级）都属于文件夹项（其实不会包括项目级，因为这些都是在子级中遍历的）
        //即文件夹项，则再此调用此函数，查找文件夹中的最后一个子项（递归调用）
        proLastChildItem = proLastChildItem->GetLastPicChild();
        if (!proLastChildItem) continue; //若当前文件夹项不存在子图片项，则继续下一个遍历
        //若存在最后一个子项（图片项），则返回
        return proLastChildItem;
    }
    //若所有子项都为文件夹项，且都不存在子图片项，返回为空
    return nullptr;
}

//获取第一个子项
ProTreeItem *ProTreeItem::GetFirstPicChild()
{
    //如果当前项是图片项，返回为空（图片项没有子项）
    if (this->type() == TreeItemPic) return nullptr;
    //遍历当前项的所有子项，从前往后找，找到第一个子项（图片项）
    int childItemCount = this->childCount();
    if (childItemCount == 0) return nullptr; //无子项，返回为空
    for (int i = 0; i < childItemCount; ++i) {
        //从前往后逐个处理每个子项，找到图片项则可以直接返回
        QTreeWidgetItem* firstChildItem = this->child(i);
        ProTreeItem* proFirstChildItem = dynamic_cast<ProTreeItem*>(firstChildItem);
        int firstItemType = proFirstChildItem->type();
        if (firstItemType == TreeItemPic) return proFirstChildItem;
        //文件夹项，则再此调用此函数，查找文件夹中的第一个子项（递归调用）
        proFirstChildItem = proFirstChildItem->GetFirstPicChild();
        if (!proFirstChildItem) continue; //若当前文件夹项不存在子图片项，则继续下一个遍历
        //若存在第一个子项（图片项），则返回
        return proFirstChildItem;
    }
    //若所有子项都为文件夹项，且都不存在子图片项，返回为空
    return nullptr;
}

void ProTreeItem::SetPreItem(QTreeWidgetItem *preItem)
{
    _preItem = preItem;
}

void ProTreeItem::SetNextItem(QTreeWidgetItem *nextItem)
{
    _nextItem = nextItem;
}
