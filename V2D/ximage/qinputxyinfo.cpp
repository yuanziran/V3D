#include"qinputxyinfo.hpp"
#include<QFormLayout>
#include<QHBoxLayout>
using namespace V3D;

QInputXYInfoDialog::QInputXYInfoDialog(int xDim,int yDim,QWidget* parent):QDialog(parent)
{
	setupUI();
	ui_xdim_edit->setText(QString::asprintf("%d", xDim));
	ui_ydim_edit->setText(QString::asprintf("%d", yDim));
}

int QInputXYInfoDialog::getXDim()
{
	return ui_xdim_edit->text().toInt();
}

int QInputXYInfoDialog::getYDim()
{
	return ui_ydim_edit->text().toInt();
}


void QInputXYInfoDialog::setupUI()
{
	ui_xdim_label=new QLabel(tr("xdim"));
	ui_xdim_edit=new QLineEdit;
	ui_ydim_label=new QLabel(tr("ydim"));
	ui_ydim_edit=new QLineEdit;
	ui_ok=new QPushButton(tr("ok"));
	ui_cancel=new QPushButton(tr("cancel"));
	QFormLayout* form_layout=new QFormLayout();
	form_layout->addRow(ui_xdim_label,ui_xdim_edit);
	form_layout->addRow(ui_ydim_label,ui_ydim_edit);
    QHBoxLayout* sub_layout=new QHBoxLayout;
    sub_layout->addWidget(ui_cancel);
    sub_layout->addWidget(ui_ok);
	form_layout->addRow(sub_layout);
	setLayout(form_layout);
	connect(ui_cancel,SIGNAL(clicked()),this,SLOT(reject()));
	connect(ui_ok,SIGNAL(clicked()),this,SLOT(accept()));
}
