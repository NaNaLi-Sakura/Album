/*
* 程序：openprothread.h
*
* 功能：独立线程，负责导入项目文件及文件夹
*
* 作者：雪与冰心丶
*
* 日期：2024/10/5
*/

#ifndef OPENPROTHREAD_H
#define OPENPROTHREAD_H

#include <QThread>
#include <QTreeWidget>

class OpenProThread : public QThread
{
    Q_OBJECT
signals:
    void sig_UpdateProgress(int file_count); //文件数量更新
    void sig_FinishProgress(int file_count); //导入完成
public:
    explicit OpenProThread(const QString& src_path, int& file_count,
                           QTreeWidget* treeWidget, QObject *parent = nullptr);
    //打开项目生成目录树，此方法供run调用
    void OpenProToTree(const QString& src_path, int& file_count, QTreeWidget* treeWidget);

protected:
    void run() override;

public slots:
    //【导入项目】进度框取消的槽函数
    void slot_CancelOpenProgress();

private:
    //递归生成项
    void RecursiveProTree(const QString& src_path, int& file_count, QTreeWidget* treeWidget,
                       QTreeWidgetItem* root_item, QTreeWidgetItem* parent_item, QTreeWidgetItem* pre_item);

    QString _src_path;
    int _file_count;
    QTreeWidget* _treeWidget;
    QTreeWidgetItem* _rootItem;
    bool _is_stop; //控制线程退出的标志
};

#endif // OPENPROTHREAD_H
