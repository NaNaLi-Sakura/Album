/*
* 程序：protreeitem.h
*
* 功能：目录树中的项
*
* 作者：雪与冰心丶
*
* 日期：2024/10/4
*/

#ifndef PROTREEITEM_H
#define PROTREEITEM_H

#include <QTreeWidgetItem>

class ProTreeItem : public QTreeWidgetItem
{
public:
    ProTreeItem(QTreeWidget* treeview, const QString& name, const QString& path, int type = Type);
    ProTreeItem(QTreeWidgetItem* parentItem, const QString& name, const QString& path, QTreeWidgetItem* rootItem, int type = Type);

    const QString& GetPath() const;
    QTreeWidgetItem* GetRootItem();
    ProTreeItem* GetPreItem() const;
    ProTreeItem* GetNextItem() const;
    //获取最后一个子项
    ProTreeItem* GetLastPicChild();
    //获取第一个子项
    ProTreeItem* GetFirstPicChild();
    void SetPreItem(QTreeWidgetItem* preItem);
    void SetNextItem(QTreeWidgetItem* nextItem);

private:
    QString _path;
    QString _name;
    QTreeWidgetItem* _rootItem;
    QTreeWidgetItem* _preItem;
    QTreeWidgetItem* _nextItem;
};

#endif // PROTREEITEM_H
