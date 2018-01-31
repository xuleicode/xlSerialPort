#ifndef QCOMWIDGET_H
#define QCOMWIDGET_H

#include <QWidget>





#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>


#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


//parsedata include==
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QVariant>
//parsedata include==end

#include <QMetaType> 

#include <QTimer>
typedef struct
{
	int n1;
	float f1;
}ReceiveDataStruct,*pReceiveDataStruct;
Q_DECLARE_METATYPE(ReceiveDataStruct);
Q_DECLARE_METATYPE(pReceiveDataStruct);

class QComWidget : public QWidget
{
	Q_OBJECT

public:
	QComWidget(QWidget *parent=0);
	~QComWidget();


public:
	QHBoxLayout *horizontalLayout_7;
	QVBoxLayout *verticalLayout;
	QLabel *label;
	QTextEdit *textEdit;
	QLabel *label_2;
	QTextEdit *textEdit_2;
	QHBoxLayout *horizontalLayout;
	QPushButton *clearButton;
	QPushButton *sendButton;
	QVBoxLayout *verticalLayout_2;
	QVBoxLayout *verticalLayout_3;
	QLabel *label_8;
	QHBoxLayout *horizontalLayout_2;
	QLabel *label_3;
	QComboBox *PortBox;
	QHBoxLayout *horizontalLayout_3;
	QLabel *label_4;
	QComboBox *BaudBox;
	QHBoxLayout *horizontalLayout_4;
	QLabel *label_5;
	QComboBox *BitNumBox;
	QHBoxLayout *horizontalLayout_5;
	QLabel *label_6;
	QComboBox *ParityBox;
	QHBoxLayout *horizontalLayout_6;
	QLabel *label_7;
	QComboBox *StopBox;
	QSpacerItem *verticalSpacer;
	QPushButton *openButton;
	QCheckBox *ckHexSend;
	QCheckBox *ckHexReceive;
	QPushButton *refreshButton;
	QCheckBox *ckAutoSend;
	QLineEdit *timeEdit;

	QSerialPort *serial;

	QTimer * m_pTimer;
private:
	void setupUi(QWidget*);
	void retranslateUi(QWidget*);
	void getSerialPortInfo();

	void sendData();
	void sendData(QString data);
	void inline addtoQueue(pReceiveDataStruct);
private slots:
	void on_clearButton_clicked();
	void on_sendButton_clicked();
	void on_openButton_clicked();
	void Read_Data();
	void autosend();
	void on_ckAutoSend_clicked();
	
signals:
	void addPoint(double ptValue,double);
};



class ParseData : public QThread
{
	Q_OBJECT
public:
	void run();
signals:
	void addPoint(double da,double dw);
};
#endif // QCOMWIDGET_H
