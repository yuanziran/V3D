#include"qxmove.hpp"
#include<QMenuBar>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QFileDialog>
#include<QFileInfo>
#include<QDir>
#include<QString>
#include<QFile>
#include<QDataStream>
#include<QIODevice>
#include"qxmove_settings.hpp"
using namespace V3D;


 QXMove::QXMove(QWidget* parent):QMainWindow(parent)
 {
	 m_move_data = nullptr;
	 m_frame_ration=0;
	 m_move_height=0;
	 m_move_width=0;
     initUI();
 }


void QXMove:: initUI()
{
    QMenuBar* menu_bar=menuBar();
    ui_file_menu=menu_bar->addMenu(tr("File"));
    ui_edit_menu=menu_bar->addMenu(tr("Edit"));
    ui_tool_menu=menu_bar->addMenu(tr("Tool"));
    ui_help_menu=menu_bar->addMenu(tr("Help"));
    //文件菜单栏处理
    ui_open_file_action=ui_file_menu->addAction(tr("Open"));
    ui_save_file_action=ui_file_menu->addAction(tr("Save"));
    ui_save_as_file_action=ui_file_menu->addAction(tr("Save as"));
    //编辑栏菜单处理
	ui_refresh_action = ui_edit_menu->addAction(tr("Refresh"));
	//工具栏菜单处理
	auto init_menu_fun = [this](const QCPColorGradient &color, const QString& name, QMenu* menu)
	{
		QPixmap pm(400, 400);
		QPainter pmp(&pm);
		QLinearGradient gradient(0, 0, 400, 0);
		QMap<double, QColor> color_data = color.colorStops();
		foreach(const double pos, color_data.keys())
		{
			gradient.setColorAt(pos, color_data[pos]);
		}
		pmp.setBrush(QBrush(gradient));
		pmp.drawRect(0, 0, 400, 400);
		QIcon icon(pm);
		QVariant t;
		t.setValue<QCPColorGradient>(color);
		QAction* ui_action=menu->addAction(icon,name);
		connect(ui_action,SIGNAL(triggered()),this,SLOT(onColormap()));
	};
	init_menu_fun(QCPColorGradient::gpGrayscale, tr("gray"), ui_tool_menu);
	init_menu_fun(QCPColorGradient::gpHot, tr("hot"), ui_tool_menu);
	init_menu_fun(QCPColorGradient::gpIon, tr("ion"), ui_tool_menu);
	init_menu_fun(QCPColorGradient::gpCold, tr("cold"), ui_tool_menu);
	init_menu_fun(QCPColorGradient::gpCandy, tr("candy"), ui_tool_menu);
	init_menu_fun(QCPColorGradient::gpNight, tr("night"), ui_tool_menu);
	init_menu_fun(QCPColorGradient::gpSpectrum, tr("spectrum"), ui_tool_menu);
	init_menu_fun(QCPColorGradient::gpThermal, tr("thermal"), ui_tool_menu);
	init_menu_fun(QCPColorGradient::gpJet, tr("jet"), ui_tool_menu);
	init_menu_fun(QCPColorGradient::gpGeography, tr("geography"), ui_tool_menu);
	init_menu_fun(QCPColorGradient::gpPolar, tr("polar"), ui_tool_menu);
	init_menu_fun(QCPColorGradient::gpHues, tr("hues"), ui_tool_menu);
    QVBoxLayout* main_layout=new QVBoxLayout();
    ui_handle=new QCustomPlot();
    QHBoxLayout* sub_layout=new QHBoxLayout();
    ui_frame_num=new QLabel;
    ui_time_slider=new QSlider;
	ui_time_slider->setOrientation(Qt::Horizontal);
    ui_timepoint_edit=new QLineEdit;
    sub_layout->addWidget(ui_frame_num);
    sub_layout->addWidget(ui_time_slider);
    sub_layout->addWidget(ui_timepoint_edit);
    main_layout->addWidget(ui_handle);
    main_layout->addLayout(sub_layout);
	QWidget* temp = new QWidget();
	temp->setLayout(main_layout);
	setCentralWidget(temp);
	connect(ui_open_file_action,SIGNAL(triggered()),this,SLOT(onOpenFileAction()));
	connect(ui_save_file_action,SIGNAL(triggered()),this,SLOT(onSaveFileAction()));
	connect(ui_refresh_action,SIGNAL(triggered()),this,SLOT(onRefresh()));
    connect(ui_time_slider,SIGNAL(valueChanged(int)),this,SLOT(onShowFrame(int)));
}

void QXMove::onOpenFileAction()
{
	QString file_name = QFileDialog::getOpenFileName(this,tr("Open File"),QDir::currentPath(),"mat3f(*.mat3f);;bin(*.bin)");
	if (file_name.isEmpty()) return;
	QFileInfo info(file_name);
	QString suffix_name = info.suffix();
	if (suffix_name == "bin")
	{
		QXMoveSettings* dlg=new QXMoveSettings(m_time_length,m_move_height,m_move_width,m_frame_ration);
		if (dlg->exec() == QDialog::Accepted)
		{
			m_time_length = dlg->timeLength();
			m_move_height = dlg->moveHeight();
			m_move_width = dlg->moveWidth();
			m_colormap_gradient = dlg->colorgradient();
		}
		delete dlg;
        QFile bin_reader(file_name);
        if(!(bin_reader.open(QIODevice::ReadOnly))) return;
        QDataStream stream(&bin_reader);
        stream.setVersion(QDataStream::Qt_5_9);
        if(m_move_data) delete[] m_move_data;
		m_move_data=new float[m_time_length*m_move_height*m_move_width];
        stream.readRawData((char*)m_move_data,(m_time_length*m_move_height*m_move_width*sizeof(float)));
	}
	else if (suffix_name == "mat3f")
	{
		QFile mat3f_reader(file_name);
		if (!(mat3f_reader.open(QIODevice::ReadOnly))) return;
		QDataStream stream(&mat3f_reader);
		stream.setVersion(QDataStream::Qt_5_9);
		stream.readRawData((char*)&m_time_length,sizeof(int));
		stream.readRawData((char*)&m_move_height,sizeof(int));
		stream.readRawData((char*)&m_move_width,sizeof(int));
		if (m_move_data) delete[] m_move_data;
		m_move_data = new float[m_time_length*m_move_height*m_move_width];
		stream.readRawData((char*)m_move_data, (m_time_length*m_move_height*m_move_width * sizeof(float)));
	}
	else
	{
		return;
	}
	ui_time_slider->setMinimum(0);
	ui_time_slider->setMaximum(m_time_length);
	ui_time_slider->setSingleStep(1);
	ui_frame_num->setText(QString::asprintf("%d",m_time_length));
	//if (ui_handle) delete ui_handle;
	initShowFrame();
}

void QXMove::onSaveFileAction()
{
	QString file_name = QFileDialog::getSaveFileName(this,tr("Save File"),QDir::currentPath(),"bmp(*.bmp);;jpg(8.jpg);;png(*.png)");
	if (file_name.isEmpty()) return;
	QFileInfo info(file_name);
	QString suffix = info.suffix();
	if (suffix == "bmp")
	{
		ui_handle->saveBmp(file_name);
	}
	else if (suffix == "jpg")
	{
		ui_handle->saveJpg(file_name);
	}
	else if (suffix == "pdf")
	{
		ui_handle->savePdf(file_name);
	}
	else if (suffix == "png")
	{
		ui_handle->savePng(file_name);
	}
	else
	{
		return;
	}
}

void QXMove::initShowFrame()
{
    if(m_move_data==nullptr) return;
    ui_handle->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectPlottables);
    ui_handle->axisRect()->setupFullAxesBox(true);
    ui_handle->xAxis->setLabel(m_xlabel);
    ui_handle->yAxis->setLabel(m_ylabel);
    ui_handle->yAxis->setRangeReversed(true);
	ui_handle->xAxis->orientation(QCPAxis::AxisType::atTop);
    m_colormap=new QCPColorMap(ui_handle->xAxis,ui_handle->yAxis);
    m_colormap->data()->setSize(m_move_height,m_move_width);
    m_colormap->data()->setRange(QCPRange(0,m_move_height),QCPRange(0,m_move_width));
    for(int i=0;i<m_move_height;++i)
    for(int j=0;j<m_move_width;++j)
    {
        m_colormap->data()->setCell(i,j,m_move_data[(i*m_move_width+j)]);
    }
	if (ui_handle->plotLayout()->hasElement(0, 1))
	{
		ui_handle->plotLayout()->remove(m_colorscale);
	}
    m_colorscale=new QCPColorScale(ui_handle);
    ui_handle->plotLayout()->addElement(0,1,m_colorscale);
    m_colorscale->setType(QCPAxis::atRight);
    m_colormap->setColorScale(m_colorscale);
    m_colormap->setGradient(m_colormap_gradient);
    m_colormap->rescaleDataRange();
    QCPMarginGroup* margin=new QCPMarginGroup(ui_handle);
    ui_handle->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop,margin);
    m_colorscale->setMarginGroup(QCP::msBottom|QCP::msTop,margin);
    ui_handle->rescaleAxes();
	ui_handle->replot();

}

void QXMove::onShowFrame(const int frame)
{
    
    if(m_move_data==nullptr || frame<0 || frame>=m_time_length) return;
    for(int i=0;i<m_move_height;++i)
    for(int j=0;j<m_move_width;++j)
    {
        m_colormap->data()->setCell(i,j,m_move_data[(frame*m_move_height+i)*m_move_width+j]);
    }
    ui_handle->replot();   
    ui_timepoint_edit->setText(QString::asprintf("%d",frame));
}

void QXMove::onRefresh()
{
	if (ui_handle) ui_handle->replot();
}

void QXMove::onColormap()
{
	QAction* ui_action = (QAction*)sender();
	QString name = ui_action->text();
	if (name == tr("gray"))
	{
	  m_colormap_gradient= QCPColorGradient::gpGrayscale;
	}
	else if (name == tr("hot"))
	{
		m_colormap_gradient = QCPColorGradient::gpHot;
	}
	else if(name==tr("ion"))
	{
		m_colormap_gradient = QCPColorGradient::gpIon;
	}
	else if (name == tr("cold"))
	{
		m_colormap_gradient = QCPColorGradient::gpCold;
	}
	else if (name == tr("candy"))
	{
		m_colormap_gradient = QCPColorGradient::gpCandy;
	}
	else if (name == tr("night"))
	{
		m_colormap_gradient = QCPColorGradient::gpNight;
	}
	else if (name == tr("spectrum"))
	{
		m_colormap_gradient = QCPColorGradient::gpSpectrum;
	}
	else if (name == tr("jet"))
	{
		m_colormap_gradient = QCPColorGradient::gpJet;
	}
	else if (name == tr("polar"))
	{
		m_colormap_gradient = QCPColorGradient::gpPolar;
	}
	else if (name == tr("geography"))
	{
		m_colormap_gradient = QCPColorGradient::gpGeography;
	}
	else if (name == tr("hues"))
	{
		m_colormap_gradient = QCPColorGradient::gpHues;
	}
	else if (name == tr("thermal"))
	{
		m_colormap_gradient = QCPColorGradient::gpThermal;
	}
	else
	{
		return;
	}
	if (ui_handle)
	{
		m_colormap->setGradient(m_colormap_gradient);
		ui_handle->replot();
	}
}