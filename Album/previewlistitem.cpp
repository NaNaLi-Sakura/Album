#include "previewlistitem.h"

PreviewListItem::PreviewListItem(const QIcon& icon, const QString& item_path, const int& index,
                                 QListWidget *listview, int type)
    : QListWidgetItem(icon, "", listview, type), _item_path(item_path), _index(index)
{

}

QString PreviewListItem::GetPath() const
{
    return _item_path;
}

int PreviewListItem::GetIndex() const
{
    return _index;
}
