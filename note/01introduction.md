
# 一、读取图像
```cpp

    //图片路径
	QString appPath = QCoreApplication::applicationDirPath();
	QString imagePath = appPath + "/sun.png";

	//读取图像
	cv::Mat img = cv::imread(imagePath.toStdString()); //IMREAD_GRAYSCALE 灰度图 IMREAD_UNCHANGED 具有透明通道

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

```

# 二、保存图像

```cpp

	//保存为png透明通道
	vector<int>opts;
	opts.push_back(IMWRITE_PAM_FORMAT_RGB_ALPHA);
	imwrite("D:/img_bgra.png", img, opts);


	//保存为单通道灰度图像
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

```

# 三、加载视频

```cpp
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
```
# 四、获取视频属性

```cpp
	//视频路径
	QString appPath = QCoreApplication::applicationDirPath();
	QString videoPath = appPath + "/vtest.avi";
	cv::VideoCapture capture;
	capture.open(videoPath.toStdString(),CAP_FFMPEG);

	qDebug() << u8"高：" << capture.get(CAP_PROP_FRAME_HEIGHT);
	qDebug() << u8"宽：" << capture.get(CAP_PROP_FRAME_WIDTH);
	qDebug() << u8"帧率FPS：" << QString::number(capture.get(CAP_PROP_FPS));
	qDebug() << u8"总帧率：" << capture.get(CAP_PROP_FRAME_COUNT);
```
# 五、保存视频

```cpp
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
```

# 六、Mat遍历

1. 数组下标
```cpp
for (int row = 0; row < h; row++)
  {
	for (int col = 0; col < w; col++)
	{
	  if (dim == 1)//灰度图像
	  {
		int pv = image.at<uchar>(row,col);//像素是字节类型
		image.at<uchar>(row, col) = 255 - pv;
	  }
	  if (dim == 3)//彩色图像
	  {
		Vec3b bgr = image.at<Vec3b>(row, col);
		image.at<Vec3b>(row, col)[0] = 255 - bgr[0];
		image.at<Vec3b>(row, col)[1] = 255 - bgr[1];
		image.at<Vec3b>(row, col)[2] = 255 - bgr[2];
	  }
	}
  }


```

2. 指针
```cpp
for (int row = 0; row < h; row++)
{
	uchar* current_row = image.ptr<uchar>(row);
	for (int col = 0; col < w; col++)
	{
	  if (dim == 1)//灰度图像
	  {
		int pv = *current_row;
		*current_row++ = 255 - *current_row;

	  }
	  if (dim == 3)//彩色图像
	  {
		*current_row++ = 255 - *current_row;
		*current_row++ = 255 - *current_row;
		*current_row++ = 255 - *current_row;
	  }
	}
}


```

# 七、Mat加减乘除
```cpp
#include <opencv2/opencv.hpp>

using namespace cv;

int main() {
    Mat image = imread("image.jpg");

    if (image.empty()) {
        std::cout << "无法加载图像" << std::endl;
        return -1;
    }

    // 加法变换
    Mat addResult;
    add(image, Scalar(50, 50, 50), addResult); // 将像素值增加50

    // 减法变换
    Mat subtractResult;
    subtract(image, Scalar(50, 50, 50), subtractResult); // 将像素值减去50

    // 乘法变换
    Mat multiplyResult;
    multiply(image, Scalar(0.5, 0.5, 0.5), multiplyResult); // 将像素值乘以0.5

    // 除法变换
    Mat divideResult;
    divide(image, Scalar(2.0, 2.0, 2.0), divideResult); // 将像素值除以2.0

    imshow("Original Image", image);
    imshow("Addition Result", addResult);
    imshow("Subtraction Result", subtractResult);
    imshow("Multiplication Result", multiplyResult);
    imshow("Division Result", divideResult);

    waitKey(0);

    return 0;
}

```
