#pragma once
#include "baseguiwidget.h"

namespace Ui {
class RestoreVideo;
}

class HERMESGUI_EXPORT RestoreVideo : public BaseGuiWidget
{
    Q_OBJECT

public:
    explicit RestoreVideo(QWidget *parent = nullptr);
    ~RestoreVideo();

signals:
    void commandSend(const QString &command, const boost::any &params = nullptr);

private slots:
    void on_rxHWReset_clicked();
    void on_rxSoftReset_clicked();
    void on_txHardReset_clicked();
    void on_gimballReboot_clicked();

private:
    Ui::RestoreVideo *ui;
};
