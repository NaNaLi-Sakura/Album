#include "protreewidget.h"
#include <QDir>
#include "protreeitem.h"
#include "const.h"
#include <QHeaderView>
#include <QGuiApplication>
#include <QMenu>
#include <QCursor>
#include <QFileDialog>
#include <QDebug>
#include "removeprodialog.h"
#include "slideshow.h"

ProTreeWidget::ProTreeWidget(QWidget *parent)
    : QTreeWidget(parent), _right_btn_item(nullptr), _active_item(nullptr),
      _selected_item(nullptr), _progress_dialog(nullptr), _open_progress_dlg(nullptr),
      _protreethread_ptr(nullptr), _openprothread_ptr(nullptr)
{
    this->header()->hide();

    //关联treeWidget内鼠标点击的信号，创建右键菜单
    connect(this, &ProTreeWidget::itemPressed, this, &ProTreeWidget::slot_itemPressed);

    //初始化右键菜单的动作项
    _act_import = new QAction(QIcon(":/res/icon/import.png"), tr("导入文件"), this);
    _act_setstart = new QAction(QIcon(":/res/icon/core.png"), tr("设置活动项目"), this);
    _act_closepro = new QAction(QIcon(":/res/icon/close.png"), tr("关闭项目"), this);
    _act_slideshow = new QAction(QIcon(":/res/icon/slideshow.png"), tr("轮播图播放"), this);
    //关联动作项的信号槽
    connect(_act_import, &QAction::triggered, this, &ProTreeWidget::slot_Import);
    connect(_act_setstart, &QAction::triggered, this, &ProTreeWidget::slot_SetActive);
    connect(_act_closepro, &QAction::triggered, this, &ProTreeWidget::slot_ClosePro);
    connect(_act_slideshow, &QAction::triggered, this, &ProTreeWidget::slot_SlideShow);

    //关联treeWidget内鼠标双击的信号，准备显示图片
    connect(this, &ProTreeWidget::itemDoubleClicked, this, &ProTreeWidget::slot_ItemDoubleClicked);

    //初始化音频接口和列表
    _player = new QMediaPlayer(this);
    _playList = new QMediaPlaylist(this);
    _playList->setPlaybackMode(QMediaPlaylist::Loop);
    _player->setPlaylist(_playList);
}

//项目配置完毕的信号槽 生成目录树
void ProTreeWidget::AddProToTree(const QString &name, const QString &path)
{
    //检测路径是否重复
    QDir pathDir(path);
    QString filePath = pathDir.absoluteFilePath(name);
    if (_path_set.find(filePath) != _path_set.end()) return; //路径重复则直接返回

    //为新项目创建新路径（文件夹）
    QDir proDir(filePath);
    if (!proDir.exists()) {
        bool enable = proDir.mkpath(filePath);
        if (!enable) return; //创建失败，直接返回
    }

    _path_set.insert(filePath); //记录新路径

    //添加项（项目级）
    ProTreeItem* proTreeItem = new ProTreeItem(this, name, filePath, TreeItemPro);
    proTreeItem->setData(0, Qt::DisplayRole, name);
    proTreeItem->setData(0, Qt::DecorationRole, QIcon(":/res/icon/dir.png"));
    proTreeItem->setData(0, Qt::ToolTipRole, filePath);
    this->addTopLevelItem(proTreeItem);
//调试
//    int itemType = proTreeItem->type();
//    qDebug() << "项类型：" << itemType;
    //    if (itemType == TreeItemPro) qDebug() << "项类型确实是项目级";
}

//前项
void ProTreeWidget::slot_PreClicked()
{
    //获取当前项(选中项)的前一项，设置为选中项、当前项，将图片路径发送信号给图片展示界面
    if (!_selected_item) return;
    ProTreeItem* preItem = dynamic_cast<ProTreeItem*>(_selected_item)->GetPreItem();
    if (!preItem) return;

    _selected_item = preItem;
    this->setCurrentItem(preItem);
    emit sig_ReloadPic(preItem->GetPath());
}

//后项
void ProTreeWidget::slot_NextClicked()
{
    //获取当前项(选中项)的后一项，设置为选中项、当前项，将图片路径发送信号给图片展示界面
    if (!_selected_item) return;
    ProTreeItem* nextItem = dynamic_cast<ProTreeItem*>(_selected_item)->GetNextItem();
    if (!nextItem) return;

    _selected_item = nextItem;
    this->setCurrentItem(nextItem);
    emit sig_ReloadPic(nextItem->GetPath());
}

//文件菜单项“背景音乐”的槽函数
void ProTreeWidget::slot_SetMusic()
{
    //创建音乐文件选择对话框，设置文件模式、窗口标题、起始目录、视图模式、过滤器
    QFileDialog musicFileDialog;
    musicFileDialog.setFileMode(QFileDialog::ExistingFiles);
    musicFileDialog.setWindowTitle(tr("选择音频文件"));
    musicFileDialog.setDirectory(QDir::currentPath());
    musicFileDialog.setViewMode(QFileDialog::Detail);
    musicFileDialog.setNameFilter("(*.mp3)");
    //模态弹出，获取用户选择后的音频文件列表
    QStringList musicFileList;
    if (musicFileDialog.exec()) {
        musicFileList = musicFileDialog.selectedFiles();
    }
    else return; //若取消对话框，则不做任何操作
    if (musicFileList.length() <= 0) return; //再检测下音频列表，防止出错
    //清空原来的音频列表，将此次选择的音频列表存储记录下来
    _playList->clear();
    for (auto& musicFile : musicFileList) {
        _playList->addMedia(QUrl::fromLocalFile(musicFile));
    }
    //若音频状态不是正在播放，就将当前音乐列表索引置0（我们的音乐播放模式是单曲循环，所以其实主要关注第一项即可）
    if (_player->state() != QMediaPlayer::PlayingState) {
        _playList->setCurrentIndex(0); //意味着播放列表中的第一项将成为当前项。
    }
    //若正在播放音乐，则不做处理
}

//启动音乐
void ProTreeWidget::slot_StartMusic()
{
    _player->play();
}

//停止音乐
void ProTreeWidget::slot_StopMusic()
{
    _player->stop();
}

//treeWidget内按下鼠标按钮触发的信号槽函数
void ProTreeWidget::slot_itemPressed(QTreeWidgetItem *pressedItem, int column)
{
    Q_UNUSED(column)
    //在点击右键的前提下
    if (QGuiApplication::mouseButtons() == Qt::RightButton) {
//调试
//qDebug() << "点击了右键";
        //创建菜单
        QMenu menu(this);
        //在点击项为项目级时
        int itemType = pressedItem->type();
        if (itemType == TreeItemPro) {
//调试
//qDebug() << "右键点击的是项目级";
            _right_btn_item = pressedItem; //存储点击项
            //为右键菜单添加动作项，在鼠标处弹出右键菜单
            menu.addAction(_act_import);
            menu.addAction(_act_setstart);
            menu.addAction(_act_closepro);
            menu.addAction(_act_slideshow);
            menu.exec(QCursor::pos());
        }
    }

}

//导入文件的槽函数
void ProTreeWidget::slot_Import()
{
    //弹出文件对话框，设置文件模式，设置标题
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle(tr("选择导入的文件夹"));
    if (!_right_btn_item) {
        //右键点击的项出现错误
        qDebug() << "_right_btn_item is empty.";
        return;
    }
    //设置对话框根目录，设置视图模型
    QString path = dynamic_cast<ProTreeItem*>(_right_btn_item)->GetPath();
    fileDialog.setDirectory(path);
    fileDialog.setViewMode(QFileDialog::Detail);
    //采用列表存放选择的文件
    QStringList fileList;
    if (fileDialog.exec()) {
        fileList = fileDialog.selectedFiles();
    }
    if (fileList.length() <= 0) return;
    //第一个文件夹路径作为导入的原路径，目的路径就是右键项表示的路径
    QString src_path = fileList.at(0);
    int file_count = 0; //初始文件数量
    //创建进度对话框
    _progress_dialog = new QProgressDialog(this);

    //创建导入线程
    qRegisterMetaType<QVector<int>>("QVector<int>");//注册新类型
    _protreethread_ptr = std::make_shared<ProTreeThread>(std::ref(src_path), std::ref(path), _right_btn_item,
                                                         file_count, this, _right_btn_item, nullptr);
    //关联导入线程的信号，使进度框作出调整
    connect(_protreethread_ptr.get(), &ProTreeThread::sig_UpdateProgress, this, &ProTreeWidget::slot_UpdateProgress);
    connect(_protreethread_ptr.get(), &ProTreeThread::sig_FinishProgress, this, &ProTreeWidget::slot_FinishProgress);
    //关联进度框的取消信号，对应槽函数发出自定义信号，关联此信号，发送给导入线程，设置退出标志即可
    connect(_progress_dialog, &QProgressDialog::canceled, this, &ProTreeWidget::slot_CancelProgress);
    connect(this, &ProTreeWidget::sig_CancelProgress, _protreethread_ptr.get(), &ProTreeThread::slot_CancelProgress);
    _protreethread_ptr->start(); //启动导入线程

    //设置进度对话框标题，设置固定宽度，设置波动范围，模态显示
    _progress_dialog->setWindowTitle("Please wait...");
    _progress_dialog->setFixedWidth(PROGRESS_WIDTH);
    _progress_dialog->setRange(0, PROGRESS_MAX);
    _progress_dialog->exec();
}

//文件数量更新的信号槽函数，需要更新进度框
void ProTreeWidget::slot_UpdateProgress(int file_count)
{
    //检测进度框的存在
    if (!_progress_dialog) return;

    //进度框的数值是固定的，文件数量超过的话，就用余数更新即可
    //文件数量不超，直接更新，但是结果和余数是一致的，所以同一用余数即可
    _progress_dialog->setValue(file_count % PROGRESS_MAX);
}

//文件导入完成的信号槽函数
void ProTreeWidget::slot_FinishProgress()
{
    //更新进度为最大值，关闭进度框
    _progress_dialog->setValue(PROGRESS_MAX);
    _progress_dialog->deleteLater();
}

//【导入项目】文件数量更新的信号槽函数，需要更新进度框
void ProTreeWidget::slot_UpdateOpenProgress(int file_count)
{
    //检测进度框的存在
    if (!_open_progress_dlg) return;

    //更新进度框的数值
    _open_progress_dlg->setValue(file_count % PROGRESS_MAX);
}

//【导入项目】文件导入完成的信号槽函数
void ProTreeWidget::slot_FinishOpenProgress()
{
    //检测进度框的存在
    if (!_open_progress_dlg) return;

    //更新进度为最大值，关闭进度框
    _open_progress_dlg->setValue(PROGRESS_MAX);
    delete _open_progress_dlg;
    _open_progress_dlg = nullptr;
}

//进度框的取消信号的槽函数
void ProTreeWidget::slot_CancelProgress()
{
    //发送信号给导入线程，回收释放进度框
    emit sig_CancelProgress();

    delete _progress_dialog;
    _progress_dialog = nullptr;
}

//【导入项目】进度框的取消信号的槽函数
void ProTreeWidget::slot_CancelOpenProgress()
{
    //发送信号给导入线程，回收释放进度框
    emit sig_CancelOpenProgress();

    delete _open_progress_dlg;
    _open_progress_dlg = nullptr;
}

//设置活动项目的槽函数
void ProTreeWidget::slot_SetActive()
{
    //检测右键点击项
    if (!_right_btn_item) return;
    //若存在原启动项，则将其恢复为非启动项（不加粗的字体）
    QFont defaultFont; defaultFont.setBold(false);
    if (_active_item) _active_item->setFont(0, defaultFont);
    //存储当前右击项为活动项目，并设置为活动项（加粗的字体）
    _active_item = _right_btn_item;
    defaultFont.setBold(true); _active_item->setFont(0, defaultFont);
}

//关闭项目的槽函数
void ProTreeWidget::slot_ClosePro()
{
    //创建关闭项目的对话框，模态弹出
    RemoveProDialog removeProDialog;
    int result = removeProDialog.exec();
    //若用户没有选择确定，则不做任何处理，直接返回
    if (result != QDialog::Accepted) return;

    //获取当前项（右键项）所表示的绝对路径，从路径集合中删除其记录
    ProTreeItem* currItem = dynamic_cast<ProTreeItem*>(_right_btn_item);
    QString delete_path = currItem->GetPath();
    _path_set.remove(delete_path);
    //获取选中项（双击项），如果当前项(右键项)是选中项的根节点项，则清理选中项，发送信号，清理图片
    ProTreeItem* selectedItem = dynamic_cast<ProTreeItem*>(_selected_item);
    if ((selectedItem) && (currItem == selectedItem->GetRootItem())) {
        selectedItem = nullptr;
        _selected_item = nullptr;
        emit sig_ClearSelected();
    }
    //判断是否删除本地文件
    bool is_deleteLocal = removeProDialog.is_deleteLocal();
    if (is_deleteLocal) {
        //从本地递归删除当前项表示的目录即可
        QDir delete_dir(delete_path);
        delete_dir.removeRecursively();
    }
    //若当前项本就是启动项，则将启动项置空（即将删除了嘛）
    if (_right_btn_item == _active_item) _active_item = nullptr;
    //获取当前项（右键项）在树形图中的索引，删除目录树中的当前项
    int index_right_item = this->indexOfTopLevelItem(_right_btn_item);
    delete this->takeTopLevelItem(index_right_item);
    //将右键项置空（已经删了嘛）
    _right_btn_item = nullptr;
}

//treeWidget内双击鼠标按钮触发的信号槽函数
void ProTreeWidget::slot_ItemDoubleClicked(QTreeWidgetItem *doubleItem, int column)
{
    Q_UNUSED(column)

    //在双击按钮是左键的前提下
    if (QGuiApplication::mouseButtons() == Qt::LeftButton) {
        //获取双击项的类型，只有图片类型才需要处理（使用自定义项的方法，要先转换下类型）
        ProTreeItem* doubleProItem = dynamic_cast<ProTreeItem*>(doubleItem);
        if (!doubleProItem) return; //尽可能安全编程
        int itemType = doubleProItem->type();
        if (itemType == TreeItemPic) {
            //发送带有路径信息的信号
            emit sig_ShowPic(doubleProItem->GetPath());
            //更新记录当前选中项（双击项）
            _selected_item = doubleItem;
        }
    }
}

//轮播图播放的槽函数
void ProTreeWidget::slot_SlideShow()
{
    //检测右键点击项
    if (!_right_btn_item) return;
    //根据当前项(右键项)获取第一项和最后一项
    ProTreeItem* pro_right_item = dynamic_cast<ProTreeItem*>(_right_btn_item);
    ProTreeItem* first_child_item =  pro_right_item->GetFirstPicChild();
    if (!first_child_item) return;
    ProTreeItem* last_child_item = pro_right_item->GetLastPicChild();
    if (!last_child_item) return;
//调试
//qDebug() << "first_child_item: " << first_child_item->GetPath();
//qDebug() << "last_child_item: " << last_child_item->GetPath();
    //根据第一项和最后一项构造幻灯片界面，模态全屏显示
    _slide_show_dlg = std::make_shared<SlideShow>(this, first_child_item, last_child_item);
    _slide_show_dlg->setModal(true);
    _slide_show_dlg->showMaximized();
}

//导入项目的槽函数，参数为项目文件夹的原路径
void ProTreeWidget::slot_OpenPro(const QString &src_path)
{
    //若路径已经记录，说明此项目已经在目录树中了，直接返回，即无法重复导入
    if (_path_set.find(src_path) != _path_set.end()) return;

    //存储原路径，设置初始文件数量，生成项目名
    _path_set.insert(src_path);
    int file_count = 0;
    QDir pro_dir(src_path);
    QString pro_name = pro_dir.dirName(); //单文件夹名作为项目名
    //创建进度框，创建导入线程，关联导入线程的信号给到进度框，启动线程，设置进度框信息，模态显示...
    _open_progress_dlg = new QProgressDialog(this);
    //创建导入线程
    qRegisterMetaType<QVector<int>>("QVector<int>");//注册新类型
    _openprothread_ptr = std::make_shared<OpenProThread>(src_path, file_count, this, nullptr);
    //关联导入线程的信号，使进度框作出调整
    connect(_openprothread_ptr.get(), &OpenProThread::sig_UpdateProgress, this, &ProTreeWidget::slot_UpdateOpenProgress);
    connect(_openprothread_ptr.get(), &OpenProThread::sig_FinishProgress, this, &ProTreeWidget::slot_FinishOpenProgress);
    //关联进度框的取消信号，对应槽函数发出自定义信号，关联此信号，发送给导入线程，设置退出标志即可
    connect(_open_progress_dlg, &QProgressDialog::canceled, this, &ProTreeWidget::slot_CancelOpenProgress);
    connect(this, &ProTreeWidget::sig_CancelOpenProgress, _openprothread_ptr.get(), &OpenProThread::slot_CancelOpenProgress);
    _openprothread_ptr->start(); //启动导入线程
    //设置进度框信息，模态显示...
    _open_progress_dlg->setWindowTitle("Please wait...");
    _open_progress_dlg->setFixedWidth(PROGRESS_WIDTH);
    _open_progress_dlg->setRange(0, PROGRESS_MAX);
    _open_progress_dlg->exec();
}
