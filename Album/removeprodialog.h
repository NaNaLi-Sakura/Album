/*
* 程序：removeprodialog.h
*
* 功能：右键菜单“取消项目”的弹出的对话框
*
* 作者：雪与冰心丶
*
* 日期：2024/10/5
*/

#ifndef REMOVEPRODIALOG_H
#define REMOVEPRODIALOG_H

#include <QDialog>

namespace Ui {
class RemoveProDialog;
}

class RemoveProDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveProDialog(QWidget *parent = nullptr);
    ~RemoveProDialog();

    //获取是否删除本地的选择结果
    bool is_deleteLocal();

private:
    Ui::RemoveProDialog *ui;
};

#endif // REMOVEPRODIALOG_H
