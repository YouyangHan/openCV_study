#pragma once

#include <iostream>

#include <QtWidgets/QWidget>
#include <QDir>
#include <QDebug>

#include "ui_actual.h"

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace  std;

namespace Ui{
  class actualClass;
}

class actual : public QWidget
{
    Q_OBJECT

public:
    actual(QWidget *parent = nullptr);
    ~actual();

public slots:
	void on_pushButton_two_value_clicked();
	void on_pushButton_two_value_overrall_clicked();
	void on_pushButton_two_value_self_clicked();

	void on_pushButton_unicom_clicked();
	void ccl_stats_demo(Mat &image);

	void on_pushButton_outline_clicked();
	void on_pushButton_outline_analysis_clicked();
	void on_pushButton_outline_match_clicked();
	void contour_info(Mat &image, vector<vector<Point>> &contours);
	void on_pushButton_outline_fit_clicked();
	void on_pushButton_outline_fit_circle_clicked();
	void fit_circle_demo(Mat &image);
private:
    Ui::actualClass *ui;
};
