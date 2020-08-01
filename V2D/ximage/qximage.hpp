#pragma once
#include"../qcustomplot.h"
#include<QString>
#include<QLabel>
#include<QLineEdit>
#include<QAction>
#include<QMainWindow>
#include<QDialog>
#include<QLineEdit>
#include<QPushButton>
namespace V3D
{

    class QXimage : public QMainWindow
    {
        Q_OBJECT
        public:
        QXimage(QWidget* parent=nullptr);
		void setData(const float* data, const int xdim, const int ydim);
		float* getData()const { return m_data; }
		int    getXDim()const { return m_xdim; }
		int	   getYDim()const { return m_ydim; }
        ~QXimage();
        protected:
        void initUI();
        protected:
        QCustomPlot*        ui_handle;
		QCPItemTracer*      ui_tracer;
		QCPItemStraightLine*ui_v_line;
		QCPItemStraightLine*ui_h_line;
		QLabel*		        ui_label;
		QCPItemLine*		ui_arrow;
        QAction*            ui_open_file_action;
        QAction*            ui_save_file_action;
        QAction*            ui_gray_colormap_action;
		QAction*			ui_hot_colormap_action;
		QAction*			ui_cold_colormap_action;
		QAction*			ui_candy_colormap_action;
		QAction*			ui_ion_colormap_action;
		QAction*			ui_night_colormap_action;
		QAction*			ui_spectrum_colormap_action;
		QAction*			ui_thermal_colormap_action;
		QAction*			ui_jet_colormap_action;
		QAction*			ui_geography_colormap_action;
		QAction*			ui_polar_colormap_action;
		QAction*			ui_hues_colormap_action;
        QAction*            ui_draw_action;///<重绘命令
		QAction*			ui_transpose_action;///<转置
		QAction*			ui_settings_action;

        protected:
        QString             m_xlabel;
        QString             m_ylabel;
        int                 m_xdim;
        int                 m_ydim;
        float*              m_data;
		QCPColorScale*      m_colorscale;
        QCPColorGradient    m_colormap;
        QString             m_colormap_label;
		bool                m_is_transpose;
        private slots:
        void onOpenFileAction();
        void onSaveFileAction();
		void onGrayAction();
		void onHotAction();
		void onColdAction();
		void onCandyAction();
		void onIonAction();
		void onNightAction();
		void onSpectrumAction();
		void onThermalAction();
		void onJetAction();
		void onGeographyAction();
		void onPolarAction();
		void onHuesAction();
		void onClear();
		void onMousePress(QMouseEvent* event);
		void onMouseRelease(QMouseEvent* event);
        void onDrawAction();
		void onTransposeAction();
		void onSettingsAction();
    };
}