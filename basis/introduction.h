#pragma once

#include <iostream>

#include <QtWidgets/QWidget>
#include <QDir>
#include <QDebug>

#include "ui_introduction.h"

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace  std;

namespace Ui{
  class introductionClass;
}

class introduction : public QWidget
{
    Q_OBJECT

public:
    introduction(QWidget *parent = nullptr);
    ~introduction();

public slots:
	void on_pushButton_first_openncv_program_clicked();
	void on_pushButton_save_image_clicked();
	void on_pushButton_video_clicked();
	void on_pushButton_video_info_clicked();
	void on_pushButton_save_video_clicked();
	void on_pushButton_brightness_clicked();
  void on_pushButton_keyboard_clicked();
  void on_pushButton_color_surface_clicked();
  void on_pushButton_logic_clicked();
private:
    Ui::introductionClass *ui;

	cv::Mat img;

	QString imagePath;
};
