#pragma once
#include<QDialog>
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>
namespace V3D
{
    class QInputXYInfoDialog : public QDialog 
	{
		Q_OBJECT
		public:
		QInputXYInfoDialog(int xDim=0,int yDim=0,QWidget* parent=0);
		int getXDim();
		int getYDim();
		protected:
		void setupUI();
		QLabel*			ui_xdim_label;
		QLineEdit*		ui_xdim_edit;
		QLabel*			ui_ydim_label;
		QLineEdit*		ui_ydim_edit;
		QPushButton*	ui_ok;
		QPushButton*	ui_cancel;
	};
}