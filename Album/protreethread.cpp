#include "protreethread.h"
#include <QDir>
#include "protreeitem.h"
#include "const.h"

ProTreeThread::ProTreeThread(const QString &src_path, const QString &dst_path,
                             QTreeWidgetItem *parent_item, int& file_count,
                             QTreeWidget *treeWidget, QTreeWidgetItem *root_item, QObject *parent)
    : QThread(parent), _src_path(src_path), _dst_path(dst_path), _file_count(file_count),
      _parent_item(parent_item), _treeWidget(treeWidget), _root_item(root_item)
{

}

ProTreeThread::~ProTreeThread()
{

}

//重写QThread的run方法
void ProTreeThread::run()
{
    //生成目录树
    CreateProTree(_src_path, _dst_path, _parent_item, _file_count, _treeWidget, _root_item);
    //检测退出
    if (_is_stop) {
        //获取根节点项在树形控件上的索引，删除此根索引定位的根项，即可删除全部项
        //（根项相当于一个最大的父项了，父项删除，子项自然就全部消失了）
        QString root_path = dynamic_cast<ProTreeItem*>(_root_item)->GetPath(); //记录电脑上新生成是所有文件及文件夹的根目录
        int rootItemIndex = _treeWidget->indexOfTopLevelItem(_root_item);
        delete _treeWidget->takeTopLevelItem(rootItemIndex);
        //通过根节点项表示的绝对路径获取所有生成的文件及文件夹的根目录
        //从此根目录开始递归删除，即可删除具体生成的文件及文件夹
        //但是上面已经将目录树删除了，所以要在删除前记录下根节点项表示的路径
        QDir root_dir(root_path);
        root_dir.removeRecursively(); //递归删除所有

        return;
    }
    //发送完成信号
    emit sig_FinishProgress();
}

//进度框取消的槽函数
void ProTreeThread::slot_CancelProgress()
{
    this->_is_stop = true; //设置退出标志即可
}

//生成目录树
void ProTreeThread::CreateProTree(const QString &src_path, const QString &dst_path, QTreeWidgetItem *parent_item, int& file_count, QTreeWidget *treeWidget, QTreeWidgetItem* root_item, QTreeWidgetItem *pre_item)
{
    //检测退出
    if (_is_stop) return;
    //检查是否需要拷贝
    bool is_need_copy = true; //是否需要拷贝
    if (src_path == dst_path) is_need_copy = false;
    //过滤原路径，余下需要的格式
    QDir src_dir(src_path);
    src_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    //设置按文件名排序
    src_dir.setSorting(QDir::Name);
    //获取原路径的文件信息列表（已过滤）
    QFileInfoList fileInfoList = src_dir.entryInfoList();
    for (int i = 0; i < fileInfoList.size(); ++i) {
        //检测退出
        if (_is_stop) return;
        //取出一个文件信息，判断是目录还是文件
        QFileInfo fileInfo = fileInfoList.at(i);
        bool is_dir = fileInfo.isDir();
        if (is_dir) {
            //目录
            if (_is_stop) return; //先检测退出，保证退出的高效
            file_count++; //文件数加1（文件夹也算数量）
            emit sig_UpdateProgress(file_count); //发送信号通知进度框

            //根据“此文件夹名 + 目标目录”生成目标文件夹绝对路径
            QDir dst_dir(dst_path);
            QString dst_full_path = dst_dir.absoluteFilePath(fileInfo.fileName());
            //准备创建新文件夹
            QDir dst_full_dir(dst_full_path);
            if (!dst_full_dir.exists()) {
                bool mkdir_ok = dst_full_dir.mkpath(dst_full_path);
                if (!mkdir_ok) continue; //创建失败，跳过，下一个
            }
            //文件夹创建成功，生成文件夹项
            ProTreeItem* dirItem = new ProTreeItem(parent_item, fileInfo.fileName(), dst_full_path, root_item, TreeItemDir);
            dirItem->setData(0, Qt::DisplayRole, fileInfo.fileName());
            dirItem->setData(0, Qt::DecorationRole, QIcon(":/res/icon/dir.png"));
            dirItem->setData(0, Qt::ToolTipRole, dst_full_path);
            //读出的是文件夹，在控件中创建文件夹项的工作已完成，接下来要再次读取，导入文件夹内部的文件
            //递归调用时，原路径就是读出来的文件夹在电脑上的绝对路径，而非目录树中的生成项的绝对路径
            //因为我们的目录树仅仅是保证了目录结构一致，不能保证前面的路径是一致的
            //父节点项自然就是刚才在目录树中新生成的目录项，下一步就是要将文件生成在该目录项之中
            CreateProTree(fileInfo.absoluteFilePath(), dst_full_path, dirItem, file_count, treeWidget, root_item, pre_item);
        }
        else {
            //文件
            if (_is_stop) return; //先检测退出，保证退出的高效
            //判断文件后缀，不合需求则跳过（我们需要的是图片文件）
            QString fileSuffix = fileInfo.completeSuffix();
            if ((fileSuffix != "png") && (fileSuffix != "jpeg") && (fileSuffix != "jpg")) continue;

            //下面是符合需求的文件，文件数加1，发送信号通知进度框
            file_count++;
            emit sig_UpdateProgress(file_count);
            //检测是否需要拷贝
            if (!is_need_copy) continue;
            //生成目标的绝对路径，开始拷贝文件
            QDir dst_dir(dst_path);
            QString dst_full_path = dst_dir.absoluteFilePath(fileInfo.fileName());
            if (!QFile::copy(fileInfo.absoluteFilePath(), dst_full_path)) {
                continue; //拷贝失败，跳过该文件，继续下一个
            }
            //生成项
            ProTreeItem* proTreeItem = new ProTreeItem(parent_item, fileInfo.fileName(), dst_full_path, root_item, TreeItemPic);
            proTreeItem->setData(0, Qt::DisplayRole, fileInfo.fileName());
            proTreeItem->setData(0, Qt::DecorationRole, QIcon(":/res/icon/pic.png"));
            proTreeItem->setData(0, Qt::ToolTipRole, dst_full_path);
            //与前项互相关联后，再成为前项
            if (pre_item) {
                ProTreeItem* pre_proItem = dynamic_cast<ProTreeItem*>(pre_item);
                pre_proItem->SetNextItem(proTreeItem);
            }
            proTreeItem->SetPreItem(pre_item);
            pre_item = proTreeItem;
        }
    }
}
