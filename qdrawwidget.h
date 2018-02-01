#ifndef QDRAWWIDGET_H
#define QDRAWWIDGET_H
#include "qcustomplot.h"
#include <QWidget>
//µãÏßÍ¼´°¿Ú
class QDrawWidget : public QWidget
{
	Q_OBJECT

public:
	explicit QDrawWidget(QWidget *parent = 0);
	~QDrawWidget();



	void addPoint(double ptValue = 0,double ptLow = 0,double ptHigh = 100,int index = -1);
	
	void addTracerPoint(int index);
public slots:
	void plotPoint(double dA,double dW);
	void plotSendPoint(double dA,double dW);
	void ScreenShots();
private slots:
	void LoadPlot1();
	void on_btnAdd_clicked();
	void on_btnRestAxes_clicked();
	void on_btnReset_clicked();
	void on_ckText_stateChanged();
	void on_ckBackground_stateChanged();
	void on_ckMove_stateChanged();
	void on_tabWidget_currentChanged(int arg1);
	void mouseMoveEvent(QMouseEvent *event);

private:

	QTimer *plot2_timer;
	QTimer *plot5_timer;

	int currentIndex;
	QString currentSkin;
	QList <QCustomPlot *> plots;
	QCPPlotTitle* plot1Title;
	QCustomPlot* plot1;
	QCustomPlot* plot2;
	int m_Index;
	QVector<int> plot1_keys;
	QVector<double> plot1_values;
	QVector<double> plot1_values_low;
	QVector<double> plot1_values_high;
	QVector<int> plot1_Tracerkeys;

	QVector<double> plot1_keys_Error;
	QVector<double> plot1_values_Error;
	QVector<double> plot1_values_ErrorPlus;

	double plot2_key;
	double plot2_value;

	QVector<double> plot3_keys;
	QVector<double> plot3_values1, plot3_values2, plot3_values3;
	QVector<QString> plot3_labels;
	QVector<QCPBars *> plot3_Bar;

	QVector<double> plot4_keys;
	QVector<double> plot4_values;
	QVector<QString> plot4_labels;
	QVector<QCPBars *> plot4_Bar;

	QVector<double> plot5_keys;
	QVector<double> plot5_values;
	QVector<double> plot5_currentValue;
	int maxValue;
	QVector<QString> plot5_labels;
	QVector<QCPBars *> plot5_Bar;


	QVector<double> receiveX0, receiveY0;
	QVector<double> sendX0, sendY0;

	QPushButton *btnAdd;
	QPushButton *btnReset;
	QPushButton *btnResetAxes;
	QCheckBox   *btnMove;
	QCheckBox	*btnBackg;
	QCheckBox	*btnLegend;
	//QPushButton *btnSkin;
	//QPushButton *btnLoad;
	//QPushButton *btnSkin;
	void initUI();
	void InitStyle();
	void InitForm();

	void InitPlot();
	void InitPlot1();
	void InitData(QString strtitle = "NULL");
signals:
	void addOutput(QString strlog,int nicon = 0,int nWindow = 0);
	void savelog(QString);

};
#endif // QDRAWWIDGET_H
