#pragma once
#include<QDialog>
#include<QString>
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>
namespace V3D
{
    class QXimageAxisDialog : public QDialog 
	{
		Q_OBJECT
		public:
		QXimageAxisDialog(QString xLabel,QString yLabel,QString colormapLabel,QWidget* parent=0);
		QString getXLabel()const;
		QString getYLabel()const;
		QString getColormapLabel()const;
		protected:
		void setupUI();
		QLabel*			ui_xlabel;
		QLineEdit*		ui_xlabel_edit;
		QLabel*			ui_ylabel;
		QLineEdit*		ui_ylabel_edit;
		QLabel*			ui_colormap_label;
		QLineEdit*		ui_colormap_label_edit;
		QPushButton*	ui_ok;
		QPushButton*	ui_cancel;
	};
}