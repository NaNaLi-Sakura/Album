#include "wizard.h"
#include "ui_wizard.h"

Wizard::Wizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::Wizard)
{
    ui->setupUi(this);
}

Wizard::~Wizard()
{
    delete ui;
}

//向导框完成之后的操作
void Wizard::done(int result)
{
    //判断选择，若选择取消，则不做处理
    if (result == QDialog::Rejected) {
        return QWizard::done(result);
    }
    //若选择确定，则获取文件名和路径名，开始处理
    QString name{}; QString path{};
    ui->wizardPage1->GetProSettings(name, path);
    emit SigProSettings(name, path); //发送带有路径信息的信号

    QWizard::done(result); //基类正常处理
}
