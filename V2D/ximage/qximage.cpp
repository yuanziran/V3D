#include"qximage.hpp"
#include<limits.h>
#include<QtGlobal>
#include<QFileDialog>
#include<QFile>
#include<QMap>
#include<QIcon>
#include<QPen>
#include<QPixmap>
#include<QFormLayout>
#include<QPainter>
#include<QBrush>
#include<QStatusBar>
#include<QLinearGradient>
#include<QDataStream>
#include<QMessageBox>
#include"qximage_axis.hpp"
#include"qinputxyinfo.hpp"
using namespace V3D;


QIcon QCPColorGradientToQIcon(const QCPColorGradient &color,const int width,const int height)
{
    QPixmap pm(width,height);
	QPainter pmp(&pm);
    QLinearGradient gradient(0,0,width,0);
    QMap<double,QColor> color_data=color.colorStops();
    foreach(const double pos,color_data.keys())
    {
        gradient.setColorAt(pos,color_data[pos]);
    }
    pmp.setBrush(QBrush(gradient));
    pmp.drawRect(0,0,width,height);
    return QIcon(pm);
}

QXimage::QXimage(QWidget* parent):QMainWindow(parent)
{
    initUI();
    m_colormap.loadPreset(QCPColorGradient::gpIon);
	m_is_transpose = false;
    m_xlabel=tr("x");
    m_ylabel=tr("y");
    m_xdim=1000;
    m_ydim=1000;
    m_data=new float[m_xdim*m_ydim];
    for(int i=0;i<m_xdim;++i)
    for(int j=0;j<m_ydim;++j)
    {
        m_data[i*m_ydim+j]=sinf(0.001*(i+j));
    }
    m_colormap_label=tr("Ion colormap");
    onDrawAction();
}


void QXimage:: setData(const float* data, const int xdim, const int ydim)
{
	const unsigned int len = xdim * ydim;
	if (len != (m_xdim*m_ydim))
	{
		if (m_data) delete[] m_data;
		m_data = new float[len];
	}
	m_xdim = xdim;
	m_ydim = ydim;
	for(int i=0;i<m_xdim;++i)
	for (int j = 0; j < m_ydim; ++j)
	{
		m_data[i*m_ydim + j] = data[i*m_ydim+j];
	}
	onDrawAction();
}

QXimage::~QXimage()
{
    if(m_data) delete[] m_data;
}

void QXimage::initUI()
{
    ui_handle=new QCustomPlot(this);
	ui_tracer = new QCPItemTracer(ui_handle);
	ui_tracer->setStyle(QCPItemTracer::tsCrosshair);
	ui_tracer->setPen(QPen(Qt::black));
	ui_tracer->setSize(10);
	QPen lines_pen(Qt::black, 1);
	//��ֱ��
	ui_v_line = new QCPItemStraightLine(ui_handle);
	ui_v_line->setLayer("overlay");
	ui_v_line->setPen(lines_pen);
	ui_v_line->setClipToAxisRect(true);
	ui_v_line->point1->setCoords(0,0);
	ui_v_line->point2->setCoords(0,0);
	//ˮƽ��
	ui_h_line = new QCPItemStraightLine(ui_handle);
	ui_h_line->setLayer("overlay");
	ui_h_line->setClipToAxisRect(true);
	ui_h_line->point1->setCoords(0,0);
	ui_h_line->point2->setCoords(0,0);
	ui_label = new QLabel;
	QStatusBar* status_bar = statusBar();
	status_bar->addWidget(ui_label);
    QMenuBar* menu_bar=menuBar();
    QMenu*  file_menu=menu_bar->addMenu(tr("File"));
    ui_open_file_action=file_menu->addAction(tr("Open"));
    ui_save_file_action=file_menu->addAction(tr("Save"));
    QMenu* edit_menu=menu_bar->addMenu(tr("Edit"));
    ui_draw_action=edit_menu->addAction(tr("Draw"));
	ui_transpose_action=edit_menu->addAction(tr("Transpose"));
    QMenu* colormap_menu=menu_bar->addMenu(tr("Colormap"));
    m_colormap.loadPreset(QCPColorGradient::gpGrayscale);
    QIcon grayscale_icon=QCPColorGradientToQIcon(m_colormap,800,600);
    ui_gray_colormap_action=colormap_menu->addAction(grayscale_icon,tr("gray"));
	m_colormap.loadPreset(QCPColorGradient::gpHot);
	QIcon hot_icon = QCPColorGradientToQIcon(m_colormap,800,600);
	ui_hot_colormap_action = colormap_menu->addAction(hot_icon,tr("hot"));
	m_colormap.loadPreset(QCPColorGradient::gpCold);
	QIcon cold_icon = QCPColorGradientToQIcon(m_colormap,1000,800);
	ui_cold_colormap_action = colormap_menu->addAction(cold_icon, tr("cold"));
	m_colormap.loadPreset(QCPColorGradient::gpCandy);
	QIcon candy_icon = QCPColorGradientToQIcon(m_colormap,800,600);
	ui_candy_colormap_action = colormap_menu->addAction(candy_icon,tr("candy"));
	m_colormap.loadPreset(QCPColorGradient::gpIon);
	QIcon ion_icon = QCPColorGradientToQIcon(m_colormap,800,600);
	ui_ion_colormap_action = colormap_menu->addAction(ion_icon,tr("ion"));
#define abc(x,y,z)		\
{\
	m_colormap.loadPreset(QCPColorGradient::x);\
	QIcon icon=QCPColorGradientToQIcon(m_colormap,800,600);\
	y=colormap_menu->addAction(icon,z);\
}
	abc(gpNight, ui_night_colormap_action, tr("night"))
	abc(gpSpectrum, ui_spectrum_colormap_action, tr("spectrum"))
	abc(gpThermal,ui_thermal_colormap_action,tr("thermal"))
	abc(gpJet,ui_jet_colormap_action,tr("jet"))
	abc(gpGeography,ui_geography_colormap_action,tr("geography"))
	abc(gpPolar,ui_polar_colormap_action,tr("polar"))
	abc(gpHues,ui_hues_colormap_action,tr("hues"))
#undef abc
	QMenu* tool_menu=menu_bar->addMenu(tr("Tool"));
	ui_settings_action=tool_menu->addAction(tr("settings"));
    connect(ui_open_file_action,SIGNAL(triggered()),this,SLOT(onOpenFileAction()));
    connect(ui_save_file_action,SIGNAL(triggered()),this,SLOT(onSaveFileAction()));
    connect(ui_draw_action,SIGNAL(triggered()),this,SLOT(onDrawAction()));
	connect(ui_gray_colormap_action,SIGNAL(triggered()),this,SLOT(onGrayAction()));
	connect(ui_hot_colormap_action,SIGNAL(triggered()),this,SLOT(onHotAction()));
	connect(ui_cold_colormap_action,SIGNAL(triggered()),this,SLOT(onColdAction()));
	connect(ui_candy_colormap_action,SIGNAL(triggered()),this,SLOT(onCandyAction()));
	connect(ui_ion_colormap_action,SIGNAL(triggered()),this,SLOT(onIonAction()));
	connect(ui_handle,SIGNAL(mousePress(QMouseEvent*)),this,SLOT(onMousePress(QMouseEvent*)));
	connect(ui_handle,SIGNAL(mouseRelease(QMouseEvent*)),this,SLOT(onMouseRelease(QMouseEvent*)));
#define xy(a,b) connect(a,SIGNAL(triggered()),this,SLOT(b))
	xy(ui_night_colormap_action,onNightAction());
	xy(ui_spectrum_colormap_action,onSpectrumAction());
	xy(ui_thermal_colormap_action, onSpectrumAction());
	xy(ui_jet_colormap_action, onJetAction());
	xy(ui_geography_colormap_action, onGeographyAction());
	xy(ui_polar_colormap_action, onPolarAction());
	xy(ui_hues_colormap_action, onHuesAction());
	xy(ui_transpose_action,onTransposeAction());
	xy(ui_settings_action,onSettingsAction());
#undef xy
	m_colormap.loadPreset(QCPColorGradient::gpSpectrum);
	QIcon h_icon = QCPColorGradientToQIcon(m_colormap,800,800);
	setWindowIcon(h_icon);
    setCentralWidget(ui_handle);
}


void QXimage::onOpenFileAction()
{
    QString file_name=QFileDialog::getOpenFileName(this,
    tr("Open File"),QDir::currentPath(),"mat2f(*.mat2f);;bin(*.bin);;mdl(*.mdl)");
    if(file_name.isEmpty())
    {
        return;
    }
   QFileInfo info(file_name);
   QFile file(file_name);
   if(!(file.open(QIODevice::ReadOnly)))
   {
       return;
   }
   QDataStream stream(&file);
   stream.setVersion(QDataStream::Qt_5_9);
   if(info.suffix()=="bin")
   {
	   QInputXYInfoDialog* dlg=new QInputXYInfoDialog(m_xdim,m_ydim,this);
	   dlg->setWindowTitle(tr("bin file xy information"));
	   if(dlg->exec()==QDialog::Accepted)
	   {
		   m_xdim=dlg->getXDim();
		   m_ydim=dlg->getYDim();
		   delete dlg;
	   }
	   else
	   {
		   delete dlg;
		   return;
	   }
   }
   else if(info.suffix()=="mdl")
   {
	   size_t tmp_x=0;
	   size_t tmp_y=0;
	   stream.readRawData((char*)&tmp_x,sizeof(size_t));
	   stream.readRawData((char*)&tmp_y,sizeof(size_t));
	   if(tmp_x >INT_MAX || tmp_y>INT_MAX)
	   {
		   QMessageBox *dlg=new QMessageBox(this);
		   dlg->setText(QString::asprintf("xdim=%llu ydim=%llu outof INTMAX=%d ",tmp_x,tmp_y,INT_MAX));
		   dlg->exec();
		   delete dlg;
		   return ;
	   }
	   m_xdim=(int)tmp_x;
	   m_ydim=(int)tmp_y;
   }
   else //if (info.suffix() == "mat2f")
   {
	   stream.readRawData((char*)&m_xdim, sizeof(int));
	   stream.readRawData((char*)&m_ydim, sizeof(int));
   }
   if(m_data) delete[] m_data;
   m_data=new float[m_xdim*m_ydim];
   stream.readRawData((char*)m_data,m_xdim*m_ydim*sizeof(float));
   onClear();
   onDrawAction();
}

void QXimage::onSaveFileAction()
{
    QString file_name=QFileDialog::getSaveFileName(this,tr("Save File"),
    QDir::currentPath(),"bmp(*.bmp);;pdf(*.pdf);;png(*.png);;jpg(*.jpg)");
    if(file_name.isEmpty()) return;
    QFileInfo file_info(file_name);
    QString file_suffix=file_info.suffix();
    if(file_suffix=="bmp")
    {
        ui_handle->saveBmp(file_name);
    }
    else if(file_suffix=="png")
    {
        ui_handle->savePng(file_name);
    }
    else if(file_suffix=="jpg")
    {
        ui_handle->saveJpg(file_name);
    }
    else
    {
        ui_handle->savePdf(file_name);
    }
}

void QXimage:: onGrayAction()
{
	onClear();
	m_colormap.loadPreset(QCPColorGradient::gpGrayscale);
	m_colormap_label = "gray colormap";
	onDrawAction();
}

void QXimage::onHotAction()
{
	onClear();
	m_colormap.loadPreset(QCPColorGradient::gpHot);
	m_colormap_label = "hot colormap";
	onDrawAction();
}
void QXimage::onColdAction()
{
	onClear();
	m_colormap.loadPreset(QCPColorGradient::gpCold);
	m_colormap_label = "cold colormap";
	onDrawAction();
}

void QXimage::onCandyAction()
{
	onClear();
	m_colormap.loadPreset(QCPColorGradient::gpCandy);
	m_colormap_label = "candy colormap";
	onDrawAction();
}

void QXimage::onIonAction()
{
	onClear();
	m_colormap.loadPreset(QCPColorGradient::gpIon);
	m_colormap_label = "ion colormap";
	onDrawAction();
}


void QXimage::onNightAction()
{
	onClear();
	m_colormap.loadPreset(QCPColorGradient::gpNight);
	m_colormap_label = "night colormap";
	onDrawAction();
}

void QXimage::onSpectrumAction()
{
	onClear();
	m_colormap.loadPreset(QCPColorGradient::gpSpectrum);
	m_colormap_label = "spectrum colormap";
	onDrawAction();
}

void QXimage::onThermalAction()
{
	onClear();
	m_colormap.loadPreset(QCPColorGradient::gpThermal);
	m_colormap_label = "thermal colormap";
	onDrawAction();
}

void QXimage::onJetAction()
{
	onClear();
	m_colormap.loadPreset(QCPColorGradient::gpJet);
	m_colormap_label = "jet colormap";
	onDrawAction();
}

void QXimage::onGeographyAction()
{
	onClear();
	m_colormap.loadPreset(QCPColorGradient::gpGeography);
	m_colormap_label = "geography colormap";
	onDrawAction();
}

void QXimage::onPolarAction()
{
	onClear();
	m_colormap.loadPreset(QCPColorGradient::gpPolar);
	m_colormap_label = "polar colormap";
	onDrawAction();
}

void QXimage::onHuesAction()
{
	onClear();
	m_colormap.loadPreset(QCPColorGradient::gpHues);
	m_colormap_label = "hues colormap";
	onDrawAction();
}

void QXimage::onClear()
{
	ui_handle->clearGraphs();
	ui_handle->clearPlottables();
    //ui_handle->plotLayout()->clear();
}

void QXimage::onDrawAction()
{
    ui_handle->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectPlottables);
    ui_handle->axisRect()->setupFullAxesBox(true);
	QCPColorMap* colormap=new QCPColorMap(ui_handle->xAxis,ui_handle->yAxis);
	ui_handle->xAxis->setLabel(m_xlabel);
	//ui_handle->xAxis->set
    ui_handle->yAxis->setLabel(m_ylabel);
	ui_handle->yAxis->setRangeReversed(true);
	if(m_is_transpose)
	{
    	colormap->data()->setSize(m_ydim,m_xdim);
    	colormap->data()->setRange(QCPRange(0,m_ydim),QCPRange(0,m_xdim));
		for(int i=0;i<m_xdim;++i)
    	for(int j=0;j<m_ydim;++j)
    	{
        	colormap->data()->setCell(j,i,m_data[i*m_ydim+j]);
    	}
	}
	else
	{
    	colormap->data()->setSize(m_xdim,m_ydim);
    	colormap->data()->setRange(QCPRange(0,m_xdim),QCPRange(0,m_ydim));
    	for(int i=0;i<m_xdim;++i)
    	for(int j=0;j<m_ydim;++j)
    	{
        	colormap->data()->setCell(i,j,m_data[i*m_ydim+j]);
    	}
	}
	
	if (ui_handle->plotLayout()->hasElement(0, 1))
	{
		ui_handle->plotLayout()->remove(m_colorscale);
		//delete m_colorscale;
	}
	m_colorscale = new QCPColorScale(ui_handle);
    ui_handle->plotLayout()->addElement(0,1,m_colorscale);
    m_colorscale->setType(QCPAxis::atRight);
    colormap->setColorScale(m_colorscale);
    m_colorscale->axis()->setLabel(m_colormap_label);
    colormap->setGradient(m_colormap);
    colormap->rescaleDataRange();
    QCPMarginGroup* margin=new QCPMarginGroup(ui_handle);
    ui_handle->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop,margin);
    m_colorscale->setMarginGroup(QCP::msBottom|QCP::msTop,margin);
    ui_handle->rescaleAxes();
	ui_handle->replot();
}

void QXimage::onTransposeAction()
{
	m_is_transpose=!m_is_transpose;
	onDrawAction();
}


void QXimage::onMousePress(QMouseEvent* event)
{
	if (event->button() != Qt::LeftButton) return;
	QPoint point = event->pos();
	if (!ui_handle->viewport().contains(event->pos())) return;
	int i = ui_handle->xAxis->pixelToCoord(point.x());
	int j = ui_handle->yAxis->pixelToCoord(point.y());
	//updataPostition(point.x(),point.y());
	ui_v_line->point1->setCoords(i, ui_handle->yAxis->range().lower);
	ui_v_line->point2->setCoords(i, ui_handle->yAxis->range().upper);
	ui_h_line->point1->setCoords(ui_handle->xAxis->range().lower, j);
	ui_h_line->point2->setCoords(ui_handle->xAxis->range().upper, j);
	float data = NAN;
	if(m_is_transpose)
	{
		if(i<m_ydim && j< m_xdim && i >= 0 && j >= 0)
		{
			data=m_data[j*m_ydim+i];
		}
	}
	else
	{
		if (i < m_xdim && j < m_ydim && i >= 0 && j >= 0)
		{
			data = m_data[i*m_ydim+j];
		}
	}
	ui_label->setText(QString::asprintf("x:%d y:%d value :%f",i,j,data));
	ui_handle->replot();
}

void QXimage::onMouseRelease(QMouseEvent* event)
{
	if (event->button() != Qt::LeftButton) return;
	ui_h_line->point1->setCoords(0,0);
	ui_h_line->point2->setCoords(0,0);
	ui_v_line->point1->setCoords(0,0);
	ui_v_line->point2->setCoords(0,0);
	ui_handle->replot();
}


void QXimage :: onSettingsAction()
{
	QXimageAxisDialog *dlg=new QXimageAxisDialog(m_xlabel,m_ylabel,m_colormap_label,this);
	dlg->setWindowTitle(tr("axis settings"));
	if(dlg->exec()==QDialog::Accepted)
	{
		m_xlabel=dlg->getXLabel();
		m_ylabel=dlg->getYLabel();
		m_colormap_label=dlg->getColormapLabel();
		onClear();
		onDrawAction();
	}
	delete dlg;
}

