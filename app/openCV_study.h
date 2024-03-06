#pragma once

#include <QtWidgets/QWidget>
#include <QScreen>

#include "ui_openCV_study.h"

#include "introduction.h"

namespace Ui{
  class openCV_studyClass;
}

class openCV_study : public QWidget
{
    Q_OBJECT

public:
    openCV_study(QWidget *parent = nullptr);
    ~openCV_study();

    //初始化出口
    void InitWindow();

    //初始化信号和槽
    void InitConnections();

	//初始化页面
	void InitPages();

private:
    Ui::openCV_studyClass *ui;

	//简介
	introduction *introduct;
};
