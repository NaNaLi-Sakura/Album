/*
* 程序：protreethread.h
*
* 功能：独立线程，负责导入文件及文件夹
*
* 作者：雪与冰心丶
*
* 日期：2024/10/4
*/

#ifndef PROTREETHREAD_H
#define PROTREETHREAD_H

#include <QThread>
#include <QTreeWidgetItem>
#include <QTreeWidget>

class ProTreeThread : public QThread
{
    Q_OBJECT
signals:
    void sig_UpdateProgress(int file_count); //文件数量更新
    void sig_FinishProgress(); //文件导入完成

public:
    ProTreeThread(const QString& src_path, const QString& dst_path, QTreeWidgetItem* parent_item,
                  int& file_count, QTreeWidget* treeWidget, QTreeWidgetItem* root_item, QObject *parent = nullptr);
    ~ProTreeThread();

protected:
    //重写QThread的run方法
    void run() override;

public slots:
    //【新建项目】进度框取消的槽函数
    void slot_CancelProgress();

private:
    //生成目录树
    void CreateProTree(const QString& src_path, const QString& dst_path, QTreeWidgetItem* parent_item,
                       int& file_count, QTreeWidget* treeWidget, QTreeWidgetItem* root_item, QTreeWidgetItem* pre_item = nullptr);

    QString _src_path;
    QString _dst_path;
    int _file_count;
    QTreeWidgetItem* _parent_item;
    QTreeWidget* _treeWidget;
    QTreeWidgetItem* _root_item;
    bool _is_stop; //控制线程退出
};

#endif // PROTREETHREAD_H
