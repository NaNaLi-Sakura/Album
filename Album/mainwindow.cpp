#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QAction>
#include <QDebug>
#include "wizard.h"
#include <QFileDialog>
#include "protreewidget.h"
#include "picshow.h"
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //设置主窗口的基础尺寸比例
    this->setMinimumSize(1629, 869);

    ui->setupUi(this);

    //初始菜单栏
    initMenu();
}

MainWindow::~MainWindow()
{
    delete ui;

    //释放资源
    delete _proTree;
    delete _picShow;
}

//初始菜单栏
void MainWindow::initMenu()
{
    //创建“文件菜单”，设置快捷键
    QMenu* menu_file = ui->menubar->addMenu(tr("文件(&F)"));
    //为文件菜单添加动作项“创建项目”，设置快捷键
    QAction* act_create_pro = new QAction(QIcon(":/res/icon/createpro.png"), tr("新建项目"), this);
    act_create_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    menu_file->addAction(act_create_pro);
    //为文件菜单添加动作项“打开项目”，设置快捷键
    QAction* act_open_pro = new QAction(QIcon(":/res/icon/openpro.png"), tr("打开项目"), this);
    act_open_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    menu_file->addAction(act_open_pro);

    //创建“设置菜单”，设置快捷键
    QMenu* menu_set = menuBar()->addMenu(tr("设置(&S)"));
    //为设置菜单添加动作项“背景音乐”，设置快捷键
    QAction* act_music = new QAction(QIcon(":/res/icon/music.png"), tr("背景音乐"), this);
    act_music->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
    menu_set->addAction(act_music);

    //连接文件菜单的动作项的信号和槽
    connect(act_create_pro, &QAction::triggered, this, &MainWindow::slot_createPro);
    connect(act_open_pro, &QAction::triggered, this, &MainWindow::slot_openPro);

    //初始目录树界面，加入到左侧布局
    _proTree = new ProTree();
    ui->proLayout->addWidget(_proTree);
    //连接信号到目录树界面中的树形图
    QTreeWidget* treeWidget = dynamic_cast<ProTree*>(_proTree)->GetTreeWidget(); //获取树形图控件
    ProTreeWidget* pro_treeWidget = dynamic_cast<ProTreeWidget*>(treeWidget); //将树形图控件转成自定义派生类的类型
    connect(this, &MainWindow::sig_OpenPro, pro_treeWidget, &ProTreeWidget::slot_OpenPro);

    //初始图片展示界面，加入到右侧布局
    _picShow = new PicShow();
    ui->picLayout->addWidget(_picShow);

    //连接树形图控件的信号，到图片展示界面
    PicShow* pro_picShow = dynamic_cast<PicShow*>(_picShow);
    connect(pro_treeWidget, &ProTreeWidget::sig_ShowPic, pro_picShow, &PicShow::slot_ShowPic);

    //连接图片展示界面的按钮信号，到树形图控件
    connect(pro_picShow, &PicShow::sig_PreClicked, pro_treeWidget, &ProTreeWidget::slot_PreClicked);
    connect(pro_picShow, &PicShow::sig_NextClicked, pro_treeWidget, &ProTreeWidget::slot_NextClicked);
    //连接树形图控件的前后项信号，到图片显示界面
    connect(pro_treeWidget, &ProTreeWidget::sig_ReloadPic, pro_picShow, &PicShow::slot_ReloadPic);
    //连接树形图控件的关闭项目信号，到图片显示界面，清理图片
    connect(pro_treeWidget, &ProTreeWidget::sig_ClearSelected, pro_picShow, &PicShow::slot_ClearSelected);

    //连接文件菜单的动作项“背景音乐”的信号到树形图
    connect(act_music, &QAction::triggered, pro_treeWidget, &ProTreeWidget::slot_SetMusic);
}

//新建项目
void MainWindow::slot_createPro(bool checked)
{
    Q_UNUSED(checked)
//调试
//    qDebug() << "slot_createPro triggered.";

    //创建向导框，设置标题
    Wizard wizard(this);
    wizard.setWindowTitle(tr("创建项目"));

    //设置第一页的标题（第一页是项目配置页）
    QWizardPage* wizardPage1 = wizard.page(0);
    wizardPage1->setTitle(tr("设置项目配置"));
    //连接信号和槽，将项目配置传回来 todo...
    connect(&wizard, &Wizard::SigProSettings, dynamic_cast<ProTree*>(_proTree), &ProTree::AddProToTree);

    //显示向导框，模态阻塞
    wizard.show();
    wizard.exec();
    //向导框结束后，断开所有信号 todo...
    disconnect();
}

//导入项目
void MainWindow::slot_openPro(bool checked)
{
    Q_UNUSED(checked)

    //弹出文件选择框，文件模式为文件夹，设置框标题，基础目录，视图模型，模态显示
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle(tr("选择导入的文件夹"));
    fileDialog.setDirectory(QDir::currentPath());
    fileDialog.setViewMode(QFileDialog::Detail);
    QStringList fileList;
    if (fileDialog.exec()) {
        //获取选择的文件列表（实际上只需要第一个文件夹）
        fileList = fileDialog.selectedFiles();
    }
    //若没有选择，直接返回
    if (fileList.length() <= 0) return;

    //发送信号，带上获取到的原路径，发送给目录树对话框的树形图
    QString src_path = fileList.at(0);
    emit sig_OpenPro(src_path);
}

//捕获窗口尺寸变化事件
void MainWindow::resizeEvent(QResizeEvent *event)
{
    PicShow* pro_picShow = dynamic_cast<PicShow*>(_picShow);
    pro_picShow->ReloadPic();
    QMainWindow::resizeEvent(event); //不影响事件的正常流程
}
