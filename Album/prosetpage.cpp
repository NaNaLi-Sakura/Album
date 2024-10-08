#include "prosetpage.h"
#include "ui_prosetpage.h"
#include <QDir>
#include <QFileDialog>

ProSetPage::ProSetPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ProSetPage)
{
    ui->setupUi(this);

    //设置注册域
    registerField("proName", ui->lE_Name);
    registerField("proPath", ui->lE_Path);
    //关联输入框的信号槽
    connect(ui->lE_Name, &QLineEdit::textEdited, this, &ProSetPage::completeChanged);
    connect(ui->lE_Path, &QLineEdit::textEdited, this, &ProSetPage::completeChanged);
    //设置路径框的文本默认为当前路径
    ui->lE_Path->setText(QDir::currentPath());
    //设置路径框的光标位置为文本末尾
    ui->lE_Path->setCursorPosition(ui->lE_Path->text().size());
    //设置框支持清空
    ui->lE_Name->setClearButtonEnabled(true);
    ui->lE_Path->setClearButtonEnabled(true);

}

ProSetPage::~ProSetPage()
{
    delete ui;
}

//获取项目配置（文件名和路径名）
void ProSetPage::GetProSettings(QString &name, QString &path) const
{
    name = ui->lE_Name->text();
    path = ui->lE_Path->text();
}

//行输入框编辑完成的槽函数
bool ProSetPage::isComplete() const
{
    //检测行框中的内容，任意一框为空则不允许"Next"
    if ((ui->lE_Name->text() == "") || (ui->lE_Path->text() == "")) {
        ui->lb_Tips->setText("Name or Path can't be null!");
        return false;
    }
    //检测路径是否存在
    QFileInfo pathInfo(ui->lE_Path->text());
    if (!pathInfo.exists()) {
        //若不存在，则设置提示内容，并且不允许"Next"
        ui->lb_Tips->setText("Project path is not exists!");
        return false;
    }
    //检测项目的绝对路径是否存在（路径 + 项目名）
//下面这种写法好像不太对，判断路径应该用的是QFileInfo
//    QString absFilePath = pathDir.absoluteFilePath(ui->lE_Name->text());
//    QDir dst_dir(absFilePath);
//    if (dst_dir.exists())
    QFileInfo fileInfo(ui->lE_Path->text() + QDir::separator() + ui->lE_Name->text());
    if (fileInfo.exists()) {
        //若不存在，则设置提示内容，并且不允许"Next"
        ui->lb_Tips->setText("Project already exists, please change path or name!");
        return false;
    }

    //条件符合，允许继续
    ui->lb_Tips->setText("The current project can be create!"); //设置为空也行
    return QWizardPage::isComplete(); //再次调用基类的此方法
}

//浏览目录
void ProSetPage::on_pbtn_Browse_clicked()
{
    //创建文件对话框，设置文件模式为目录模式，设置标题
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle("选择导入的文件夹");
    //设置文件框的路径，视图模式
    fileDialog.setDirectory(QDir::currentPath());
    fileDialog.setViewMode(QFileDialog::Detail);

    //若用户选择确定，存储所选文件夹的绝对路径的字符串列表
    QStringList fullPathList;
    if (fileDialog.exec()) {
        fullPathList = fileDialog.selectedFiles();
    }
    //若列表内容为空，直接返回
    if (fullPathList.length() <= 0) return;
    //设置路径框的内容为所选路径列表的第一个绝对路径
    ui->lE_Path->setText(fullPathList.at(0));
}
