#include "actual.h"

actual::actual(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::actualClass)
{
    ui->setupUi(this);


}

actual::~actual()
{}

void actual::on_pushButton_two_value_clicked()
{
	QString appPath = QCoreApplication::applicationDirPath();
	QString imagePath = appPath + "/A.jpg";
	Mat img = cv::imread(imagePath.toStdString()); //IMREAD_GRAYSCALE 灰度图 IMREAD_UNCHANGED 具有透明通道
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

}

void actual::on_pushButton_two_value_overrall_clicked()
{
	QString appPath = QCoreApplication::applicationDirPath();
	QString imagePath = appPath + "/A.jpg";
	Mat img = cv::imread(imagePath.toStdString());
	if (img.empty()) {
		return;
	}
	namedWindow("input", WINDOW_AUTOSIZE);
	imshow("input", img);

	//To gray image
	Mat gray, binary;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	imshow("gray", gray);

	Scalar m = mean(gray);
	threshold(gray, binary, m[0], 255, THRESH_BINARY);
	imshow("THRESH_BINARY", binary);

	//OTSU
	double m_otsu = threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
	imshow("THRESH_BINARY_OTSU", binary);

	//三角法
	double m_triangle = threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_TRIANGLE);
	imshow("THRESH_BINARY_TRIANGLE", binary);

	waitKey();
	destroyAllWindows();
}

void actual::on_pushButton_two_value_self_clicked()
{
	QString appPath = QCoreApplication::applicationDirPath();
	QString imagePath = appPath + "/A.jpg";
	Mat img = cv::imread(imagePath.toStdString());
	if (img.empty()) {
		return;
	}
	namedWindow("input", WINDOW_AUTOSIZE);
	imshow("input", img);

	//To gray image
	Mat gray, binary;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	imshow("gray", gray);

	//MEAN_C
	adaptiveThreshold(gray, binary, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 25,10);
	imshow("ADAPTIVE_THRESH_MEAN_C", binary);

	//高斯
	adaptiveThreshold(gray, binary, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 25, 10);
	imshow("ADAPTIVE_THRESH_GAUSSIAN_C", binary);

	waitKey();
	destroyAllWindows();
}

void actual::on_pushButton_unicom_clicked()
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

void actual::ccl_stats_demo(Mat &image)
{
	Mat labels = Mat::zeros(image.size(), CV_32S);
	Mat stats, centrolds;
	int num_labels = connectedComponentsWithStats(image,labels,stats,centrolds,8, CV_32S, CCL_DEFAULT);

	vector<Vec3b> colorTable(num_labels);

	//background color
	RNG rng(12345);
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
