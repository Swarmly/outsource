#include "setmavuavmode.h"
#include "ui_setmavuavmode.h"

setMavUavMode::setMavUavMode(QWidget *parent)
    : BaseGuiWidget(parent)
    , ui(new Ui::setMavUavMode)
{
    ui->setupUi(this);
}

setMavUavMode::~setMavUavMode()
{
    delete ui;
}

void setMavUavMode::on_pushButtonAuto_clicked() {}

void setMavUavMode::on_pushButton433_clicked() {}

void setMavUavMode::on_pushButton866_clicked() {}

void setMavUavMode::on_pushButtonMixed_clicked() {}
