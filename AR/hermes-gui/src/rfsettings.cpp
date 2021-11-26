#include "rfsettings.h"
#include "ui_rfsettings.h"

RFSettings::RFSettings(QWidget *parent)
    : BaseGuiWidget(parent)
    , ui(new Ui::RFSettings)
{
    ui->setupUi(this);
}

RFSettings::~RFSettings()
{
    delete ui;
}

void RFSettings::on_pushButtonSetChannel_clicked() {}

void RFSettings::on_pushButtonSetGain_clicked() {}
