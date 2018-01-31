#include "xlserialport.h"
#include <QtWidgets/QApplication>


//test
#include "qcomwidget.h"
//test==end
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	xlSerialPort w;
	w.show();
	return a.exec();
}
