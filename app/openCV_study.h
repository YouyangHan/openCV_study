#pragma once

#include <QtWidgets/QWidget>
#include <QScreen>

#include "ui_openCV_study.h"

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

private:
    Ui::openCV_studyClass *ui;
};
