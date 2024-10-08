/*
* 程序：confirmpage.h
*
* 功能：菜单动作项“新建项目”的向导框的第2页“确认页”界面类
*
* 作者：雪与冰心丶
*
* 日期：2024/10/3
*/

#ifndef CONFIRMPAGE_H
#define CONFIRMPAGE_H

#include <QWizardPage>

namespace Ui {
class ConfirmPage;
}

class ConfirmPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ConfirmPage(QWidget *parent = nullptr);
    ~ConfirmPage();

private:
    Ui::ConfirmPage *ui;
};

#endif // CONFIRMPAGE_H
