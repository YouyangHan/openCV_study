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
  void on_pushButton_channel_clicked();
  void on_pushButton_in_range_clicked();
  void on_pushButton_pixel_statistic_clicked();
  void on_pushButton_paint_clicked();
  void on_pushButton_paint_random_clicked();
  void on_pushButton_paint_polygon_clicked();
  void on_pushButton_mouse_clicked();
  void on_pushButton_pix_clicked();
  void on_pushButton_resize_clicked();
  void on_pushButton_rotate_clicked();
  void on_pushButton_histogram_clicked();
  void on_pushButton_histogram_two_clicked();
  void on_pushButton_histogram_equilibrium_clicked();
  void on_pushButton_image_convolute_clicked();
  void on_pushButton_gaussian_clicked();
  void on_pushButton_gaussian_two_clicked();
  void on_pushButton_face_clicked();
private:
    Ui::introductionClass *ui;

	cv::Mat img;

	QString imagePath;
};
