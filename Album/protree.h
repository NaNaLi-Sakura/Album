/*
* 程序：protree.h
*
* 功能：目录树界面类
*
* 作者：雪与冰心丶
*
* 日期：2024/10/3
*/

#ifndef PROTREE_H
#define PROTREE_H

#include <QDialog>
#include <QTreeWidget>

namespace Ui {
class ProTree;
}

class ProTree : public QDialog
{
    Q_OBJECT

public:
    explicit ProTree(QWidget *parent = nullptr);
    ~ProTree();

    //获取树形图控件
    QTreeWidget* GetTreeWidget();

public slots:
    //项目配置完毕的信号槽 生成目录树
    void AddProToTree(const QString name, const QString path);

private:
    Ui::ProTree *ui;
};

#endif // PROTREE_H
