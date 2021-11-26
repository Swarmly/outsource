#include "testar.h"
#include "./ui_testar.h"

TestAR::TestAR(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TestAR)
{
    ui->setupUi(this);
}

TestAR::~TestAR()
{
    delete ui;
}

