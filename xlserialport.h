#ifndef XLSERIALPORT_H
#define XLSERIALPORT_H

#include <QtWidgets/QMainWindow>
#include "ui_xlserialport.h"
#include "qdrawwidget.h"
#include "qcomwidget.h"

class QAction;
class QToolBar;
class QPushButton;
class QLabel;
class QMenu;
class QSystemTrayIcon;
class QString;
class QTimer;
class QDockWidget;
class MainWindow;
class xlSerialPort : public QMainWindow
{
	Q_OBJECT

public:
	xlSerialPort(QWidget *parent = 0);
	~xlSerialPort();

private:
	Ui::xlSerialPortClass ui;


	//Action
	QAction *AcFileOpen;
	QAction *AcFileSave;
	QAction *AcAbout;
	QAction *AcToolBar1;
	QAction *AcComWid;
	QAction *AcPlotWid;
	//toolbar
	QToolBar *toolBar1;
	//label
	QLabel *statusMsg;
	QLabel *labelTime;

	QSystemTrayIcon *systemTray;
	QTimer* m_pTimer;


	MainWindow* m_centralWidget;
	QDockWidget *dockGraph1;
	QDrawWidget* m_drawWidget;
	QDockWidget* dockCom;
	QComWidget* m_comWidget;

	ParseData* m_parseData;
private:
	void setupUI();
	void initUI();
	void initThread();
	void SetsystemTray();
private slots:
	void timedisplay();
	void ShowComWidget();
	void ShowPlotWidget();
	void ShowToolBar();
	void About();
};

#endif // XLSERIALPORT_H
