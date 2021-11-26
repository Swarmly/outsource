#include "gimbal.h"
#include "ui_gimbal.h"

Gimbal::Gimbal(QWidget *parent)
    : BaseGuiWidget(parent)
    , ui(new Ui::Gimbal)
{
    ui->setupUi(this);
}

Gimbal::~Gimbal()
{
    delete ui;
}
