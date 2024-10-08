#include "removeprodialog.h"
#include "ui_removeprodialog.h"

RemoveProDialog::RemoveProDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RemoveProDialog)
{
    ui->setupUi(this);
}

RemoveProDialog::~RemoveProDialog()
{
    delete ui;
}

//获取是否删除本地的选择结果
bool RemoveProDialog::is_deleteLocal()
{
    return ui->ckbx_deleteLocal->isChecked();
}
