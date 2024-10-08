#include "protree.h"
#include "ui_protree.h"

ProTree::ProTree(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProTree)
{
    ui->setupUi(this);

    //设置目录树界面的固定宽度
    this->setMinimumWidth(378);
    this->setMaximumWidth(378);
}

ProTree::~ProTree()
{
    delete ui;
}

//获取树形图控件
QTreeWidget *ProTree::GetTreeWidget()
{
    return ui->treeWidget;
}

//项目配置完毕的信号槽 生成目录树
void ProTree::AddProToTree(const QString name, const QString path)
{
    //主要操作treeWidget控件
    ui->treeWidget->AddProToTree(name, path);
}
