# 一、图像二值化

## 1.二值图像
- 灰度图像  0 - 255
- 二值图像  0（黑） /  255（白）

## 2.二值分割

五种阈值分割方法（阈值T）：

- 大于T为255，小于T为0

- 大于T为0，小于T为255

- 小于T为原值 else T

- 小于T为0 else 原值

- 大于T为0 else 原值

```cpp
    QString appPath = QCoreApplication::applicationDirPath();
	QString imagePath = appPath + "/A.jpg";
	Mat img = cv::imread(imagePath.toStdString()); 
	if (img.empty()) {
		return;
	}
	namedWindow("input", WINDOW_AUTOSIZE);
	imshow("input", img);

	//To gray image
	Mat gray,binary;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	imshow("gray", gray);

	//二值化
	threshold(gray, binary, 127, 255, THRESH_BINARY);
	imshow("THRESH_BINARY", binary);
	threshold(gray, binary, 127, 255, THRESH_BINARY_INV);
	imshow("THRESH_BINARY_INV", binary);

	//阈值化
	threshold(gray, binary, 127, 255, THRESH_TRUNC);
	imshow("THRESH_TRUNC", binary);
	threshold(gray, binary, 127, 255, THRESH_TOZERO);
	imshow("THRESH_TOZERO", binary);
	threshold(gray, binary, 127, 255, THRESH_TOZERO_INV);
	imshow("THRESH_TOZERO_INV", binary);

	waitKey();
	destroyAllWindows();
```

## 3.阈值

（1）全局阈值
```cpp
Scalar m = mean(gray);
threshold(gray, binary, m[0], 255, THRESH_BINARY);
imshow("THRESH_BINARY", binary);

//OTSU
double m_otsu = threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
imshow("THRESH_BINARY_OTSU", binary);

//三角法:X光片等
double m_triangle = threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_TRIANGLE);
imshow("THRESH_BINARY_TRIANGLE", binary);
```
（2）自适应阈值
```cpp
//均值c
adaptiveThreshold(gray, binary, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 25,10);
imshow("ADAPTIVE_THRESH_MEAN_C", binary);

//高斯c
adaptiveThreshold(gray, binary, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 25, 10);
imshow("ADAPTIVE_THRESH_GAUSSIAN_C", binary);
```

# 二、联通组件扫描（CCL）

>注：黑色背景
```cpp
void ccl_demo()
{

	QString appPath = QCoreApplication::applicationDirPath();
	QString imagePath = appPath + "/rice.png";
	Mat img = cv::imread(imagePath.toStdString());
	if (img.empty()) {
		return;
	}
	namedWindow("input", WINDOW_AUTOSIZE);
	imshow("input", img);

	//高斯模糊：降噪
	GaussianBlur(img, img, Size(3, 3),0);

	//To gray image
	Mat gray, binary;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	imshow("gray", gray);

	//OTSU
	double m_otsu = threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
	imshow("BINARY", binary);

	ccl_stats_demo(binary);

}
void ccl_stats_demo(Mat &image)
{
	Mat labels = Mat::zeros(image.size(), CV_32S);
	Mat stats, centrolds;
	int num_labels = connectedComponentsWithStats(image,labels,stats,centrolds,8, CV_32S, CCL_DEFAULT);

	vector<Vec3b> colorTable(num_labels);

	RNG rng(12345);
	//background color
	colorTable[0] = Vec3b(0, 0, 0);
	
	for (int i = 1; i < num_labels; i++)
	{
		colorTable[i] = Vec3b(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
	}
	Mat result = Mat::zeros(image.size(), CV_8UC3);
	int w = result.cols;
	int h = result.rows;
	for (int row = 0; row < h; row++)
	{
		for (int col = 0; col < w; col++)
		{
			int label = labels.at<int>(row, col);
			result.at<Vec3b>(row, col) = colorTable[label];
		}
	}

	for (int i = 1; i < num_labels; i++)
	{
		//center
		int cx = centrolds.at<double>(i, 0);
		int cy = centrolds.at<double>(i, 1);
		//rectangle
		int x = stats.at<int>(i, CC_STAT_LEFT);
		int y = stats.at<int>(i, CC_STAT_TOP);
		int w = stats.at<int>(i, CC_STAT_WIDTH);
		int h = stats.at<int>(i, CC_STAT_HEIGHT);
		int area = stats.at<int>(i, CC_STAT_AREA);

		circle(result, Point(cx, cy), 3, Scalar(0, 0, 255), 2, 8, 0);
		Rect box(x, y, w, h);
		rectangle(result, box, Scalar(0, 255, 0), 2, 8, 0);
		putText(result, format("%d", area), Point(x,y), FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 255, 255), 1);
	}

	putText(result, format("number:%d", num_labels-1), Point(10, 10), FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 255), 1);
	imshow("CCL demo", result);
}
```

# 三、轮廓分析

## 1.轮廓发现
- 基于联通组件
- 反映图像拓扑结构

```cpp
void outline_demo()
{
	QString appPath = QCoreApplication::applicationDirPath();
	QString imagePath = appPath + "/morph3.png";
	Mat img = cv::imread(imagePath.toStdString());
	if (img.empty()) {
		return;
	}
	namedWindow("input", WINDOW_AUTOSIZE);
	imshow("input", img);

	//高斯模糊：降噪
	GaussianBlur(img, img, Size(3, 3), 0);

	//To gray image
	Mat gray, binary;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	imshow("gray", gray);

	//OTSU
	double m_otsu = threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
	imshow("BINARY", binary);

	vector<vector<Point>> contours;
	vector<Vec4i> hierachy;
	findContours(binary, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
	drawContours(img, contours, -1, Scalar(0, 0, 255), 2, 8);

	imshow("outline", img);

	waitKey();
	destroyAllWindows();
}

```
## 2.轮廓分析
```cpp
void outline_analysis_demo()
{
	QString appPath = QCoreApplication::applicationDirPath();
	QString imagePath = appPath + "/zhifang_ball.png";
	Mat img = cv::imread(imagePath.toStdString());
	if (img.empty()) {
		return;
	}
	namedWindow("input", WINDOW_AUTOSIZE);
	imshow("input", img);

	//高斯模糊：降噪
	GaussianBlur(img, img, Size(3, 3), 0);

	//To gray image
	Mat gray, binary;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	imshow("gray", gray);

	//OTSU
	double m_otsu = threshold(gray, binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
	imshow("BINARY", binary);

	vector<vector<Point>> contours;
	vector<Vec4i> hierachy;
	findContours(binary, contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	for (size_t t = 0; t < contours.size(); t++)
	{
		double area = contourArea(contours[t]);
		double size = arcLength(contours[t],true);
		qDebug() << "area:" << area << "  " << "size:" << size;
		if (area < 100 || size < 10) continue;
		//最大矩形
		Rect box = boundingRect(contours[t]);
		rectangle(img,box,Scalar(0,0,255),2,8,0);
		//最小矩形
		RotatedRect rrt = minAreaRect(contours[t]);
		ellipse(img, rrt, Scalar(255, 0, 0), 2, 8);

		Point2f pts[4];
		rrt.points(pts);
		for (int i = 0; i < 4; i++)
		{
			line(img, pts[i], pts[(i + 1) % 4], Scalar(0, 255, 0), 2, 8);
		}
		//绘制轮廓
		drawContours(img, contours, t, Scalar(0, 0, 255), 2, 8);
	}

	imshow("outline", img);

	waitKey();
	destroyAllWindows();
}
```

## 3.轮廓匹配
```cpp
void outline_match_demo()
{
	QString appPath = QCoreApplication::applicationDirPath();
	Mat img1 = cv::imread(appPath.toStdString() + "/abc.png");
	Mat img2 = cv::imread(appPath.toStdString() + "/a5.png");
	if (img1.empty() || img2.empty()) {
		return;
	}
	imshow("input1", img1);
	imshow("input2", img2);

	vector<vector<Point>> contours1,contours2;
	contour_info(img1, contours1);
	contour_info(img2, contours2);

	Moments mm2 = moments(contours2[0]);
	Mat hu2;
	HuMoments(mm2, hu2);


	for (size_t t = 0; t < contours1.size(); t++)
	{
		Moments mm = moments(contours1[t]);
		double cx = mm.m10 / mm.m00;
		double cy = mm.m01 / mm.m00;
		circle(img1,Point(cx, cy), 3, Scalar(255, 0, 0), 2,8, 0);
		Mat hu;
		HuMoments(mm, hu);
		double dist = matchShapes(hu, hu2, CONTOURS_MATCH_I1,0);
		if (dist < 1.0)
		{
			drawContours(img1, contours1, t, Scalar(0, 0, 255), 2, 8);
		}
	}

	imshow("match contrours demo", img1);


	waitKey();
	destroyAllWindows();
}

void contour_info(Mat &image, vector<vector<Point>> &contours)
{
	//高斯模糊：降噪
	GaussianBlur(image, image, Size(3, 3), 0);

	//To gray image
	Mat gray, binary;
	cvtColor(image, gray, COLOR_BGR2GRAY);

	//OTSU
	double m_otsu = threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);

	vector<Vec4i> hierachy;
	findContours(binary, contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	
}
```

## 4.多边形逼近
```cpp
void outline_fit_clicked()
{
	QString appPath = QCoreApplication::applicationDirPath();
	QString imagePath = appPath + "/contours.png";
	Mat img = cv::imread(imagePath.toStdString());
	if (img.empty()) {
		return;
	}
	namedWindow("input", WINDOW_AUTOSIZE);
	imshow("input", img);

	//高斯模糊：降噪
	GaussianBlur(img, img, Size(3, 3), 0);

	//To gray image
	Mat gray, binary;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	imshow("gray", gray);

	//OTSU
	double m_otsu = threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
	imshow("BINARY", binary);

	vector<vector<Point>> contours;
	vector<Vec4i> hierachy;
	findContours(binary, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
	//多边形逼近
	for (size_t t = 0; t < contours.size(); t++)
	{
		Moments mm = moments(contours[t]);
		double cx = mm.m10 / mm.m00;
		double cy = mm.m01 / mm.m00;
		circle(img, Point(cx, cy), 3, Scalar(255, 0, 0), 2, 8, 0);

		double area = contourArea(contours[t]);
		double clen = arcLength(contours[t], true);
		
		Mat result;
		approxPolyDP(contours[t], result, 4, true);
		qDebug() << result.rows << " " << result.cols;

		if (result.rows == 6)
		{
			putText(img, "poly", Point(cx,cy-10), FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 255), 1,8);
		}
		if (result.rows == 4)
		{
			putText(img, "rectangle", Point(cx, cy - 10), FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 255), 1, 8);
		}
		if (result.rows == 3)
		{
			putText(img, "trriangle", Point(cx, cy - 10), FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 255), 1, 8);
		}
		if (result.rows >10)
		{
			putText(img, "circle", Point(cx, cy - 10), FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 255), 1, 8);
		}
	}


	imshow("多边形逼近", img);

	waitKey();
	destroyAllWindows();
}
```
## 5.拟合圆和椭圆
```cpp
void actual::fit_circle_demo(Mat &image)
{
	//To gray image
	Mat gray, binary;
	cvtColor(image, gray, COLOR_BGR2GRAY);

	//OTSU
	double m_otsu = threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);

	vector<vector<Point>> contours;
	vector<Vec4i> hierachy;
	findContours(binary, contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	//多边形逼近
	for (size_t t = 0; t < contours.size(); t++)
	{
		RotatedRect rrt = fitEllipse(contours[t]);
		float w = rrt.size.width;
		float h = rrt.size.height;
		Point center = rrt.center;
		circle(image, center, 3, Scalar(255.0, 0), 2, 8, 0);
		ellipse(image, rrt, Scalar(0, 255, 0), 2, 8);

	}


	imshow("拟合圆和椭圆", image);
}
```