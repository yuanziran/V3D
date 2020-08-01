#include"qximage_axis.hpp"
#include<QFormLayout>
#include<QHBoxLayout>
using namespace V3D;

QXimageAxisDialog::QXimageAxisDialog(QString xLabel,QString yLabel,QString colormapLabel,QWidget* parent):QDialog(parent)
{
	setupUI();
	ui_xlabel_edit->setText(xLabel);
	ui_ylabel_edit->setText(yLabel);
	ui_colormap_label_edit->setText(colormapLabel);
}


void QXimageAxisDialog::setupUI()
{
	ui_xlabel=new QLabel(tr("xlabel"));
	ui_xlabel_edit=new QLineEdit();
	ui_ylabel=new QLabel(tr("ylabel"));
	ui_ylabel_edit=new QLineEdit();
	ui_colormap_label=new QLabel("colormap label");
	ui_colormap_label_edit=new QLineEdit();
	ui_ok=new QPushButton(tr("ok"));
	ui_cancel=new QPushButton(tr("cancel"));
	QFormLayout* form_layout=new QFormLayout();
	form_layout->addRow(ui_xlabel,ui_xlabel_edit);
	form_layout->addRow(ui_ylabel,ui_ylabel_edit);
	form_layout->addRow(ui_colormap_label,ui_colormap_label_edit);
	QHBoxLayout* sub_layout=new QHBoxLayout;
    sub_layout->addWidget(ui_cancel);
	sub_layout->addWidget(ui_ok);
    form_layout->addRow(sub_layout);
	setLayout(form_layout);
	connect(ui_cancel,SIGNAL(clicked()),this,SLOT(reject()));
	connect(ui_ok,SIGNAL(clicked()),this,SLOT(accept()));
}

QString QXimageAxisDialog::getXLabel()const
{
	return ui_xlabel_edit->text();
}

QString QXimageAxisDialog:: getYLabel()const
{
	return ui_ylabel_edit->text();
}

QString QXimageAxisDialog:: getColormapLabel()const
{
	return ui_colormap_label_edit->text();
}