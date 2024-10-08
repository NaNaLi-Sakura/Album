/*
* 程序：previewlistitem.h
*
* 功能：自定义操作预览列表中的项
*
* 作者：雪与冰心丶
*
* 日期：2024/10/7
*/

#ifndef PREVIEWLISTITEM_H
#define PREVIEWLISTITEM_H

#include <QListWidgetItem>

class PreviewListItem : public QListWidgetItem
{
public:
    PreviewListItem(const QIcon& icon, const QString& item_path, const int& index,
                    QListWidget *listview = nullptr, int type = Type);

    QString GetPath() const;
    int GetIndex() const;

private:
    QString _item_path;
    int _index;
};

#endif // PREVIEWLISTITEM_H
