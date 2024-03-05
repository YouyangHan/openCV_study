#pragma once

#include <QtWidgets/QWidget>
#include "ui_openCV_study.h"

class openCV_study : public QWidget
{
    Q_OBJECT

public:
    openCV_study(QWidget *parent = nullptr);
    ~openCV_study();

private:
    Ui::openCV_studyClass ui;
};
