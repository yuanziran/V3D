#pragma once
#include"../qcustomplot.h"
#include<QMainWindow>
#include<QSlider>
#include<QMenu>
#include<QAction>
#include<QString>
namespace V3D
{
    /**
     * @brief 播放一个时间序列图像集合
    */
    class QXMove : public QMainWindow
    {
        Q_OBJECT
        public:
        QXMove(QWidget* parent=0);
        protected:
        void initUI();                                  ///<初始化UI
        private:
        QCustomPlot*        ui_handle;                  ///<绘图窗口
        QLabel*             ui_frame_num;               ///<显示帧数
        QSlider*            ui_time_slider;             ///<时间进度条
        QLineEdit*          ui_timepoint_edit;          ///<时间值
        QTimer*             ui_timer;                   ///<定时器对象
        QMenu*              ui_file_menu;               ///<文件菜单
        QMenu*              ui_edit_menu;               ///<编辑菜单
        QMenu*              ui_tool_menu;               ///<工具菜单
        QMenu*              ui_help_menu;               ///<帮助菜单
        QAction*            ui_open_file_action;        ///<打开文件操作
        QAction*            ui_save_file_action;        ///<报出文件对话框
        QAction*            ui_save_as_file_action;     ///<另存为
		QAction*			ui_refresh_action;			///<更新当前显示画面
		protected:
		float*				m_move_data;				///<数据
		int					m_time_length;				///<时间长度
		int					m_move_height;				///<电影的画面的高度
		int					m_move_width;				///<电影换面的宽度
		int                 m_frame_ration;				///<帧率
        QCPColorMap*        m_colormap;                 ///<颜色
        QCPColorGradient    m_colormap_gradient;        ///<色标
        QCPColorScale*      m_colorscale;               ///<色标显示值
        QString             m_xlabel;               
        QString             m_ylabel;
        QString             m_colormap_label;
		protected:
		void initShowFrame();                           ///<初始化绘制系统 
        protected slots:
        //void onTimerAction();///<响应定时器操作
        void onOpenFileAction();						///<打开文件
		void onSaveFileAction();						///<保存文件
        void onShowFrame(const int frame);              ///<响应某一帧画面
		void onRefresh();							    ///<更新当前画面
		void onColormap();								///<响应色标更新
    };
}