#include "irmode.h"
#include "commoncommands.h"
#include "ui_irmode.h"

IRMode::IRMode(QWidget *parent)
    : BaseGuiWidget(parent)
    , ui(new Ui::IRMode)
{
    ui->setupUi(this);
}

IRMode::~IRMode()
{
    delete ui;
}

void IRMode::on_pushButtonDay_clicked()
{
    commandSend(cmd_gimbal_DAY);
}

void IRMode::on_pushButtonDayIR_clicked()
{
    commandSend(cmd_gimbal_IR);
}

void IRMode::on_pushButtonNUC_clicked()
{
    commandSend(cmd_gimbal_NUC);
}

void IRMode::on_pushButtonWhColorPalette_clicked()
{
    commandSend(cmd_gimbal_PALETTE, 1);
}

void IRMode::on_pushButtonWhBWPalette_clicked()
{
    commandSend(cmd_gimbal_PALETTE, 0);
}

void IRMode::on_pushButtonBH_clicked()
{
    commandSend(cmd_gimbal_BH);
}

void IRMode::on_pushButtonWH_clicked()
{
    commandSend(cmd_gimbal_WH);
}
