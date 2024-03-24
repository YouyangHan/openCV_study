#include "introduction.h"

introduction::introduction(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::introductionClass)
{
    ui->setupUi(this);


}

introduction::~introduction()
{}

void introduction::on_pushButton_first_openncv_program_clicked()
{
	//图片路径
	QString appPath = QCoreApplication::applicationDirPath();
	imagePath = appPath + "/A.jpg";

	//读取图像
	img = cv::imread(imagePath.toStdString()); //IMREAD_GRAYSCALE 灰度图 IMREAD_UNCHANGED 具有透明通道

	if (img.empty()) {
		qDebug()<< "Could not load image!\n";
		return;
	}

	//创建窗口
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	//显示图像
	cv::imshow("input", img);
	//参数为0一直阻塞
	cv::waitKey(0);
	//销毁窗口
	cv::destroyAllWindows();
}

void introduction::on_pushButton_save_image_clicked()
{
	//保存为png透明通道
	vector<int>opts;
	opts.push_back(IMWRITE_PAM_FORMAT_RGB_ALPHA);
	imwrite("D:/img_bgra.png", img, opts);


	//单通道灰度图像
	img = cv::imread(imagePath.toStdString(), IMREAD_GRAYSCALE);
	vector<int> opts_gray;
	opts_gray.push_back(IMWRITE_PAM_FORMAT_GRAYSCALE);
	imwrite("D:/img_gray.png", img, opts_gray);

	//保存为默认的彩色BGR图像
	imwrite("D:/img_BGR.png", img);

	//保存为png彩色压缩图像
	img = imread(imagePath.toStdString(), IMREAD_ANYCOLOR);
	vector<int> opts_png_anycolor;
	opts_png_anycolor.push_back(IMWRITE_PAM_FORMAT_GRAYSCALE);
	opts_png_anycolor.push_back(9);
	imwrite("D:/img_png_anycolor.png", img, opts_png_anycolor);

	//保存为JPG高压缩比图像
	img = imread(imagePath.toStdString(), IMREAD_COLOR);
	vector<int> opts_jpeg;
	opts_jpeg.push_back(IMWRITE_JPEG_QUALITY);
	opts_jpeg.push_back(50);
	opts_jpeg.push_back(IMWRITE_JPEG_OPTIMIZE);
	opts_jpeg.push_back(1);
	imwrite("D:/img_jpeg.jpg", img, opts_jpeg);

}

void introduction::on_pushButton_video_clicked()
{
	//视频路径
	QString appPath = QCoreApplication::applicationDirPath();
	QString videoPath = appPath + "/vtest.avi";
	cv::VideoCapture capture;
	//capture.open(videoPath.toStdString(),CAP_FFMPEG);
	//从摄像头读取
	capture.open(0, CAP_DSHOW);

	cv::Mat frame;
	while (true) {
		//读帧
		bool ret = capture.read(frame);
		if (!ret)
			break;
		imshow("frame", frame);
		//添加帧处理 等待 100 毫秒，检查用户是否按下键盘上的按键
		char c = waitKey(100);
		//按下的是 ESC 键 (ASCII 值为 27)，则跳出循环，停止播放视频
		if (c == 27)
			break;
	}
	waitKey(0);
	destroyAllWindows();
}

void introduction::on_pushButton_video_info_clicked()
{
	//视频路径
	QString appPath = QCoreApplication::applicationDirPath();
	QString videoPath = appPath + "/vtest.avi";
	cv::VideoCapture capture;
	capture.open(videoPath.toStdString(),CAP_FFMPEG);

	qDebug() << u8"高：" << capture.get(CAP_PROP_FRAME_HEIGHT);
	qDebug() << u8"宽：" << capture.get(CAP_PROP_FRAME_WIDTH);
	qDebug() << u8"帧率FPS：" << QString::number(capture.get(CAP_PROP_FPS));
	qDebug() << u8"总帧率：" << capture.get(CAP_PROP_FRAME_COUNT);
}

void introduction::on_pushButton_save_video_clicked()
{
	//视频路径
	QString appPath = QCoreApplication::applicationDirPath();
	QString videoPath = appPath + "/vtest.avi";
	cv::VideoCapture capture;
	capture.open(videoPath.toStdString(), CAP_FFMPEG);


	qDebug() << u8"高：" << capture.get(CAP_PROP_FRAME_HEIGHT);
	qDebug() << u8"宽：" << capture.get(CAP_PROP_FRAME_WIDTH);
	qDebug() << u8"帧率FPS：" << capture.get(CAP_PROP_FPS);
	qDebug() << u8"总帧率：" << capture.get(CAP_PROP_FRAME_COUNT);

	//保存视频路径
	QString savevideoPath = appPath + "/output.avi";

	cv::VideoWriter writer(savevideoPath.toStdString(), capture.get(CAP_PROP_FOURCC), capture.get(CAP_PROP_FPS), Size(capture.get(CAP_PROP_FRAME_WIDTH), capture.get(CAP_PROP_FRAME_HEIGHT)));

	cv::Mat frame;
	while (true) {
		//读帧
		bool ret = capture.read(frame);
		if (!ret)
			break;
		imshow("frame", frame);
		//添加帧处理 
		writer.write(frame);
		//等待 100 毫秒，检查用户是否按下键盘上的按键
		char c = waitKey(100);
		//按下的是 ESC 键 (ASCII 值为 27)，则跳出循环，停止播放视频
		if (c == 27)
			break;
	}
	capture.release();
	writer.release();
	waitKey(0);
	destroyAllWindows();
}



static void on_track_lightness(int b, void* user_data)
{
	Mat image = *((Mat *)user_data);
	if (image.empty())
		return;
	Mat dst = Mat::zeros(image.size(), image.type());
	Mat m = Mat::zeros(image.size(), image.type());

	addWeighted(image, 1.0, m, 0, b, dst);

	imshow("亮度对比度调节", dst);
}

static void on_track_contrast(int b, void* user_data)
{
	Mat image = *((Mat *)user_data);
	Mat dst = Mat::zeros(image.size(), image.type());
	Mat m = Mat::zeros(image.size(), image.type());

	double contrast = b / 100.0;
	addWeighted(image, contrast, m, 0.0, 0, dst);

	imshow("亮度对比度调节", dst);
}

void introduction::on_pushButton_brightness_clicked()
{
	namedWindow("亮度对比度调节", WINDOW_AUTOSIZE);
	
	int lightness = 50;
	int max_value = 100;

	int contrast_value = 100;

	createTrackbar("Value Bar:", "亮度对比度调节", &lightness, max_value, on_track_lightness,(void*)(&img));
	createTrackbar("Constrast Bar:", "亮度对比度调节", &contrast_value, 200, on_track_contrast, (void*)(&img));
	on_track_lightness(50, &img);
}

void introduction::on_pushButton_keyboard_clicked()
{
  //图片路径
  QString appPath = QCoreApplication::applicationDirPath();
  imagePath = appPath + "/A.jpg";

  //读取图像
  img = cv::imread(imagePath.toStdString());
  if (img.empty())
    return;

  cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
  //显示图像
  cv::imshow("input", img);

  Mat dst = Mat::zeros(img.size(),img.type());
  while (true) {
    char c = waitKey(100);
    std::cout << c << std::endl;
    if (c == 27)
      break;
    if (c == 49)//Key 1
    {
      cvtColor(img, dst, COLOR_BGR2GRAY);
    }
    if (c == 50)//Key 2
    {
      cvtColor(img, dst, COLOR_BGR2HSV);
    }
    if (c == 51)//Key 3
    {
      dst = Scalar(50, 50, 50);
      add(img, dst, dst);
    }
    cv::imshow("input", dst);
  }

  //参数为0一直阻塞
  cv::waitKey(0);
  //销毁窗口
  cv::destroyAllWindows();
}

void introduction::on_pushButton_color_surface_clicked()
{
  //图片路径
  QString appPath = QCoreApplication::applicationDirPath();
  imagePath = appPath + "/A.jpg";

  //读取图像
  img = cv::imread(imagePath.toStdString());
  if (img.empty())
    return;

  cv::namedWindow("input", cv::WINDOW_AUTOSIZE);

  cv::Mat dst;

  int color_map = COLORMAP_AUTUMN;
  while (true)
  {
    int c = cv::waitKey(2000);
    if (c == 27) {
      break;
    }
    applyColorMap(img, dst, color_map%21);
    color_map++;
    cv::imshow("input", dst);
  }

  cv::waitKey(0);
  cv::destroyAllWindows();
}

void introduction::on_pushButton_logic_clicked()
{
  cv::Mat m1 = cv::Mat::zeros(Size(256, 256), CV_8UC3);
  cv::Mat m2 = cv::Mat::zeros(Size(256, 256), CV_8UC3);

  rectangle(m1, Rect(100, 100, 80, 80),Scalar(255,255,0),-1,LINE_8,0);
  rectangle(m2, Rect(150, 150, 80, 80), Scalar(0, 255, 255), -1, LINE_8, 0);

  imshow("m1", m1);
  imshow("m2", m2);

  Mat dst;
  bitwise_and(m1, m2, dst);
  imshow("与操作", dst);

  bitwise_or(m1, m2, dst);
  imshow("或操作", dst);

  QString appPath = QCoreApplication::applicationDirPath();
  imagePath = appPath + "/A.jpg";
  img = cv::imread(imagePath.toStdString());
  if (img.empty())
    return;
  bitwise_not(img, dst);
  imshow("非操作", dst);

  bitwise_xor(m1, m2, dst);
  imshow("异或操作", dst);

  cv::waitKey(0);
  cv::destroyAllWindows();
}

void introduction::on_pushButton_channel_clicked()
{
	QString appPath = QCoreApplication::applicationDirPath();
	imagePath = appPath + "/A.jpg";
	img = cv::imread(imagePath.toStdString());
	if (img.empty())
		return;

	std::vector<Mat> mv;
	cv::split(img, mv);

	imshow("B", mv[0]);
	imshow("G", mv[1]);
	imshow("R", mv[2]);

	cv::Mat dst;
	mv[1] = 0;
	mv[2] = 0;

	cv::merge(mv, dst);
	imshow("Blue", dst);

	int from_to[] = { 0,2,1,1,2,0 };
	cv::mixChannels(&img, 1, &dst, 1, from_to,3);
	imshow("通道混合", dst);
}

void introduction::on_pushButton_in_range_clicked()
{
	QString appPath = QCoreApplication::applicationDirPath();
	imagePath = appPath + "/sun.png";
	img = cv::imread(imagePath.toStdString());
	if (img.empty())
		return;
	imshow("img", img);
	cv::Mat hsv;
	cv::cvtColor(img,hsv,COLOR_BGR2HSV);

	cv::Mat mask;
	inRange(hsv, Scalar(0,0,221), Scalar(180,30,255), mask);

	Mat redback = Mat::zeros(img.size(), img.type());
	redback = Scalar(40, 40, 200);
	bitwise_not(mask, mask);

	imshow("mask", mask);

	img.copyTo(redback, mask);

	imshow("roi", redback);
}