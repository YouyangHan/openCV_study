#include "openCV_study.h"

openCV_study::openCV_study(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::openCV_studyClass)
{
    ui->setupUi(this);

    //��ʼ������
    InitWindow();
    //��ʼ���źźͲ�
    InitConnections();
	//��ʼ��ҳ��
	InitPages();
}

openCV_study::~openCV_study()
{}

void openCV_study::InitWindow()
{
  QScreen *primaryScreen = QGuiApplication::primaryScreen();
  QRect primaryScreenGeometry = primaryScreen->geometry();

  this->setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

  this->move(primaryScreenGeometry.x(), primaryScreenGeometry.y());
}

void openCV_study::InitConnections()
{
  ui->stackedWidget->setCurrentIndex(1);
  //ҳ���л�
  connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=] (int index) {
    ui->stackedWidget->setCurrentIndex(index+1);
  });
}

void openCV_study::InitPages()
{
  introduct = new introduction;
  ui->stackedWidget->addWidget(introduct);

  act = new actual;
  ui->stackedWidget->addWidget(act);

}
