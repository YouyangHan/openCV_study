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

# 四、霍夫曼检测

## 1.霍夫曼检测

```cpp
void hoffman_check_demo()
{
	QString appPath = QCoreApplication::applicationDirPath();
	QString imagePath = appPath + "/lines.png";
	Mat img = cv::imread(imagePath.toStdString());
	if (img.empty()) {
		return;
	}

	//To gray image
	Mat gray, binary;
	cvtColor(img, gray, COLOR_BGR2GRAY);

	//OTSU
	double m_otsu = threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
	imshow("BINARY", binary);

	//霍夫曼直线检测
	vector<Vec3f>lines;
	HoughLines(binary, lines, 1, CV_PI / 180.0, 100, 0, 0);

	//绘制直线
	Point pt1, pt2;
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0];//距离
		float theta = lines[i][1];//角度
		float acc = lines[i][2];//累加值

		double a = cos(theta);
		double b = sin(theta);
		double x0 = a * rho;
		double y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));

		int angle = round((theta/CV_PI)*180);

		if (rho > 0)//右倾
		{
			line(img, pt1, pt2, Scalar(255, 0, 0), 1, 8, 0);
			if(angle == 90)
				line(img, pt1, pt2, Scalar(255,255, 0), 1, 8, 0);
			if (angle <1)//垂直
				line(img, pt1, pt2, Scalar(0, 255, 255), 1, 8, 0);
		}
		else//左倾
		{
			line(img, pt1, pt2, Scalar(0, 0, 255), 1, 8, 0);
		}
	}
	imshow("hoffman lines", img);
	

	waitKey();
	destroyAllWindows();
}
```

## 2.霍夫曼直线检测
```cpp
void hoffman_check_line_demo()
{
	QString appPath = QCoreApplication::applicationDirPath();
	QString imagePath = appPath + "/morph01.png";
	Mat img = cv::imread(imagePath.toStdString());
	if (img.empty()) {
		return;
	}

	Mat binary;
	Canny(img, binary, 80, 160, 3, false);
	imshow("BINARY", binary);

	vector<Vec4i> lines;
	HoughLinesP(binary, lines, 1, CV_PI / 180.0, 80, 30, 10);
	Mat result = Mat::zeros(img.size(), img.type());
	for (size_t i = 0; i < lines.size(); i++)
	{
		line(result,Point(lines[i][0],lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(0, 0, 255), 2, 8, 0);
	}
	imshow("hoffman linesf", result);

	waitKey();
	destroyAllWindows();
}
```

## 3.霍夫曼圆检测
```cpp
void hoffman_check_circle_demo()
{
	QString appPath = QCoreApplication::applicationDirPath();
	QString imagePath = appPath + "/coins.png";
	Mat img = cv::imread(imagePath.toStdString());
	if (img.empty()) {
		return;
	}

	//To gray image
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	GaussianBlur(gray, gray, Size(3, 3), 0);

	imshow("Gray", gray);

	vector<Vec3f>circles;
	double minDist = 20;
	double min_radius = 15;
	double max_radius = 70;
	HoughCircles(gray, circles,HOUGH_GRADIENT, 3, minDist, 100, 100, min_radius,max_radius);
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(circles[i][0], circles[i][1]);
		int radius = round(circles[i][2]);
		circle(img, center,radius, Scalar(0, 0, 255), 2, 8, 0);
		circle(img, center,3, Scalar(0, 0, 255), 2, 8, 0);
	}
	imshow("circle", img);

	waitKey();
	destroyAllWindows();
}

```

# 五、图像形态学

## 1.膨胀和腐蚀

```cpp
void corrosion_expansion_demo()
{
	QString appPath = QCoreApplication::applicationDirPath();
	QString imagePath = appPath + "/LinuxLogo.jpg";
	Mat img = cv::imread(imagePath.toStdString());
	if (img.empty()) {
		return;
	}

	//To gray image
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);

	//OTSU
	Mat binary;
	double m_otsu = threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
	imshow("BINARY", binary);

	Mat erode_img,dilate_img;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));

	erode(binary, erode_img, kernel);
	imshow("erode", binary);

	dilate(binary, dilate_img, kernel);
	imshow("dilate", dilate_img);

	waitKey();
	destroyAllWindows(0);
}

```

## 2. 开闭操作

 - 开操作 = 腐蚀 + 膨胀 ———— 删除小的干扰快

 - 闭操作 = 膨胀 + 腐蚀 ———— 填充闭合区域

 
```cpp
void open_close_demo()
{
	QString appPath = QCoreApplication::applicationDirPath();
	QString imagePath = appPath + "/LinuxLogo.jpg";
	Mat img = cv::imread(imagePath.toStdString());
	if (img.empty()) {
		return;
	}

	//To gray image
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);

	//OTSU
	Mat binary;
	double m_otsu = threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
	imshow("BINARY", binary);

	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));

	Mat dst_open,dst_close;
	morphologyEx(binary, dst_open, MORPH_OPEN, kernel, Point(-1, -1), 1, 0);
	imshow("open", dst_open);

	morphologyEx(binary, dst_close, MORPH_CLOSE, kernel, Point(-1, -1), 1, 0);
	imshow("close", dst_close);

	waitKey(0);
	destroyAllWindows();
}
```
## 3.形态学梯度
- 基本梯度 ———— 膨胀减去腐蚀之后的结果

- 内梯度 ———— 原图减去腐蚀之后的结果

- 外梯度 ———— 膨胀减去原图的结果

```cpp
void morphological_gradient_demo()
{
	QString appPath = QCoreApplication::applicationDirPath();
	QString imagePath = appPath + "/yuan_test.png";
	Mat img = cv::imread(imagePath.toStdString());
	if (img.empty()) {
		return;
	}

	//To gray image
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);

	Mat basic_grad, inter_grad, exter_grad;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));

	morphologyEx(gray, basic_grad, MORPH_GRADIENT, kernel, Point(-1, -1), 1, 0);
	imshow("MORPH_GRADIENT", basic_grad);

	Mat dst1, dst2;
	erode(gray, dst1, kernel);
	dilate(gray, dst2, kernel);

	subtract(gray, dst1, inter_grad);
	imshow("inter_grad", inter_grad);

	subtract( dst2, gray, exter_grad);
	imshow("exter_grad", exter_grad);

	Mat binary;
	threshold(basic_grad,binary,0,255,THRESH_BINARY| THRESH_OTSU);
	imshow("binary", binary);

	waitKey(0);
	destroyAllWindows();
}

```

## 4.更多形态学操作
- 顶帽：原图减去开操作之后的结果
- 黑帽：是闭操作之后结果减去原图
>顶帽与黑帽的作用是用来提取图像中微小有用的信息块

- 击中击不中


```cpp
void more_morphological_gradient_demo()
{
	QString appPath = QCoreApplication::applicationDirPath();
	QString imagePath = appPath + "/cross.png";
	Mat img = cv::imread(imagePath.toStdString());
	if (img.empty()) {
		return;
	}

	//To gray image
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);

	Mat binary;
	threshold(gray, binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
	imshow("binary", binary);

	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	Mat dst_tophat;
	morphologyEx(binary, dst_tophat, MORPH_TOPHAT, kernel, Point(-1, -1), 1, 0);
	imshow("MORPH_TOPHAT", dst_tophat);//morph3.png

	Mat kernel_ellipse = getStructuringElement(MORPH_ELLIPSE, Size(15, 15), Point(-1, -1));
	Mat dst_blackhat;
	morphologyEx(binary, dst_blackhat, MORPH_BLACKHAT, kernel_ellipse, Point(-1, -1), 1, 0);
	imshow("MORPH_BLACKHAT", dst_blackhat);//morph3.png

	Mat dst_hit_miss;
	Mat kernel_cross = getStructuringElement(MORPH_CROSS, Size(15, 15), Point(-1, -1));
	morphologyEx(binary, dst_hit_miss, MORPH_HITMISS, kernel_cross);
	imshow("MORPH_HITMISS", dst_hit_miss);//cross.png

	waitKey(0);
	destroyAllWindows();
}
```

# 六、二值图像分析

```cpp
void two_value_analysis_demo()
{
	QString appPath = QCoreApplication::applicationDirPath();
	QString imagePath = appPath + "/case6.jpg";
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

	//闭操作
	Mat se = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
	morphologyEx(binary, binary, MORPH_CLOSE, se);
	
	imshow("BINARY", binary);

	int height = binary.rows;
	int width = binary.cols;
	vector<vector<Point>> contours;
	vector<Vec4i> hierachy;
	findContours(binary, contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	double max_area = -1;
	int cindex = -1;
	for (size_t t = 0; t < contours.size(); t++)
	{
		Rect rect = boundingRect(contours[t]);
		if (rect.height >= height || rect.width >= width)continue;
		double area = contourArea(contours[t]);
		double size = arcLength(contours[t], true);
		qDebug() << "area:" << area << "  " << "size:" << size;

		if (area > max_area)
		{
			max_area = area;
			cindex = t;
		}

		//if (area < 100 || size < 10) continue;
	
		//绘制轮廓
		//drawContours(img, contours, t, Scalar(0, 0, 255), 1, 8);
	}
	Mat result = Mat::zeros(img.size(), img.type());
	drawContours(result, contours, cindex, Scalar(0, 0, 255), 2, 8);
	drawContours(img, contours, cindex, Scalar(0, 0, 255), 2, 8);

	Mat pts;
	approxPolyDP(contours[cindex], pts, 8, true);
	for (int i = 0; i < pts.rows; i++)
	{
		Vec2i pt = pts.at<Vec2i>(i, 0);
		circle(img, Point(pt[0], pt[1]), 2, Scalar(0, 255, 0));
		circle(result, Point(pt[0], pt[1]), 2, Scalar(0, 255, 0));
	}
	imshow("Max contours", img);

	imshow("result", result);

	waitKey(0);
	destroyAllWindows();
}
```