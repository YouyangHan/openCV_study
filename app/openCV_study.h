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

    //��ʼ������
    void InitWindow();

    //��ʼ���źźͲ�
    void InitConnections();

	//��ʼ��ҳ��
	void InitPages();

private:
    Ui::openCV_studyClass *ui;

	//���
	introduction *introduct;
};
