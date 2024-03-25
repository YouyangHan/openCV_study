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
	//capture.open(videoPath.toStdString(),CAP_FFMPEG);
	//������ͷ��ȡ
	capture.open(0, CAP_DSHOW);

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

void introduction::on_pushButton_keyboard_clicked()
{
  //ͼƬ·��
  QString appPath = QCoreApplication::applicationDirPath();
  imagePath = appPath + "/A.jpg";

  //��ȡͼ��
  img = cv::imread(imagePath.toStdString());
  if (img.empty())
    return;

  cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
  //��ʾͼ��
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

  //����Ϊ0һֱ����
  cv::waitKey(0);
  //���ٴ���
  cv::destroyAllWindows();
}

void introduction::on_pushButton_color_surface_clicked()
{
  //ͼƬ·��
  QString appPath = QCoreApplication::applicationDirPath();
  imagePath = appPath + "/A.jpg";

  //��ȡͼ��
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
  imshow("�����", dst);

  bitwise_or(m1, m2, dst);
  imshow("�����", dst);

  QString appPath = QCoreApplication::applicationDirPath();
  imagePath = appPath + "/A.jpg";
  img = cv::imread(imagePath.toStdString());
  if (img.empty())
    return;
  bitwise_not(img, dst);
  imshow("�ǲ���", dst);

  bitwise_xor(m1, m2, dst);
  imshow("������", dst);

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
	imshow("ͨ�����", dst);
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

void introduction::on_pushButton_pixel_statistic_clicked()
{
  QString appPath = QCoreApplication::applicationDirPath();
  imagePath = appPath + "/A.jpg";
  img = cv::imread(imagePath.toStdString());
  if (img.empty())
    return;

  double minv, maxv;
  Point minLoc, maxLoc;

  std::vector<Mat> mv;
  split(img, mv);
  for (int i = 0; i < mv.size(); i++)
  {
    //ͼ����뵥ͨ��
    minMaxLoc(mv[i], &minv, &maxv, &minLoc, &maxLoc, cv::Mat());
    std::cout << "channel" << i << "min value" << minv << "max value" << maxv<<std::endl ;
  }

  Mat mean, stddev;
  //�����ֵ����
  meanStdDev(img,mean,stddev);
  std::cout << "mean" << mean << "stddev" << stddev << std::endl;
}

void introduction::on_pushButton_paint_clicked()
{
  QString appPath = QCoreApplication::applicationDirPath();
  imagePath = appPath + "/A.jpg";
  img = cv::imread(imagePath.toStdString());
  if (img.empty())
    return;

  Mat bg = Mat::zeros(img.size(),img.type());

  Rect rect;
  rect.x = 200;
  rect.y = 200;
  rect.width = 100;
  rect.height = 50;
  rectangle(bg, rect, Scalar(0, 0, 255), -1,8,0);
  circle(bg, Point(50, 100), 15, Scalar(255, 0, 0), 2);
  line(bg, Point(100, 100), Point(100, 200), Scalar(0, 255, 0), 2, LINE_AA, 0);
  RotatedRect rrt(Point(200, 200),Size(100, 200), 90.0);
  ellipse(bg, rrt, Scalar(0, 255, 255), 2, 8);

  Mat dst;
  addWeighted(img, 0.7,bg,0.3,0, dst);
  imshow("Draw", dst);
}

void introduction::on_pushButton_paint_random_clicked()
{
  Mat bg = Mat::zeros(Size(512,512), CV_8UC3);
  int w = bg.cols;
  int h = bg.rows;
  RNG rng(12345);
  while (true)
  {
    int c = cv::waitKey(10);
    if (c == 27) {
      break;
    }
    int x1 = rng.uniform(0, w);
    int y1 = rng.uniform(0, h);
    int x2 = rng.uniform(0, w);
    int y2 = rng.uniform(0, h);
    bg = Scalar(0, 0, 0);
    line(bg, Point(x1, y1), Point(x2, y2), Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255)), 2, LINE_AA, 0);
    imshow("Draw_random", bg);
  }

}

void introduction::on_pushButton_paint_polygon_clicked()
{
  Mat bg = Mat::zeros(Size(512, 512), CV_8UC3);
  Point p1(100, 100);
  Point p2(350, 100);
  Point p3(450, 280);
  Point p4(320, 450);
  Point p5(80, 400);
  std::vector<Point> pts;
  pts.push_back(p1);
  pts.push_back(p2);
  pts.push_back(p3);
  pts.push_back(p4);
  pts.push_back(p5);

  polylines(bg, pts, true, Scalar(0, 0, 255), 2, 8, 0);
  fillPoly(bg, pts, Scalar(0, 255, 255),8, 0);

  std::vector<std::vector<Point>> contours;
  contours.push_back(pts);
  drawContours(bg, contours, -1, Scalar(255, 0, 255), 2);

  imshow("Draw_polygon", bg);
}

Point sp(-1, -1);
Point ep(-1, -1);
Mat temp;
static void on_draw(int event, int x, int y, int flags,void *user_data)
{
  Mat image = *((Mat *)user_data);
  if (event == EVENT_LBUTTONDOWN)
  {
    sp.x = x;
    sp.y = y;
  }
  else if (event == EVENT_LBUTTONUP)
  {
    ep.x = x;
    ep.y = y;
    int dx = ep.x - sp.x;
    int dy = ep.y - sp.y;
    if (dx > 0 && dy > 0)
    {
      Rect rect(sp.x,sp.y,dx,dy);
      rectangle(image, rect, Scalar(0, 0, 255), 2, 8, 0);
      imshow("mouse_draw", image);
      imshow("roi", image(rect));
      sp.x = -1;
      sp.y = -1;
    }
  }
  else if (event == EVENT_MOUSEMOVE)
  {
    if (sp.x > 0 && sp.y > 0)
    {
      ep.x = x;
      ep.y = y;
      int dx = ep.x - sp.x;
      int dy = ep.y - sp.y;
      if (dx > 0 && dy > 0)
      {
        temp.copyTo(image);

        Rect rect(sp.x, sp.y, dx, dy);
        rectangle(image, rect, Scalar(0, 0, 255), 2, 8, 0);
        imshow("mouse_draw", image);
      }
    }
  }
}

void introduction::on_pushButton_mouse_clicked()
{
  QString appPath = QCoreApplication::applicationDirPath();
  imagePath = appPath + "/A.jpg";
  img = cv::imread(imagePath.toStdString());
  if (img.empty())
    return;
  temp = img.clone();
  namedWindow("mouse_draw", WINDOW_AUTOSIZE);
  setMouseCallback("mouse_draw", on_draw,(void*)(&img));

  imshow("mouse_draw", img);
}