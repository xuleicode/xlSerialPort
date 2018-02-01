#include "qdrawwidget.h"
#include <QTime>
//#include "iconhelper.h"

#define TextColor QColor(128,128,128)
#define Plot_NoColor QColor(0,0,0,0)

#define Plot1_DotColor QColor(5,189,251)//
#define Plot1_LineColor QColor(41,138,220)//41,138,220)
#define Plot1_BGColor QColor(41,138,220,80)

#define Plot2_DotColor QColor(236,110,0)
#define Plot2_LineColor QColor(246,98,0)
#define Plot2_BGColor QColor(246,98,0,80)

#define Plot3_AlarmColor1 QColor(204,0,0)
#define Plot3_AlarmColor1X QColor(204,0,0,200)
#define Plot3_AlarmColor2 QColor(246,98,0)
#define Plot3_AlarmColor2X QColor(246,98,0,200)
#define Plot3_AlarmColor3 QColor(41,138,220)
#define Plot3_AlarmColor3X QColor(41,138,220,200)

#define Plot4_AlarmColor1 QColor(238,0,0)
#define Plot4_AlarmColor1X QColor(238,0,0,200)
#define Plot4_AlarmColor2 QColor(204,250,255)
#define Plot4_AlarmColor2X QColor(204,250,255,200)
#define Plot4_AlarmColor3 QColor(5,189,251)
#define Plot4_AlarmColor3X QColor(5,189,251,200)
#define Plot4_AlarmColor4 QColor(236,110,0)
#define Plot4_AlarmColor4X QColor(236,110,0,200)
#define Plot4_AlarmColor5 QColor(106,139,34)
#define Plot4_AlarmColor5X QColor(106,139,34,200)
#define Plot4_AlarmColor6 QColor(99,149,236)
#define Plot4_AlarmColor6X QColor(99,149,236,200)
#define Plot4_AlarmColor7 QColor(130,111,255)
#define Plot4_AlarmColor7X QColor(130,111,255,200)
#define Plot4_AlarmColor8 QColor(205,173,0)
#define Plot4_AlarmColor8X QColor(205,173,0,200)
#define Plot4_AlarmColor9 QColor(1,139,0)
#define Plot4_AlarmColor9X QColor(1,139,0,200)

#define TextWidth 1
#define LineWidth 2
#define DotWidth 10

#define Plot1_Count 20
#define Plot2_Count 5
#define Plot3_Count 7
#define Plot4_Count 9
#define Plot5_Count 9

#define Plot1_MaxY 50
#define Plot2_MaxY 100
#define Plot3_MaxY 500
#define Plot4_MaxY 100

#ifdef __arm__
#define Plot5_MaxY 20
#else
#define Plot5_MaxY 50
#endif

const int cnCount=50;//画图的点数
const double PI = 3.14159265358;//pi
QDrawWidget::QDrawWidget(QWidget *parent) :
QWidget(parent),m_Index(0)
{
	
	this->InitStyle();
	this->InitForm();
	this->initUI();
	this->InitPlot();
	this->InitPlot1();

	//this->LoadPlot1();
}
QDrawWidget::~QDrawWidget()
{
	//delete ui;
}
void QDrawWidget::initUI()
{
	btnAdd = new QPushButton(this);
	btnAdd->setText(tr("AddPoint"));
	connect(btnAdd,SIGNAL(clicked()),this,SLOT(on_btnAdd_clicked()));


	btnMove = new QCheckBox(this);
	btnMove->setText(tr("MoveAble"));
	connect(btnMove,SIGNAL(clicked()),this,SLOT(on_ckMove_stateChanged()));
	btnBackg = new QCheckBox(this);
	btnBackg->setText(tr("Background"));
	connect(btnBackg,SIGNAL(clicked()),this,SLOT(on_ckBackground_stateChanged()));
	btnLegend= new QCheckBox(this);
	btnLegend->setText(tr("Legend"));
	connect(btnLegend,SIGNAL(clicked()),this,SLOT(on_ckText_stateChanged()));
	btnReset = new QPushButton(this);
	btnReset->setText(tr("Reset"));
	connect(btnReset,SIGNAL(clicked()),this,SLOT(on_btnReset_clicked()));
	btnResetAxes = new QPushButton(this);
	btnResetAxes->setText(tr("ResetAxes"));
	connect(btnResetAxes,SIGNAL(clicked()),this,SLOT(on_btnRestAxes_clicked()));
	QComboBox *cmbBox = new QComboBox(this);
	QStringList strlist;
	//strlist<<QString::fromLocal8Bit("水平垂直")<<QString::fromLocal8Bit("水平")<<QString::fromLocal8Bit("垂直");
	strlist<<"Default"<<"Horizontal"<<"Vertical";
	cmbBox->addItems(strlist);
	cmbBox->setCurrentIndex(0);
	connect(cmbBox,SIGNAL(currentIndexChanged(int)),this,SLOT(on_tabWidget_currentChanged(int)));
	QLabel *lableRangeZoom = new QLabel(this);
	lableRangeZoom->setText("RangeZoom:");
////===============================
//	QHBoxLayout *plotLayout = new QHBoxLayout();
//	plotLayout->setMargin(0);
//	plotLayout->addStretch();
//	plotLayout->addWidget(plot1);
//	QScrollArea *pArea= new QScrollArea(this);
//	QWidget * qw = new QWidget(pArea);//需要滚动的是一个Qwidget，而如果是在设计器里面拖入控件，会自动添加一个
//	qw->setLayout(plotLayout);
//
//	pArea->setWidget(qw);//这里设置滚动窗口qw，
//	pArea->setGeometry(0,0,200,200);//要显示的区域大小
//
//	//===============================

	QHBoxLayout *btnLayout = new QHBoxLayout();
	btnLayout->setMargin(0);
	btnLayout->addStretch();
	
	btnLayout->addWidget(btnAdd);
	btnAdd->hide();
	btnLayout->addWidget(btnReset);
	btnReset->hide();
	btnLayout->addWidget(btnResetAxes);
	btnLayout->addWidget(btnBackg);
	btnLayout->addWidget(btnLegend);
	btnLayout->addWidget(btnMove);
	btnAdd->hide();
	btnLayout->addWidget(lableRangeZoom);
	btnLayout->addWidget(cmbBox);
	QVBoxLayout *centerLayout = new QVBoxLayout();
	centerLayout->setMargin(0);
	centerLayout->setContentsMargins(5,0,5,5);
	centerLayout->addWidget(plot1);
	centerLayout->addLayout(btnLayout);
	this->setLayout(centerLayout);

	btnBackg->setChecked(true);
	btnMove->setChecked(true);
	btnLegend->setChecked(true);
}
void QDrawWidget::InitStyle()
{
	//this->setProperty("Form", true);
	//this->setWindowTitle(ui->lab_Title->text());
	//this->setGeometry(qApp->desktop()->availableGeometry());
	//this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
	//IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf012));
	//IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d));
	//connect(ui->btnMenu_Close, SIGNAL(clicked()), this, SLOT(close()));
}
void QDrawWidget::InitForm()
{
	currentIndex = 0;
	plot1 = new QCustomPlot;
	plot1->setMouseTracking(true);//important  
	connect(plot1,&QCustomPlot::mouseMove,this, &QDrawWidget::mouseMoveEvent);
	
	//初始化随机数种子
	QTime time = QTime::currentTime();
	qsrand(time.msec() + time.second() * 1000);
}
void QDrawWidget::InitData(QString strtitle)
{
	m_Index = 0;
	plot1_values.clear();
	plot1_values_low.clear();
	plot1_values_high.clear();
	plot1->graph(0)->clearData();
	plot1->graph(1)->clearData();
	//plot1->graph(2)->clearData();
	plot1->clearItems();
	plot1Title->setText(strtitle);	
	plot1->rescaleAxes(true);
	plot1->replot();
}
void QDrawWidget::InitPlot()
{
	//设置纵坐标名称
	//plot1->yAxis->setLabel(tr("力度值(单位:N)"));


	//设置纵坐标范围
	//plot1->yAxis->setRange(0, Plot1_MaxY);
	
		//设置坐标颜色/坐标名称颜色
		plot1->yAxis->setLabelColor(TextColor);
		plot1->xAxis->setTickLabelColor(TextColor);
		plot1->yAxis->setTickLabelColor(TextColor);
		plot1->xAxis->setBasePen(QPen(TextColor, TextWidth));
		plot1->yAxis->setBasePen(QPen(TextColor, TextWidth));
		plot1->xAxis->setTickPen(QPen(TextColor, TextWidth));
		plot1->yAxis->setTickPen(QPen(TextColor, TextWidth));
		plot1->xAxis->setSubTickPen(QPen(TextColor, TextWidth));
		plot1->yAxis->setSubTickPen(QPen(TextColor, TextWidth));

		//设置画布背景色
		//QLinearGradient plotGradient;
		//plotGradient.setStart(0, 0);
		//plotGradient.setFinalStop(0, 350);
		//plotGradient.setColorAt(0, QColor(255, 255, 255));//(80, 80, 80));
		//plotGradient.setColorAt(1, QColor(220, 220, 220));//(50, 50, 50));
		//plot1->setBackground(plotGradient);
		plot1->setBackground( QColor(0, 0, 0));

		//设置坐标背景色
		//QLinearGradient axisRectGradient;
		//axisRectGradient.setStart(0, 0);
		//axisRectGradient.setFinalStop(0, 350);
		//axisRectGradient.setColorAt(0, QColor(255, 255, 255));
		//axisRectGradient.setColorAt(1, QColor(200, 200, 200));
		//plot1->axisRect()->setBackground(axisRectGradient);
		plot1->axisRect()->setBackground(QColor(0, 0, 0));
		//设置图例提示位置及背景色
		plot1->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
		plot1->legend->setBrush(QColor(255, 255, 255, 200));

		plot1->replot();

		//初始化 画图用的点的vector  
		receiveX0.resize(cnCount);
		receiveY0.resize(cnCount);
		sendX0.resize(cnCount);
		sendY0.resize(cnCount);

		for (int i=0; i<cnCount; ++i)//初始化横坐标
		{
			receiveX0[i] = (double)i*PI*2/(cnCount-1) - PI;//2pi/cnCount
			sendX0[i] = (double)i*PI*2/(cnCount-1) - PI;//2pi/cnCount
		}
}
void QDrawWidget::InitPlot1()
{
	plot1->plotLayout()->insertRow(0); 
	plot1Title = new QCPPlotTitle(plot1,"plot 1");
	plot1->plotLayout()->addElement(0, 0, plot1Title ); 
	
	plot1->addGraph();
	plot1->addGraph();
	//plot1->addGraph();
	plot1->graph(0)->setName(tr("receive"));
	plot1->graph(0)->setPen(QPen(Plot1_LineColor, LineWidth));
	plot1->graph(0)->setScatterStyle(
		QCPScatterStyle(QCPScatterStyle::ssNone,//ssCircle,
		QPen(Plot1_DotColor, LineWidth),
		QBrush(Plot1_DotColor), DotWidth));

	plot1->graph(1)->setName(tr("send"));
	plot1->graph(1)->setPen(QPen(Plot2_LineColor, LineWidth));
	plot1->graph(1)->setScatterStyle(
		QCPScatterStyle(QCPScatterStyle::ssNone,//ssCircle,
		QPen(Plot2_DotColor, LineWidth),
		QBrush(Plot2_DotColor), DotWidth));
	plot1->graph(1)->setChannelFillGraph(plot1->graph(0));//阴影图
	plot1->graph(1)->setBrush(QBrush(Plot1_BGColor));
	//设置静态曲线的横坐标范围及自适应横坐标
	plot1->xAxis->setRange(0, 7, Qt::AlignHCenter);
	plot1->yAxis->setRange(0,10,Qt::AlignVCenter);
	// 自动调整轴上值的显示范围
	plot1->rescaleAxes(true);


	on_ckBackground_stateChanged();
	on_ckMove_stateChanged();
	on_ckText_stateChanged();
}
void QDrawWidget::LoadPlot1()
{
	//plot1_values.clear();
	//plot1_values_1.clear();
	//plot1_values_2.clear();
	//for (int i = 0; i < Plot1_Count; i++) {
	//	double value = qrand() % Plot1_MaxY;
	//	plot1_values.append(value);
	//	plot1_values_1.append(40);
	//	plot1_values_2.append(10);
	//}
	//plot1->graph(0)->setData(plot1_keys, plot1_values);
	//plot1->graph(1)->setData(plot1_keys,plot1_values_1);
	//plot1->graph(2)->setData(plot1_keys,plot1_values_2);
	//plot1->replot();
}
void QDrawWidget::on_btnAdd_clicked()
{
	double value = qrand() % 100;
	double ptLow = qrand()%5 + 10;
	double ptHigh = qrand()%6 + 75;
	addPoint(value,ptLow,ptHigh);
}
void QDrawWidget::addTracerPoint(int index)
{
	QCPItemTracer *phaseTracer = new QCPItemTracer(plot1);
	plot1->addItem(phaseTracer);
	phaseTracer->setGraph(plot1->graph(0));
	phaseTracer->setGraphKey(index);
	phaseTracer->setInterpolating(true);
	phaseTracer->setStyle(QCPItemTracer::tsCircle);
	phaseTracer->setPen(QPen(Qt::red));
	phaseTracer->setBrush(Qt::red);
	phaseTracer->setSize(10);
	QString str = "add tracer point :"+QString::number(index);
	emit addOutput(str,2,1);
}
//画收到的数据正弦
void QDrawWidget::plotPoint(double dA,double dW)
{
	//尽量减少plot中的计算量，内存申请、不变的横坐标放到初始化中做
	//plot1->graph(0)->clearData();
	//QVector<double> x0(cnCount), y0(cnCount);
	for (int i=0; i<cnCount; ++i)
	{
		//receiveX0[i] = (double)i*3.14*2/cnCount - 3.14;//2pi/cnCount
		receiveY0[i] = (double)dA*qSin(dW*receiveX0[i]); // sinc function
	}
	plot1->graph(0)->setData(receiveX0, receiveY0);
	plot1->replot();
	//addPoint(dA);
}
//画发送的数据正弦
void QDrawWidget::plotSendPoint(double dA,double dW)
{
	//尽量减少plot中的计算量，内存申请、不变的横坐标放到初始化中做
	//plot1->graph(1)->clearData();
	//QVector<double> x0(cnCount), y0(cnCount);
	for (int i=0; i<cnCount; ++i)
	{
		//sendX0[i] = (double)i*3.14*2/cnCount - 3.14;//2pi/cnCount
		sendY0[i] = (double)dA*qSin(dW*sendX0[i]); // sinc function
	}
	plot1->graph(1)->setData(sendX0, sendY0);
	plot1->replot();
}
void QDrawWidget::addPoint(double ptValue,double ptLow,double ptHigh,int index)
{
	if (index = -1)
	{
		m_Index++;
		index = m_Index;
	}

	QString str;
	str = QString("index %1,value %2, low %3,high %4").arg(index).arg(ptValue).arg(ptLow).arg(ptHigh);
	emit addOutput(str);
	
	bool bChange = 0;
	if (ptValue>ptHigh || ptValue <ptLow)
	{
		plot1_Tracerkeys.append(index);

		if (btnBackg->isChecked())
		{
			addTracerPoint(index);
		}		

		plot1->graph(0)->setPen(QPen(QColor(255,255,0), LineWidth));
		bChange = true;
	}



	plot1_keys.append(index);
	plot1_values.append(ptValue);
	plot1_values_low.append(ptLow);
	plot1_values_high.append(ptHigh);

	plot1->graph(0)->addData(m_Index,ptValue);
	plot1->graph(1)->addData(m_Index,ptLow);
	//plot1->graph(2)->addData(m_Index,ptHigh);
	plot1->rescaleAxes(true);
	plot1->replot();
	if (bChange)
	{
		plot1->graph(0)->setPen(QPen(Plot1_LineColor, LineWidth));
		bChange = 0;
	}
}
void QDrawWidget::on_btnRestAxes_clicked()
{
	plot1->rescaleAxes(true);
	plot1->replot();
}
void QDrawWidget::on_btnReset_clicked()
{
	InitData();
}
void QDrawWidget::on_ckText_stateChanged()
{
	if (btnLegend->isChecked()) {
		plot1->legend->setVisible(true);
	} else {
		plot1->legend->setVisible(false);
	}
	plot1->replot();
}
void QDrawWidget::on_ckBackground_stateChanged()
{
	if (btnBackg->isChecked()) {
		plot1->graph(1)->setBrush(QBrush(Plot1_BGColor));
		int index;
		for (int n = 0;n<plot1_Tracerkeys.size();n++)
		{
			addTracerPoint(plot1_Tracerkeys[n]);
		}
	} else {
		plot1->graph(1)->setBrush(QBrush(Plot_NoColor));
		plot1->clearItems();
	}
	plot1->replot();
}
void QDrawWidget::on_ckMove_stateChanged()
{
	if (btnMove->isChecked()) {
		plot1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |QCP::iSelectLegend |QCP::iSelectItems);
	} else {
		plot1->setInteractions(QCP::iSelectOther);
	}
}
void QDrawWidget::on_tabWidget_currentChanged(int index)
{
	if (index == 0)
	{
		plot1->axisRect(0)->setRangeZoom(Qt::Horizontal|Qt::Vertical);
	}
	else if (index == 1)
	{
		plot1->axisRect(0)->setRangeZoom(Qt::Horizontal);
	}
	else
		plot1->axisRect(0)->setRangeZoom(Qt::Vertical);
}
void QDrawWidget::mouseMoveEvent(QMouseEvent *event)
{
	return;
	QPoint pt = QCursor::pos();
	QWidget *action = QApplication::widgetAt(pt);

	if(action == plot1&&plot1_keys.size()>0) 
	{
		//随鼠标浮动显示坐标值
		double xx = plot1->xAxis->pixelToCoord(event->x());
		if(xx<plot1_keys[0] || xx>plot1_keys[plot1_keys.size()-1]) return;
		
		int Index = 0;
		for (;Index<plot1_keys.size()-1;Index++)
		{
			if (xx>plot1_keys[Index]-0.05 && xx<plot1_keys[Index]+0.05)
			{
				break;
			}
		}
		double yx = plot1->yAxis->pixelToCoord(event->y());
		if (yx<plot1_values[Index]*0.9 || yx>plot1_values[Index]*1.1)
		{
			return QWidget::mouseMoveEvent(event);
		}
		QString str,strToolTip;
		str = QString::number(plot1_keys[Index]);
		strToolTip+="x: ";
		strToolTip+=str;
		strToolTip+="\n";
		str = QString("%L2").arg(plot1_values[Index],0,'f',0);
		strToolTip+="y: ";
		strToolTip+=str;
		QToolTip::showText(cursor().pos(),strToolTip,this);
	}
	return QWidget::mouseMoveEvent(event);
}
//画图区域截图
void QDrawWidget::ScreenShots()
{
	WId nwindId= plot1->winId();
	int nwidth = plot1->frameGeometry().width();
	int nheight = plot1->frameGeometry().height();
	int nx= plot1->x();
	int ny=plot1->y();

	//QPixmap pm = QPixmap::grabWindow(qApp->desktop()->winId(), this->x()+2, this->y()+2, this->frameGeometry().width()-4, this->frameGeometry().height()-4);
	QPixmap pm = QPixmap::grabWindow(nwindId, nx, ny, nwidth, nheight);

	QDateTime current_time = QDateTime::currentDateTime();
	QString fileName = current_time.toString("yyyy-MM-dd-HH-mm-ss")+".png";
	QString str=qApp->applicationDirPath();
	QFileInfo fi(str+"/"+fileName);
	QString strpath = str+"/screenshot_"+fileName;
	bool breturn = pm.save(strpath);
	emit savelog("save screen shot to file:" + strpath);
}