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
