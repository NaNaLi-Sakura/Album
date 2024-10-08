/*
* 程序：prosetpage.h
*
* 功能：菜单动作项“新建项目”的向导框的第1页“项目配置页”界面类
*
* 作者：雪与冰心丶
*
* 日期：2024/10/3
*/

#ifndef PROSETPAGE_H
#define PROSETPAGE_H

#include <QWizardPage>

namespace Ui {
class ProSetPage;
}

class ProSetPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ProSetPage(QWidget *parent = nullptr);
    ~ProSetPage();

    //获取项目配置（文件名和路径名）
    void GetProSettings(QString& name, QString& path) const;

protected:
    //行输入框编辑完成的槽函数
    bool isComplete() const override;

private slots:
    void on_pbtn_Browse_clicked();

private:
    Ui::ProSetPage *ui;
};

#endif // PROSETPAGE_H
