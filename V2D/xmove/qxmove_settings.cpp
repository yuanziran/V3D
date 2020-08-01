#include"qxmove_settings.hpp"
#include<QHBoxLayout>
#include<QIcon>
#include<QPixmap>
#include<QPainter>
#include<QString>
#include<QVariant>
using namespace V3D;

QXMoveSettings::QXMoveSettings(int timeLength,int height,int width,int frameRation,QWidget* parent):QDialog(parent)
{
    initUI();
    setTimeLength(timeLength);
    setMoveHeight(height);
    setMoveWidth(width);
    setMoveFrameRation(frameRation);
}

int QXMoveSettings::timeLength()const
{
    return ui_time_length_data->text().toInt();
}

void QXMoveSettings::setTimeLength(const int length)
{
    if(length<=0) return;
    ui_time_length_data->setText(QString::asprintf("%d",length));
}

int  QXMoveSettings::moveHeight()const
{
    return ui_move_height_data->text().toInt();
}

void QXMoveSettings::setMoveHeight(const int height)
{
    if(height<=0) return;
    ui_move_height_data->setText(QString::asprintf("%d",height));
}

int QXMoveSettings::moveWidth()const
{
    return ui_move_width_data->text().toInt();
}

void QXMoveSettings:: setMoveWidth(const int width)
{
    if(width<=0) return;
    ui_move_width_data->setText(QString::asprintf("%d",width));
}

int  QXMoveSettings::moveFrameRation()const
{
    return ui_move_frame_ration_data->text().toInt();
}

void QXMoveSettings::setMoveFrameRation(const int frameRation)
{
    if(frameRation<=0) return;
    ui_move_frame_ration_data->setText(QString::asprintf("%d",frameRation));
}

QCPColorGradient QXMoveSettings::colorgradient()const
{
    return ui_combobox_data->currentData().value<QCPColorGradient>();
	
}


void QXMoveSettings::initUI()
{
    ui_time_length_label=new QLabel(tr("TimeLength"));
    ui_time_length_data=new QLineEdit;
    ui_move_height_label=new QLabel(tr("MoveHeight"));
    ui_move_height_data=new QLineEdit;
    ui_move_width_label=new QLabel(tr("MoveWidth"));
    ui_move_width_data=new QLineEdit;
    ui_move_frame_ration_label=new QLabel(tr("FrameRation"));
    ui_move_frame_ration_data=new QLineEdit;
    ui_color_label=new QLabel(tr("Colormap"));
    ui_combobox_data=new QComboBox();
    ui_reject_button=new QPushButton(tr("Cancel"));
    ui_accept_button=new QPushButton(tr("Ok"));
    ui_main_layout=new QFormLayout;
    ui_main_layout->addRow(ui_time_length_label,ui_time_length_data);
    ui_main_layout->addRow(ui_move_height_label,ui_move_height_data);
    ui_main_layout->addRow(ui_move_width_label,ui_move_width_data);
    ui_main_layout->addRow(ui_move_frame_ration_label,ui_move_frame_ration_data);
    ui_main_layout->addRow(ui_color_label,ui_combobox_data);
    QHBoxLayout *sub_layout=new QHBoxLayout;
    sub_layout->addWidget(ui_reject_button);
    sub_layout->addWidget(ui_accept_button);
    ui_main_layout->addRow(sub_layout);
    setLayout(ui_main_layout);
    //处理组合框
    auto init_combobox_fun=[](const QCPColorGradient &color,const QString& name,QComboBox* combobox)
    {
        QPixmap pm(400,400);
        QPainter pmp(&pm);
        QLinearGradient gradient(0,0,400,0);
        QMap<double,QColor> color_data=color.colorStops();
        foreach(const double pos,color_data.keys())
        {
            gradient.setColorAt(pos,color_data[pos]);
        }
        pmp.setBrush(QBrush(gradient));
        pmp.drawRect(0,0,400,400);
        QIcon icon(pm);
		QVariant t;
		t.setValue<QCPColorGradient>(color);
        combobox->addItem(icon,name,t);
    };
    init_combobox_fun(QCPColorGradient::gpGrayscale,tr("gray"),ui_combobox_data);
    init_combobox_fun(QCPColorGradient::gpHot,tr("hot"),ui_combobox_data);
    init_combobox_fun(QCPColorGradient::gpIon,tr("ion"),ui_combobox_data);
    init_combobox_fun(QCPColorGradient::gpCold,tr("cold"),ui_combobox_data);
    init_combobox_fun(QCPColorGradient::gpCandy,tr("candy"),ui_combobox_data);
    init_combobox_fun(QCPColorGradient::gpNight,tr("night"),ui_combobox_data);
    init_combobox_fun(QCPColorGradient::gpSpectrum,tr("spectrum"),ui_combobox_data);
    init_combobox_fun(QCPColorGradient::gpThermal,tr("thermal"),ui_combobox_data);
    init_combobox_fun(QCPColorGradient::gpJet,tr("jet"),ui_combobox_data);
    init_combobox_fun(QCPColorGradient::gpGeography,tr("geography"),ui_combobox_data);
    init_combobox_fun(QCPColorGradient::gpPolar,tr("polar"),ui_combobox_data);
    init_combobox_fun(QCPColorGradient::gpHues,tr("hues"),ui_combobox_data);
    connect(ui_reject_button,SIGNAL(clicked()),this,SLOT(reject()));
    connect(ui_accept_button,SIGNAL(clicked()),this,SLOT(accept()));
}