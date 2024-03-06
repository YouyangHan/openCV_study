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
	QString imagePath = appPath + "/sun.png";

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
	//单通道灰度图像
	vector<int> opts;
	opts.push_back(IMWRITE_PAM_FORMAT_GRAYSCALE);
	imwrite("D:/gray.png", img,opts);

}
