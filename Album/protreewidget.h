/*
* 程序：protreewidget.h
*
* 功能：操作目录树中的treeWidget控件。
*
* 作者：雪与冰心丶
*
* 日期：2024/10/3
*/

#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include <QTreeWidget>
#include <QAction>
#include <QProgressDialog>
#include "protreethread.h"
#include "openprothread.h"
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>

//幻灯片播放界面的前置声明
class SlideShow;

class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT
signals:
    void sig_CancelProgress(); //【新建项目】表示进度框点击了取消
    void sig_CancelOpenProgress(); //【导入项目】表示进度框点击了取消
    void sig_ShowPic(const QString& pic_path); //双击项
    void sig_ReloadPic(const QString& pic_path); //按钮切换前后项
    void sig_ClearSelected(); //关闭项目，清理当前选中项的图片展示

public:
    explicit ProTreeWidget(QWidget *parent = nullptr);

public slots:
    //项目配置完毕的信号槽 生成目录树
    void AddProToTree(const QString& name, const QString& path);
    //前项
    void slot_PreClicked();
    //后项
    void slot_NextClicked();
    //文件菜单项“背景音乐”的槽函数
    void slot_SetMusic();
    //启动音乐
    void slot_StartMusic();
    //停止音乐
    void slot_StopMusic();

private slots:
    //treeWidget内按下鼠标按钮触发的信号槽函数
    void slot_itemPressed(QTreeWidgetItem *pressedItem, int column);
    //导入文件的槽函数
    void slot_Import();
    //【新建项目】文件数量更新的信号槽函数，需要更新进度框
    void slot_UpdateProgress(int file_count);
    //【新建项目】文件导入完成的信号槽函数
    void slot_FinishProgress();
    //【导入项目】文件数量更新的信号槽函数，需要更新进度框
    void slot_UpdateOpenProgress(int file_count);
    //【导入项目】文件导入完成的信号槽函数
    void slot_FinishOpenProgress();
    //【新建项目】进度框的取消信号的槽函数
    void slot_CancelProgress();
    //【导入项目】进度框的取消信号的槽函数
    void slot_CancelOpenProgress();
    //设置活动项目的槽函数
    void slot_SetActive();
    //关闭项目的槽函数
    void slot_ClosePro();
    //treeWidget内双击鼠标按钮触发的信号槽函数
    void slot_ItemDoubleClicked(QTreeWidgetItem *doubleItem, int column);
    //轮播图播放的槽函数
    void slot_SlideShow();
public slots:
    //导入项目的槽函数，参数为项目文件夹的原路径
    void slot_OpenPro(const QString& src_path);

private:
    QSet<QString> _path_set; //存放路径
    QTreeWidgetItem* _right_btn_item; //触发右键菜单时点击的项
    QTreeWidgetItem* _active_item; //当前活动项目
    QTreeWidgetItem* _selected_item; //当前选择的项目
    QAction* _act_import; //导入文件的动作项
    QAction* _act_setstart; //设置活动项的动作项
    QAction* _act_closepro; //关闭项目的动作项
    QAction* _act_slideshow; //幻灯片放映的动作项
    QProgressDialog* _progress_dialog; //新建项目进度对话框
    QProgressDialog* _open_progress_dlg; //打开项目进度对话框
    std::shared_ptr<ProTreeThread> _protreethread_ptr; //导入文件线程
    std::shared_ptr<OpenProThread> _openprothread_ptr; //打开项目线程
    std::shared_ptr<SlideShow> _slide_show_dlg; //幻灯片播放界面
    QMediaPlayer* _player; //音频接口
    QMediaPlaylist* _playList; //媒体播放列表
};

#endif // PROTREEWIDGET_H
