/*
* 程序：wizard.h
*
* 功能：菜单动作项“新建项目”的向导框
*
* 作者：雪与冰心丶
*
* 日期：2024/10/3
*/

#ifndef WIZARD_H
#define WIZARD_H

#include <QWizard>

namespace Ui {
class Wizard;
}

class Wizard : public QWizard
{
    Q_OBJECT

public:
    explicit Wizard(QWidget *parent = nullptr);
    ~Wizard();

signals:
    //根据路径生成目录树的信号
    void SigProSettings(const QString name, const QString path);

protected:
    //向导框完成之后的操作
    void done(int result) override;

private:
    Ui::Wizard *ui;
};

#endif // WIZARD_H
