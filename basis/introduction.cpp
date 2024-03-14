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
	imagePath = appPath + "/A.jpg";

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
	//����Ϊpng͸��ͨ��
	vector<int>opts;
	opts.push_back(IMWRITE_PAM_FORMAT_RGB_ALPHA);
	imwrite("D:/img_bgra.png", img, opts);


	//��ͨ���Ҷ�ͼ��
	img = cv::imread(imagePath.toStdString(), IMREAD_GRAYSCALE);
	vector<int> opts_gray;
	opts_gray.push_back(IMWRITE_PAM_FORMAT_GRAYSCALE);
	imwrite("D:/img_gray.png", img, opts_gray);

	//����ΪĬ�ϵĲ�ɫBGRͼ��
	imwrite("D:/img_BGR.png", img);

	//����Ϊpng��ɫѹ��ͼ��
	img = imread(imagePath.toStdString(), IMREAD_ANYCOLOR);
	vector<int> opts_png_anycolor;
	opts_png_anycolor.push_back(IMWRITE_PAM_FORMAT_GRAYSCALE);
	opts_png_anycolor.push_back(9);
	imwrite("D:/img_png_anycolor.png", img, opts_png_anycolor);

	//����ΪJPG��ѹ����ͼ��
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
	//��Ƶ·��
	QString appPath = QCoreApplication::applicationDirPath();
	QString videoPath = appPath + "/vtest.avi";
	cv::VideoCapture capture;
	capture.open(videoPath.toStdString(),CAP_FFMPEG);
	//������ͷ��ȡ
	//capture.open(0, CAP_DSHOW);

	cv::Mat frame;
	while (true) {
		//��֡
		bool ret = capture.read(frame);
		if (!ret)
			break;
		imshow("frame", frame);
		//���֡���� �ȴ� 100 ���룬����û��Ƿ��¼����ϵİ���
		char c = waitKey(100);
		//���µ��� ESC �� (ASCII ֵΪ 27)��������ѭ����ֹͣ������Ƶ
		if (c == 27)
			break;
	}
	waitKey(0);
	destroyAllWindows();
}

void introduction::on_pushButton_video_info_clicked()
{
	//��Ƶ·��
	QString appPath = QCoreApplication::applicationDirPath();
	QString videoPath = appPath + "/vtest.avi";
	cv::VideoCapture capture;
	capture.open(videoPath.toStdString(),CAP_FFMPEG);

	qDebug() << u8"�ߣ�" << capture.get(CAP_PROP_FRAME_HEIGHT);
	qDebug() << u8"��" << capture.get(CAP_PROP_FRAME_WIDTH);
	qDebug() << u8"֡��FPS��" << QString::number(capture.get(CAP_PROP_FPS));
	qDebug() << u8"��֡�ʣ�" << capture.get(CAP_PROP_FRAME_COUNT);
}

void introduction::on_pushButton_save_video_clicked()
{
	//��Ƶ·��
	QString appPath = QCoreApplication::applicationDirPath();
	QString videoPath = appPath + "/vtest.avi";
	cv::VideoCapture capture;
	capture.open(videoPath.toStdString(), CAP_FFMPEG);


	qDebug() << u8"�ߣ�" << capture.get(CAP_PROP_FRAME_HEIGHT);
	qDebug() << u8"��" << capture.get(CAP_PROP_FRAME_WIDTH);
	qDebug() << u8"֡��FPS��" << capture.get(CAP_PROP_FPS);
	qDebug() << u8"��֡�ʣ�" << capture.get(CAP_PROP_FRAME_COUNT);

	//������Ƶ·��
	QString savevideoPath = appPath + "/output.avi";

	cv::VideoWriter writer(savevideoPath.toStdString(), capture.get(CAP_PROP_FOURCC), capture.get(CAP_PROP_FPS), Size(capture.get(CAP_PROP_FRAME_WIDTH), capture.get(CAP_PROP_FRAME_HEIGHT)));

	cv::Mat frame;
	while (true) {
		//��֡
		bool ret = capture.read(frame);
		if (!ret)
			break;
		imshow("frame", frame);
		//���֡���� 
		writer.write(frame);
		//�ȴ� 100 ���룬����û��Ƿ��¼����ϵİ���
		char c = waitKey(100);
		//���µ��� ESC �� (ASCII ֵΪ 27)��������ѭ����ֹͣ������Ƶ
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

	imshow("���ȶԱȶȵ���", dst);
}

static void on_track_contrast(int b, void* user_data)
{
	Mat image = *((Mat *)user_data);
	Mat dst = Mat::zeros(image.size(), image.type());
	Mat m = Mat::zeros(image.size(), image.type());

	double contrast = b / 100.0;
	addWeighted(image, contrast, m, 0.0, 0, dst);

	imshow("���ȶԱȶȵ���", dst);
}

void introduction::on_pushButton_brightness_clicked()
{
	namedWindow("���ȶԱȶȵ���", WINDOW_AUTOSIZE);
	
	int lightness = 50;
	int max_value = 100;

	int contrast_value = 100;

	createTrackbar("Value Bar:", "���ȶԱȶȵ���", &lightness, max_value, on_track_lightness,(void*)(&img));
	createTrackbar("Constrast Bar:", "���ȶԱȶȵ���", &contrast_value, 200, on_track_contrast, (void*)(&img));
	on_track_lightness(50, &img);
}
