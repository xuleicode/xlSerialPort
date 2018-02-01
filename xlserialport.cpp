#include "xlserialport.h"

#include <QWaitCondition>
#include <QMutex>
#include <QSplitter>

#include <QPushButton>
#include <QTabBar>
#include <QString>

#include <QTime>
#include <QTimer>
#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QString>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QDockWidget>
const QString strTitle="Serial port assistent v0.0.1";
xlSerialPort::xlSerialPort(QWidget *parent)
	: QMainWindow(parent)
{
	//ui.setupUi(this);
	setupUI();
	initThread();
	initUI();
	statusBar()->showMessage("Ready");
}

xlSerialPort::~xlSerialPort()
{

}
void xlSerialPort::setupUI()
{	
#pragma region pushbutton
	
#pragma endregion pushbutton
#pragma region Action
	//action============
	AcFileOpen= new QAction((QIcon(":/Action/FileOpen")),tr("&Open"),this);
	AcFileOpen->setShortcut(QKeySequence::Open);
	connect(AcFileOpen,SIGNAL(triggered()),this,SLOT(FileOpen()));

	AcFileSave = new QAction((QIcon(":/Action/FileSave")),tr("&Save"),this);
	AcFileSave->setShortcut(QKeySequence::Save);
	connect(AcFileSave,SIGNAL(triggered()),this,SLOT(FileSave()));

	AcAbout = new QAction(QIcon(":/Action/About"),tr("About"),this);
	//	AcLinkOut->setShortcut(tr("Ctrl+H"));
	connect(AcAbout,SIGNAL(triggered()),this,SLOT(About()));

	AcToolBar1 = new QAction(QIcon(":/Action/ToolBar1"),tr("&EditToolBar"),this);
	AcToolBar1->setShortcut(tr("Ctrl+Shift+E"));
	AcToolBar1->setCheckable(true); 
	connect(AcToolBar1,SIGNAL(triggered()),this,SLOT(ShowToolBar()));

	AcComWid = new QAction(QIcon(":/Action/comwid"),tr("&show/hide assist window"),this);
	AcComWid->setShortcut(tr("Ctrl+Shift+S"));
	AcComWid->setStatusTip("show/hide assist window");
	AcComWid->setCheckable(true); 
	connect(AcComWid,SIGNAL(triggered()),this,SLOT(ShowComWidget()));
	AcPlotWid =  new QAction(QIcon(":/Action/plotwid"),tr("&show/hide plot widget"),this);
	AcPlotWid->setShortcut(tr("Ctrl+Shift+P"));
	AcPlotWid->setStatusTip("show/hide plot widget");
	AcPlotWid->setCheckable(true); 
	connect(AcPlotWid,SIGNAL(triggered()),this,SLOT(ShowPlotWidget()));

	AcScreenShot = new QAction(QIcon(":/Action/screenshot"),tr("&Screen Shot"),this);
	AcScreenShot->setShortcut(tr("Ctrl+Shift+S"));
	AcScreenShot->setStatusTip("make a screen shot for plot widget");
	connect(AcScreenShot,SIGNAL(triggered()),this,SIGNAL(screenShot()));

#pragma endregion Action

#pragma region Menubar
	//Menu==================
	QMenu *menufile = menuBar()->addMenu(tr("&File")); 
	menufile->addAction(AcFileOpen);
	menufile->addAction(AcFileSave); 

	QMenu *menuView = menuBar()->addMenu(tr("&View")); 
	menuView->addAction(AcToolBar1);
	menuView->addAction(AcComWid);
	menuView->addAction(AcPlotWid);
	menuView->addAction(AcScreenShot);

	//menu help
	QMenu *menuHelp = menuBar()->addMenu(tr("&Help")); 
	menuHelp->addAction(AcAbout); 

#pragma endregion Menubar
#pragma region ToolBar
//ToolBar======
	toolBar1 = addToolBar(tr("&toolbar"));
	toolBar1->addAction(AcComWid);
	toolBar1->addAction(AcPlotWid);
	toolBar1->addAction(AcScreenShot);


#pragma endregion ToolBar

#pragma region StatusBar
	//statusMsg = new QLabel(tr("Ready"));
	//statusMsg->setMinimumSize(statusMsg->sizeHint());
	//statusMsg->setAlignment(Qt::AlignHCenter);
	//statusMsg->setFrameShape(QFrame::WinPanel); // 设置标签形状
	//statusMsg->setFrameShadow(QFrame::Sunken); // 设置标签阴影
	//statusBar()->addWidget(statusMsg);

	labelTime = new QLabel(tr("Time"));
	m_pTimer = new QTimer(this);
	m_pTimer->start(1000);
	connect(m_pTimer,SIGNAL(timeout()),this,SLOT(timedisplay()));	
	statusBar()->setStyleSheet(QString("QStatusBar::item{border:0px}"));
	//statusBar()->setSizeGripEnabled(0);//右下角的大小控制点
	statusBar()->addPermanentWidget(labelTime); //现实永久信息
#pragma endregion StatusBar

#pragma region DockWidget
	//停靠窗口
	//graph
	dockGraph1= new QDockWidget(tr("Graph-1"), this ); 
	dockGraph1->setFeatures( QDockWidget::AllDockWidgetFeatures ); 
	dockGraph1->setAllowedAreas( Qt::AllDockWidgetAreas);
	m_drawWidget = new QDrawWidget();
	dockGraph1->setWidget(m_drawWidget); 
	//dockGraph1->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	connect(dockGraph1,SIGNAL(visibilityChanged(bool)),this,SLOT(setShowDockeGraph1PropertyCheck(bool)));

	dockCom =new QDockWidget(tr("SerialPortAssist"), this ); 
	dockCom->setFeatures( QDockWidget::AllDockWidgetFeatures ); 
	dockCom->setAllowedAreas( Qt::AllDockWidgetAreas);
	m_comWidget = new QComWidget();
	dockCom->setWidget(m_comWidget);
	//dockCom->setFeatures(QDockWidget::DockWidgetMovable);
	connect(dockCom,SIGNAL(visibilityChanged(bool)),this,SLOT(setShowDockeComPropertyCheck(bool)));

	addDockWidget( Qt::RightDockWidgetArea, dockGraph1);//右
	addDockWidget( Qt::LeftDockWidgetArea, dockCom);//左
#pragma endregion DockWidget

#pragma region centerWidget
	//QWidget* centralWidget = new QWidget(this);
	//m_centralWidget = new MainWindow(this);
	
	//setCentralWidget(m_centralWidget);
	//QVBoxLayout *centerLayout = new QVBoxLayout();
	//centerLayout->setContentsMargins(0,0,0,1);
	//centerLayout->addWidget(mytab);
	//centralWidget->setLayout(centerLayout);
#pragma endregion centerWidget


//设置style 图标，大小，标题
	QIcon icon;
	icon.addFile(QString::fromUtf8(":/images/in.ico"), QSize(), QIcon::Normal, QIcon::Off);
	setWindowIcon(icon);
	setWindowTitle(strTitle);
	
	resize(700, 500); //设置显示大小
	//setWindowState(Qt::WindowMaximized);//最大化显示
	//SetsystemTray();//设置托盘

	
}
void xlSerialPort::initUI()
{
	AcComWid->setChecked(true);
	AcPlotWid->setChecked(true);
	AcToolBar1->setChecked(true);
	ShowComWidget();
	ShowToolBar();
	ShowPlotWidget();
	connect(this,&xlSerialPort::screenShot,m_drawWidget,&QDrawWidget::ScreenShots);
	connect(m_drawWidget,&QDrawWidget::savelog,this,&xlSerialPort::showStatusMessage);
}
void xlSerialPort::initThread()
{
	//receive 画图
	m_parseData = new ParseData();
	m_parseData->start();

}

void xlSerialPort::SetsystemTray()//托盘程序
{
	QIcon icon(":/images/in.ico");
	systemTray = new QSystemTrayIcon(this);
	systemTray->setIcon(icon);
	systemTray->setToolTip(tr("system tray exe"));
	QAction* minimumAct = new QAction(tr("Minimum Window"), this);
	//Note the differences between hide() and showMinimized().
	connect(minimumAct, SIGNAL(triggered()), this, SLOT(hide()));
	QAction* maximumAct = new QAction(tr("Maximum Window"), this);
	connect(maximumAct, SIGNAL(triggered()), this, SLOT(showMaximized()));
	QAction* restoreAct = new QAction(tr("Restore Window"), this);
	connect(restoreAct, SIGNAL(triggered()), this, SLOT(showNormal()));
	QAction* quitAct = new QAction(tr("Quit Application"), this);
	connect(quitAct, SIGNAL(triggered()), qApp, SLOT(quit()));

	QMenu* pContextMenu = new QMenu(this);
	pContextMenu->addAction(minimumAct);
	pContextMenu->addAction(maximumAct);
	pContextMenu->addAction(restoreAct);
	pContextMenu->addSeparator();
	pContextMenu->addAction(quitAct);
	systemTray->setContextMenu(pContextMenu);
	systemTray->show();
	systemTray->showMessage(tr("Tips"), tr("The program is running behind!"));
}
void xlSerialPort::timedisplay()
{
	QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
	QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式
	labelTime->setText(str);
//	QTime current_time = QTime::currentTime();
//	labelTime->setText(current_time.toString(" HH:mm:ss"));
}
void xlSerialPort::ShowComWidget()
{
	if (AcComWid->isChecked())
	{
		dockCom->show();
	}
	else
		dockCom->hide();
}
void xlSerialPort::ShowPlotWidget()
{
	if(AcPlotWid->isChecked())
	{
		dockGraph1->show();
		emit showStatusMessage("show plot widget");
	}
	else
	{
		dockGraph1->hide();
		emit showStatusMessage("hide plot widget");
	}
}
void xlSerialPort::setShowDockeComPropertyCheck(bool bshow)
{
	AcComWid->setChecked(bshow);
}
void xlSerialPort::setShowDockeGraph1PropertyCheck(bool bshow)
{
	AcPlotWid->setChecked(bshow);
	AcScreenShot->setEnabled(bshow);
	if (bshow)
	{
		//receive 画图
		connect(m_parseData,&ParseData::addPoint,m_drawWidget,&QDrawWidget::plotPoint);	
		connect(m_comWidget,&QComWidget::addPoint,m_drawWidget,&QDrawWidget::plotSendPoint);
	}
	else
	{
		//画图太频繁影响计算机性能，当画图窗口隐藏时，断开连接，不画图，等显示的时候再进行连接画图。
		//receive 画图
		disconnect(m_parseData,&ParseData::addPoint,m_drawWidget,&QDrawWidget::plotPoint);
		//send 画图
		disconnect(m_comWidget,&QComWidget::addPoint,m_drawWidget,&QDrawWidget::plotSendPoint);
	}
}
void xlSerialPort::ShowToolBar()
{
	if(AcToolBar1->isChecked())
		toolBar1->show();
	else
		toolBar1->hide();
}
void xlSerialPort::showStatusMessage(QString strMsg)
{
	statusBar()->showMessage(strMsg,2000);
}
void xlSerialPort::About()
{
	QMessageBox::about(this, "About", strTitle+"\nCopyright 2018-2020 XL Ltd. All rights reserved");
}