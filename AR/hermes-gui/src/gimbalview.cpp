#include "gimbalview.h"
#include "commoncommands.h"
#include "ui_gimbalview.h"

GimbalView::GimbalView(QWidget *parent)
    : BaseGuiWidget(parent)
    , ui(new Ui::GimbalView)
{
    ui->setupUi(this);
}

GimbalView::~GimbalView()
{
    delete ui;
}

void GimbalView::on_pushButtonEpr_clicked()
{
    commandSend(cmd_gimbal_EPR);
}

void GimbalView::on_pushButtonGrr_clicked()
{
    commandSend(cmd_gimbal_GRR);
}

void GimbalView::on_pushButtonObs_clicked()
{
    commandSend(cmd_gimbal_OBSERVATION);
}

void GimbalView::on_pushButtonStow_clicked()
{
    commandSend(cmd_gimbal_STOW);
}

void GimbalView::on_pushButtonPilot_clicked()
{
    commandSend(cmd_gimbal_PILOT);
}

void GimbalView::on_pushButtonHold_clicked()
{
    commandSend(cmd_gimbal_HOLD_COORD);
    close();
}

void GimbalView::on_pushButtonPV1_clicked()
{
    commandSend(cmd_gimbal_PV1);
}

void GimbalView::on_pushButtonPV2_clicked()
{
    commandSend(cmd_gimbal_PV2);
}

void GimbalView::on_pushButtonGPOS_clicked()
{
    commandSend(cmd_gimbal_GPOS);
}

void GimbalView::on_pushButtonLPOS_clicked()
{
    commandSend(cmd_gimbal_LPOS);
}
