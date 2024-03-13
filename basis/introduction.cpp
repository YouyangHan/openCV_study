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
	imagePath = appPath + "/sun.png";

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
	capture.open(videoPath.toStdString(),CAP_FFMPEG);
	//从摄像头读取
	//capture.open(0, CAP_DSHOW);

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
