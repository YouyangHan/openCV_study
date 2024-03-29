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

	void ccl_stats_demo(Mat &image);
public slots:
	void on_pushButton_two_value_clicked();
	void on_pushButton_two_value_overrall_clicked();
	void on_pushButton_two_value_self_clicked();

	void on_pushButton_unicom_clicked();
private:
    Ui::actualClass *ui;
};
