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
	//ͼƬ·��
	QString appPath = QCoreApplication::applicationDirPath();
	QString imagePath = appPath + "/sun.png";

	//��ȡͼ��
	img = cv::imread(imagePath.toStdString()); //IMREAD_GRAYSCALE �Ҷ�ͼ IMREAD_UNCHANGED ����͸��ͨ��

	if (img.empty()) {
		qDebug()<< "Could not load image!\n";
		return;
	}

	//��������
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	//��ʾͼ��
	cv::imshow("input", img);
	//����Ϊ0һֱ����
	cv::waitKey(0);
	//���ٴ���
	cv::destroyAllWindows();
}

void introduction::on_pushButton_save_image_clicked()
{
	//��ͨ���Ҷ�ͼ��
	vector<int> opts;
	opts.push_back(IMWRITE_PAM_FORMAT_GRAYSCALE);
	imwrite("D:/gray.png", img,opts);

}
