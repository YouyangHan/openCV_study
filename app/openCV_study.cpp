#include "openCV_study.h"

openCV_study::openCV_study(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::openCV_studyClass)
{
    ui->setupUi(this);

    //初始化窗口
    InitWindow();
    //初始化信号和槽
    InitConnections();
	//初始化页面
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
  //页面切换
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
