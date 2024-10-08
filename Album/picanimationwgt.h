/*
* 程序：picanimationwgt.h
*
* 功能：幻灯片动画展示区域操作类
*
* 作者：雪与冰心丶
*
* 日期：2024/10/7
*/

#ifndef PICANIMATIONWGT_H
#define PICANIMATIONWGT_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QTimer>

class PicAnimationWgt : public QWidget
{
    Q_OBJECT    
public:
    explicit PicAnimationWgt(QWidget *parent = nullptr);
    ~PicAnimationWgt();

    //设置展示的图片
    void SetPixmap(QTreeWidgetItem* treeItem);
    //开启轮播
    void Start();
    //停止轮播
    void Stop();
    //前置切换
    void SlidePrevious();
    //后置切换
    void SlideNext();

protected:
    //重绘事件
    void paintEvent(QPaintEvent *event) override;

public slots:
    //列表项被点击，给动画区域发送带有点击项路径信息的信号，动画区域对应的槽函数
    void slot_UpdateSelectedShow(QString item_path);
    //动画播放按钮被点击，控制动画播放的启停
    void slot_StartOrStop();

private slots:
    //定时器超时信号的槽函数
    void slot_Timeout();

private:
    float _factor; //动画因子（0 - 1.0），用于控制动画显示
    QTimer* _timer; //定时器
    QPixmap _pixmap1; //双缓冲绘图，图1
    QPixmap _pixmap2; //双缓冲绘图，图2
    QTreeWidgetItem* _curr_item; //当前图片项
    QMap<QString, QTreeWidgetItem*> _item_map; //记录存储图片项 （路径 <=> 项）
    bool _is_start; //是否开始播放

    //加载动画
    void UpdateSelectedShow(QTreeWidgetItem* treeItem);

signals:
    //更新预览列表
    void sig_UpdatePreviewList(QTreeWidgetItem* treeItem);
    //同步预览列表选中项
    void sig_SelectedPreviewListItem(QTreeWidgetItem* treeItem);

    //动画区域的启停信号
    void sig_Start();
    void sig_Stop();

    //音乐播放启停信号
    void sig_StartMusic();
    void sig_StopMusic();

};

#endif // PICANIMATIONWGT_H
