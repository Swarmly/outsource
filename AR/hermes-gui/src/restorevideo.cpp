#include "restorevideo.h"
#include "commoncommands.h"
#include "ui_restorevideo.h"
RestoreVideo::RestoreVideo(QWidget *parent)
    : BaseGuiWidget(parent)
    , ui(new Ui::RestoreVideo)
{
    ui->setupUi(this);
    setWindowModality(Qt::ApplicationModal);
}

RestoreVideo::~RestoreVideo()
{
    delete ui;
}

void RestoreVideo::on_rxHWReset_clicked()
{
    commandSend(cmd_rf_reboot_rx);
}

void RestoreVideo::on_rxSoftReset_clicked()
{
    commandSend(cmd_rf_reopen_rx);
}

void RestoreVideo::on_txHardReset_clicked()
{
    commandSend(cmd_rf_reboot_tx);
}

void RestoreVideo::on_gimballReboot_clicked()
{
    commandSend(cmd_reboot_gimbal);
}
