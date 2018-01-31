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

	AcComWid = new QAction(QIcon(":/Action/comwid"),tr("&SerialPortAsssist Widget"),this);
	AcComWid->setShortcut(tr("Ctrl+Shift+S"));
	AcComWid->setCheckable(true); 
	connect(AcComWid,SIGNAL(triggered()),this,SLOT(ShowComWidget()));
	AcPlotWid =  new QAction(QIcon(":/Action/plotwid"),tr("&Plot Widget"),this);
	AcPlotWid->setShortcut(tr("Ctrl+Shift+P"));
	AcPlotWid->setCheckable(true); 
	connect(AcPlotWid,SIGNAL(triggered()),this,SLOT(ShowPlotWidget()));
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

	//menu help
	QMenu *menuHelp = menuBar()->addMenu(tr("&Help")); 
	menuHelp->addAction(AcAbout); 

#pragma endregion Menubar
#pragma region ToolBar
//ToolBar======
	toolBar1 = addToolBar(tr("&toolbar"));
	toolBar1->addAction(AcComWid);
	toolBar1->addAction(AcPlotWid);


#pragma endregion ToolBar

#pragma region StatusBar
	statusMsg = new QLabel(tr("Ready"));
	labelTime = new QLabel(tr("Time"));
	m_pTimer = new QTimer(this);
	m_pTimer->start(1000);
	connect(m_pTimer,SIGNAL(timeout()),this,SLOT(timedisplay()));
	statusBar()->addWidget(statusMsg);
	statusBar()->setStyleSheet(QString("QStatusBar::item{border:0px}"));
	//statusBar()->setSizeGripEnabled(0);
	statusBar()->addPermanentWidget(labelTime); //��ʵ������Ϣ
#pragma endregion StatusBar

#pragma region DockWidget
	//ͣ������
	//graph
	dockGraph1= new QDockWidget(tr("Graph-1"), this ); 
	dockGraph1->setFeatures( QDockWidget::AllDockWidgetFeatures ); 
	dockGraph1->setAllowedAreas( Qt::AllDockWidgetAreas);
	m_drawWidget = new QDrawWidget();
	dockGraph1->setWidget(m_drawWidget); 

	dockCom =new QDockWidget(tr("SerialPortAssist"), this ); 
	dockCom->setFeatures( QDockWidget::AllDockWidgetFeatures ); 
	dockCom->setAllowedAreas( Qt::AllDockWidgetAreas);
	m_comWidget = new QComWidget();
	dockCom->setWidget(m_comWidget);

	addDockWidget( Qt::RightDockWidgetArea, dockGraph1);//��
	addDockWidget( Qt::LeftDockWidgetArea, dockCom);//��
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


//����style ͼ�꣬��С������
	QIcon icon;
	icon.addFile(QString::fromUtf8(":/images/in.ico"), QSize(), QIcon::Normal, QIcon::Off);
	setWindowIcon(icon);
	setWindowTitle(strTitle);
	
	resize(700, 500); //������ʾ��С
	//setWindowState(Qt::WindowMaximized);//�����ʾ
	//SetsystemTray();//��������

	
}
void xlSerialPort::initUI()
{
	AcComWid->setChecked(true);
	AcPlotWid->setChecked(true);
	AcToolBar1->setChecked(true);
	ShowComWidget();
	ShowToolBar();
	ShowPlotWidget();
}
void xlSerialPort::initThread()
{
	//receive ��ͼ
	m_parseData = new ParseData();
	m_parseData->start();
}

void xlSerialPort::SetsystemTray()//���̳���
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
//	QDateTime time = QDateTime::currentDateTime();//��ȡϵͳ���ڵ�ʱ��
//	QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd"); //������ʾ��ʽ

	QTime current_time = QTime::currentTime();
	labelTime->setText(current_time.toString(" HH:mm:ss"));
	//m_pTimer->start(1000);

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
		//receive ��ͼ
		connect(m_parseData,&ParseData::addPoint,m_drawWidget,&QDrawWidget::plotPoint);	
		connect(m_comWidget,&QComWidget::addPoint,m_drawWidget,&QDrawWidget::plotSendPoint);
	}
	else
	{
		//��ͼ̫Ƶ��Ӱ���������ܣ�����ͼ��������ʱ���Ͽ����ӣ�����ͼ������ʾ��ʱ���ٽ������ӻ�ͼ��
		dockGraph1->hide();
		//receive ��ͼ
		disconnect(m_parseData,&ParseData::addPoint,m_drawWidget,&QDrawWidget::plotPoint);
		//send ��ͼ
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
void xlSerialPort::About()
{
	QMessageBox::about(this, "About", strTitle+"\nCopyright 2018-2020 XL Ltd. All rights reserved");
}