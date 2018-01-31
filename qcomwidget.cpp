#include "qcomwidget.h"
#include "myhelper.h"

QQueue<pReceiveDataStruct> queue;
QWaitCondition queueIsNotEmpty;
QMutex mutex;


QComWidget::QComWidget(QWidget *parent)
	: QWidget(parent)
{
    setupUi(this);
	getSerialPortInfo();


	//初始化随机数种子
	QTime time = QTime::currentTime();
	qsrand(time.msec() + time.second() * 1000);
	m_pTimer = new QTimer(this);
	connect(m_pTimer,SIGNAL(timeout()),this,SLOT(autosend()));
}
QComWidget::~QComWidget()
{

}
void QComWidget::setupUi(QWidget *pWidget)
{
	if (pWidget->objectName().isEmpty())
		pWidget->setObjectName(QStringLiteral("MainWindow"));
	horizontalLayout_7 = new QHBoxLayout(pWidget);
	horizontalLayout_7->setSpacing(6);
	horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
	horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
	verticalLayout = new QVBoxLayout();
	verticalLayout->setSpacing(6);
	verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
	label = new QLabel(pWidget);
	label->setObjectName(QStringLiteral("label"));

	verticalLayout->addWidget(label);

	textEdit = new QTextEdit(pWidget);
	textEdit->setObjectName(QStringLiteral("textEdit"));

	verticalLayout->addWidget(textEdit);

	label_2 = new QLabel(pWidget);
	label_2->setObjectName(QStringLiteral("label_2"));

	verticalLayout->addWidget(label_2);

	textEdit_2 = new QTextEdit(pWidget);
	textEdit_2->setObjectName(QStringLiteral("textEdit_2"));

	verticalLayout->addWidget(textEdit_2);

	horizontalLayout = new QHBoxLayout();
	horizontalLayout->setSpacing(6);
	horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
	clearButton = new QPushButton(pWidget);
	clearButton->setObjectName(QStringLiteral("clearButton"));

	horizontalLayout->addWidget(clearButton);

	sendButton = new QPushButton(pWidget);
	sendButton->setObjectName(QStringLiteral("sendButton"));

	horizontalLayout->addWidget(sendButton);


	verticalLayout->addLayout(horizontalLayout);


	horizontalLayout_7->addLayout(verticalLayout);

	verticalLayout_2 = new QVBoxLayout();
	verticalLayout_2->setSpacing(6);
	verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
	verticalLayout_3 = new QVBoxLayout();
	verticalLayout_3->setSpacing(6);
	verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
	label_8 = new QLabel(pWidget);
	label_8->setObjectName(QStringLiteral("label_8"));

	verticalLayout_3->addWidget(label_8);

	horizontalLayout_2 = new QHBoxLayout();
	horizontalLayout_2->setSpacing(6);
	horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
	label_3 = new QLabel(pWidget);
	label_3->setObjectName(QStringLiteral("label_3"));

	horizontalLayout_2->addWidget(label_3);

	PortBox = new QComboBox(pWidget);
	PortBox->setObjectName(QStringLiteral("PortBox"));

	horizontalLayout_2->addWidget(PortBox);


	verticalLayout_3->addLayout(horizontalLayout_2);

	horizontalLayout_3 = new QHBoxLayout();
	horizontalLayout_3->setSpacing(6);
	horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
	label_4 = new QLabel(pWidget);
	label_4->setObjectName(QStringLiteral("label_4"));

	horizontalLayout_3->addWidget(label_4);

	BaudBox = new QComboBox(pWidget);
	BaudBox->setObjectName(QStringLiteral("BaudBox"));

	horizontalLayout_3->addWidget(BaudBox);


	verticalLayout_3->addLayout(horizontalLayout_3);

	horizontalLayout_4 = new QHBoxLayout();
	horizontalLayout_4->setSpacing(6);
	horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
	label_5 = new QLabel(pWidget);
	label_5->setObjectName(QStringLiteral("label_5"));

	horizontalLayout_4->addWidget(label_5);

	BitNumBox = new QComboBox(pWidget);
	BitNumBox->setObjectName(QStringLiteral("BitNumBox"));

	horizontalLayout_4->addWidget(BitNumBox);


	verticalLayout_3->addLayout(horizontalLayout_4);

	horizontalLayout_5 = new QHBoxLayout();
	horizontalLayout_5->setSpacing(6);
	horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
	label_6 = new QLabel(pWidget);
	label_6->setObjectName(QStringLiteral("label_6"));

	horizontalLayout_5->addWidget(label_6);

	ParityBox = new QComboBox(pWidget);
	ParityBox->setObjectName(QStringLiteral("ParityBox"));

	horizontalLayout_5->addWidget(ParityBox);


	verticalLayout_3->addLayout(horizontalLayout_5);

	horizontalLayout_6 = new QHBoxLayout();
	horizontalLayout_6->setSpacing(6);
	horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
	label_7 = new QLabel(pWidget);
	label_7->setObjectName(QStringLiteral("label_7"));

	horizontalLayout_6->addWidget(label_7);

	StopBox = new QComboBox(pWidget);
	StopBox->setObjectName(QStringLiteral("StopBox"));

	horizontalLayout_6->addWidget(StopBox);


	verticalLayout_3->addLayout(horizontalLayout_6);

	ckHexSend = new QCheckBox(pWidget);
	ckHexSend->setObjectName(QStringLiteral("ckHexSend"));
	ckHexReceive = new QCheckBox(pWidget);
	ckHexReceive->setObjectName(QStringLiteral("ckHexReceive"));
	refreshButton = new QPushButton(pWidget);
	refreshButton->setObjectName(QStringLiteral("refreshButton"));
	connect(refreshButton,&QPushButton::clicked,this,&QComWidget::getSerialPortInfo);
	verticalLayout_3->addWidget(ckHexSend);
	verticalLayout_3->addWidget(ckHexReceive);
	verticalLayout_3->addWidget(refreshButton);

	ckAutoSend=new QCheckBox(pWidget);
	ckAutoSend->setObjectName(QStringLiteral("ckAutoSend"));
	timeEdit = new QLineEdit (pWidget);
	verticalLayout_3->addWidget(ckAutoSend);
	verticalLayout_3->addWidget(timeEdit);
	verticalLayout_3->addStretch();

	verticalLayout_2->addLayout(verticalLayout_3);

	verticalSpacer = new QSpacerItem(20, 68, QSizePolicy::Minimum, QSizePolicy::Expanding);

	verticalLayout_2->addItem(verticalSpacer);

	openButton = new QPushButton(pWidget);
	openButton->setObjectName(QStringLiteral("openButton"));

	verticalLayout_2->addWidget(openButton);


	horizontalLayout_7->addLayout(verticalLayout_2);

	this->setLayout(horizontalLayout_7);
	retranslateUi(pWidget);
	pWidget->resize(484, 311);

	QMetaObject::connectSlotsByName(pWidget);
}
void QComWidget::retranslateUi(QWidget *pWidget)
{
	pWidget->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
	label->setText(QApplication::translate("MainWindow", "\346\216\245\346\224\266\347\252\227\345\217\243", 0));
	label_2->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201\347\252\227\345\217\243", 0));
	clearButton->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272\346\216\245\346\224\266", 0));
	sendButton->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201\346\225\260\346\215\256", 0));
	label_8->setText(QString());
	label_3->setText(QApplication::translate("MainWindow", "\347\253\257\345\217\243\345\217\267\357\274\232", 0));
	label_4->setText(QApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207\357\274\232", 0));
	BaudBox->clear();
	BaudBox->insertItems(0, QStringList()
		<< QApplication::translate("MainWindow", "1200", 0)
		<< QApplication::translate("MainWindow", "2400", 0)
		<< QApplication::translate("MainWindow", "4800", 0)
		<< QApplication::translate("MainWindow", "9600", 0)
		<< QApplication::translate("MainWindow", "19200", 0)
		);
	label_5->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\344\275\215\357\274\232", 0));
	BitNumBox->clear();
	BitNumBox->insertItems(0, QStringList()
		<< QApplication::translate("MainWindow", "8", 0)
		);
	label_6->setText(QApplication::translate("MainWindow", "\346\240\241\351\252\214\344\275\215\357\274\232", 0));
	ParityBox->clear();
	ParityBox->insertItems(0, QStringList()
		<< QApplication::translate("MainWindow", "0", 0)
		);
	label_7->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\344\275\215\357\274\232", 0));
	StopBox->clear();
	StopBox->insertItems(0, QStringList()
		<< QApplication::translate("MainWindow", "1", 0)
		<< QApplication::translate("MainWindow", "2", 0)
		);
	openButton->setText(QString::fromLocal8Bit("打开串口"));
	refreshButton->setText(QString::fromLocal8Bit("刷新串口"));

	ckHexSend->setText(QApplication::translate("frmComTool", "Hex\345\217\221\351\200\201", 0));
	ckHexReceive->setText(QApplication::translate("frmComTool", "Hex\346\216\245\346\224\266", 0));
	ckAutoSend->setText(QString::fromLocal8Bit("自动发送"));
	timeEdit->setText("100");
} // retranslateUi
void QComWidget::getSerialPortInfo()
{
	//查找可用的串口
	PortBox->clear();
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		QSerialPort serial;
		serial.setPort(info);
		if(serial.open(QIODevice::ReadWrite))
		{
			PortBox->addItem(serial.portName());
			serial.close();
		}
	}
	//设置波特率下拉菜单默认显示第三项
	BaudBox->setCurrentIndex(3);
	//关闭发送按钮的使能
	sendButton->setEnabled(false);
	//    qDebug() << tr("界面设定成功！");
}
//清空接受窗口
void QComWidget::on_clearButton_clicked()
{
	textEdit->clear();
}
//发送数据
void QComWidget::on_sendButton_clicked()
{
	sendData();
	if (ckAutoSend->isChecked())
	{
		m_pTimer->start(timeEdit->text().toInt());
	}
}
void QComWidget::sendData()
{
	QString str = textEdit_2->toPlainText();

	if (str.isEmpty()) {
		textEdit_2->setFocus();
		return;
	}
	sendData(str);
}
void QComWidget::sendData(QString data)
{
	if (serial == 0 || !serial->isOpen()) {
		return;
	}

	QByteArray buffer;

	if (ckHexSend->isChecked()) {
		buffer = myHelper::hexStrToByteArray(data);
	} else {
		buffer = myHelper::asciiStrToByteArray(data);
	}
	  

	ReceiveDataStruct pdata;
	pdata.f1=qrand()%5;//这两行发送随机数 测试使用
	pdata.f2=qrand()%5;//这两行发送随机数 测试使用

	serial->write((const char*)&pdata,sizeof(pdata));
	emit addPoint(pdata.f1,pdata.f2);//发送信号 画图信息 幅度，频率
}
//读取接收到的数据
void QComWidget::Read_Data()
{
	if (serial->bytesAvailable() <= 0) {
		return;
	}
	myHelper::sleep(10);//延迟10毫秒

	m_tempBuf = serial->readAll();
	if(!m_tempBuf.isEmpty())
	{
		m_receiveBuf.append(m_tempBuf);
		QString strbuf;
		if (ckHexReceive->isChecked()) {
			strbuf = myHelper::byteArrayToHexStr(m_tempBuf);
		} else {
			strbuf = myHelper::byteArrayToAsciiStr(m_tempBuf);
		}
		textEdit->append(strbuf);
		//要处理的数据
		if (m_receiveBuf.size()>=sizeof(ReceiveDataStruct))
		{
			pReceiveDataStruct pdata = new ReceiveDataStruct;
			memcpy(pdata,m_receiveBuf.data(),sizeof(ReceiveDataStruct));
			addtoQueue(pdata);
			m_receiveBuf.remove(0,sizeof(ReceiveDataStruct));
		}
		m_tempBuf.clear();
	}
}
void QComWidget::on_openButton_clicked()
{
	if(openButton->text()==QString::fromLocal8Bit("打开串口"))
	{
		serial = new QSerialPort;
		//设置串口名
		serial->setPortName(PortBox->currentText());
		//打开串口
		bool breturn = serial->open(QIODevice::ReadWrite);
		if (!breturn)
		{
			QMessageBox::critical(this, ("error"), serial->errorString());
			return;
		}
		//设置波特率
		serial->setBaudRate(BaudBox->currentText().toInt());
		//设置数据位数
		switch(BitNumBox->currentIndex())
		{
		case 8: serial->setDataBits(QSerialPort::Data8); break;
		default: break;
		}
		//设置奇偶校验
		switch(ParityBox->currentIndex())
		{
		case 0: serial->setParity(QSerialPort::NoParity); break;
		default: break;
		}
		//设置停止位
		switch(StopBox->currentIndex())
		{
		case 1: serial->setStopBits(QSerialPort::OneStop); break;
		case 2: serial->setStopBits(QSerialPort::TwoStop); break;
		default: break;
		}
		//设置流控制
		serial->setFlowControl(QSerialPort::NoFlowControl);

		//关闭设置菜单使能
		PortBox->setEnabled(false);
		BaudBox->setEnabled(false);
		BitNumBox->setEnabled(false);
		ParityBox->setEnabled(false);
		StopBox->setEnabled(false);
		StopBox->setEnabled(false);
		refreshButton->setEnabled(false);
		openButton->setText(QString::fromLocal8Bit("关闭串口"));
		sendButton->setEnabled(true);

		//连接信号槽
		QObject::connect(serial, &QSerialPort::readyRead, this, &QComWidget::Read_Data);
	}
	else
	{
		if (NULL == serial)
		{
			return;
		}
		//关闭串口
		serial->clear();
		serial->close();
		serial->deleteLater();
		serial=0;
		//恢复设置使能
		PortBox->setEnabled(true);
		BaudBox->setEnabled(true);
		BitNumBox->setEnabled(true);
		ParityBox->setEnabled(true);
		StopBox->setEnabled(true);
		refreshButton->setEnabled(true);
		openButton->setText(QString::fromLocal8Bit("打开串口"));
		sendButton->setEnabled(false);
	}
}
void QComWidget::on_ckAutoSend_clicked()
{
	if (!ckAutoSend->isChecked())
	{
		m_pTimer->stop();
		timeEdit->setEnabled(true);
	}
	else
		timeEdit->setEnabled(false);
}
void QComWidget::addtoQueue(pReceiveDataStruct p)
{
	mutex.lock();
	queue.enqueue(p);
	mutex.unlock();	
	queueIsNotEmpty.wakeAll();
}
void QComWidget::autosend()
{
	sendData();//自动发送信息   非测试使用，一直发送发送框内的内容
	//if (!ckAutoSend->isChecked())
	//{
	//	m_pTimer->stop();
	//}
}


//class parsedata

void ParseData::run()
{
	qDebug() << "+++++++++ParseData+++++++++" << QThread::currentThreadId();  
	while(1)
	{
		mutex.lock();
		while(queue.isEmpty())
			queueIsNotEmpty.wait(&mutex);		
		pReceiveDataStruct pdata = queue.dequeue();
		mutex.unlock();
		emit addPoint(pdata->f1,pdata->f2);
		delete pdata;
	}
}


