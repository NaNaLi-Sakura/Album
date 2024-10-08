#include "openprothread.h"
#include <QDir>
#include "protreeitem.h"
#include "const.h"

OpenProThread::OpenProThread(const QString &src_path, int &file_count,
                             QTreeWidget *treeWidget, QObject *parent)
    : QThread(parent), _src_path(src_path), _file_count(file_count),
      _treeWidget(treeWidget), _rootItem(nullptr), _is_stop(false)
{

}

//打开项目生成目录树，此方法供run调用
void OpenProThread::OpenProToTree(const QString &src_path, int &file_count,
                                  QTreeWidget *treeWidget)
{
    //获取根路径的单目录名，作为根项的名称，创建根项（项目级）
    QDir src_dir(src_path);
    QString pro_name = src_dir.dirName();
    ProTreeItem* root_item = new ProTreeItem(treeWidget, pro_name, src_path, TreeItemPro);
    //设置根项的数据，记录根项
    root_item->setData(0, Qt::DisplayRole, pro_name);
    root_item->setData(0, Qt::DecorationRole, QIcon(":/res/icon/dir.png"));
    root_item->setData(0, Qt::ToolTipRole, src_path);
    _rootItem = root_item;

    //根项配置完毕，开始递归生成子项
    RecursiveProTree(src_path, file_count, treeWidget, _rootItem, _rootItem, nullptr);
}

void OpenProThread::run()
{
    //生成目录树
    OpenProToTree(_src_path, _file_count, _treeWidget);
    //检测退出
    if (_is_stop) {
        //删除目录树中的根项（即可删除所有项）
        int rootItemIndex = _treeWidget->indexOfTopLevelItem(_rootItem);
        delete _treeWidget->takeTopLevelItem(rootItemIndex);
        //无需删除本地文件，因为此功能仅仅是导入项目，不应该有删除本地项目的能力
        //若想删除本地项目，可以先导入，然后在关闭项目时选择同时删除本地项目
        return;
    }
    //发送完成信号（这个其实在OpenProToTree的最后是已经发了的，这里再发下也没什么）
    emit sig_FinishProgress(_file_count);
}

//【导入项目】进度框取消的槽函数
void OpenProThread::slot_CancelOpenProgress()
{
    this->_is_stop = true; //设置退出标志即可
}

//递归生成项
void OpenProThread::RecursiveProTree(const QString &src_path, int &file_count,
                                     QTreeWidget *treeWidget, QTreeWidgetItem *root_item,
                                     QTreeWidgetItem *parent_item, QTreeWidgetItem *pre_item)
{
    //获取原路径下过滤、排序后的文件信息列表（包含所有一级子文件及文件夹）
    QDir src_dir(src_path);
    src_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    src_dir.setSorting(QDir::Name);
    QFileInfoList fileInfoList = src_dir.entryInfoList();
    //遍历文件信息列表，若为文件夹则生成文件夹项，递归调用自己进入二级文件，若为文件则生成文件项
    for (int i = 0; i < fileInfoList.size(); ++i) {
        //检测退出，保证退出的高效
        if (_is_stop) return;
        //取出一个文件信息，判断是目录还是文件
        QFileInfo fileInfo = fileInfoList.at(i);
        bool is_dir = fileInfo.isDir();
        if (is_dir) {
            //目录。检测退出，更新文件数并发送文件数更新的信号
            if (_is_stop) return;
            file_count++;
            emit sig_UpdateProgress(file_count);
            //生成文件夹项
            ProTreeItem* dirItem = new ProTreeItem(parent_item, fileInfo.fileName(), fileInfo.absoluteFilePath(), root_item, TreeItemDir);
            dirItem->setData(0, Qt::DisplayRole, fileInfo.fileName());
            dirItem->setData(0, Qt::DecorationRole, QIcon(":/res/icon/dir.png"));
            dirItem->setData(0, Qt::ToolTipRole, fileInfo.absoluteFilePath());
            //递归调用该方法，进入当前目录，继续遍历。（此时的父级项就变成了当前项）
            RecursiveProTree(fileInfo.absoluteFilePath(), file_count, treeWidget, root_item, dirItem, pre_item);
        }
        else {
            //文件。检测退出
            if (_is_stop) return;
            //判断文件后缀，不合需求则跳过（我们需要的是图片文件）
            const QString& fileSuffix = fileInfo.completeSuffix();
            if ((fileSuffix != "png") && (fileSuffix != "jpeg") && (fileSuffix != "jpg")) continue;
            //下面是符合需求的文件，文件数加1，发送信号通知进度框
            file_count++;
            emit sig_UpdateProgress(file_count);
            //生成项
            ProTreeItem* fileItem = new ProTreeItem(parent_item, fileInfo.fileName(), fileInfo.absoluteFilePath(), root_item, TreeItemPic);
            fileItem->setData(0, Qt::DisplayRole, fileInfo.fileName());
            fileItem->setData(0, Qt::DecorationRole, QIcon(":/res/icon/pic.png"));
            fileItem->setData(0, Qt::ToolTipRole, fileInfo.absoluteFilePath());
            //与前项互相关联后，再成为前项
            if (pre_item) {
                ProTreeItem* pre_proItem = dynamic_cast<ProTreeItem*>(pre_item);
                pre_proItem->SetNextItem(fileItem);
            }
            fileItem->SetPreItem(pre_item);
            pre_item = fileItem;
        }
    }
    //全部项生成完成，向进度框发送完成的信号
    emit sig_FinishProgress(file_count);
}
