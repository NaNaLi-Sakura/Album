/*
* 程序：const.h
*
* 功能：定义一些程序中用到的常量。
*
* 作者：雪与冰心丶
*
* 日期：2024/10/3
*/

#ifndef CONST_H
#define CONST_H

//目录树中的项类型
enum TreeItemType {
    TreeItemPro = 1, //项目级
    TreeItemDir = 2, //文件夹级
    TreeItemPic = 3, //图片文件
};

//动画播放按钮的状态
enum PicBtnState {
    PlayNormalState = 1, //播放-正常状态
    PlayHoverState = 2, //播放-悬停状态
    PlayPressedState = 3, //播放-点击状态

    PauseNormalState = 4, //暂停-正常状态
    PauseHoverState = 5, //暂停-悬停状态
    PausePressedState = 6, //暂停-点击状态
};

const int PROGRESS_WIDTH = 300; //进度对话框固定宽度
const int PROGRESS_MAX = 300; //进度框数值的最大值

const int PREVIEW_ITEM_SIZE = 100; //预览缩略图项大小
const int PREVIEW_ICON_SIZE = 90; //预览缩略图图标大小

#endif // CONST_H
