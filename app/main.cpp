#include <QtWidgets/QApplication>
#include <iostream>

#include "openCV_study.h"

#include <opencv2/opencv.hpp>

using namespace  std;
using namespace cv;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	openCV_study cv;
	cv.show();

	return a.exec();
}
